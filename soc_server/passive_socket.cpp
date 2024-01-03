#include "passive_socket.h"
#include "active_socket.h"

PassiveSocket::PassiveSocket() : socketDescriptor(INVALID_SOCKET), isListening(false), isWaiting(false) {
    InitializeCriticalSection(&mutex);
    InitializeConditionVariable(&waitingFinished);
}

PassiveSocket::~PassiveSocket() {
    if (socketDescriptor != INVALID_SOCKET) {
        closesocket(socketDescriptor);
    }
    socketDescriptor = INVALID_SOCKET;
    isListening = false;
    isWaiting = false;
    DeleteCriticalSection(&mutex);
}

bool PassiveSocket::startListening(short port) {
    EnterCriticalSection(&mutex);
    if (isListening || isWaiting) {
        LeaveCriticalSection(&mutex);
        std::cerr << "PassiveSocket is listening. Cannot call startListening again." << std::endl;
        return false;
    }

    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor == INVALID_SOCKET) {
        perror("socket failed.");
        LeaveCriticalSection(&mutex);
        return false;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(socketDescriptor, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        perror("bind failed.");
        LeaveCriticalSection(&mutex);
        return false;
    }

    listen(socketDescriptor, 10);
    isListening = true;
    LeaveCriticalSection(&mutex);

    return true;
}

void PassiveSocket::stopListening() {
    EnterCriticalSection(&mutex);
    if (!isListening) {
        LeaveCriticalSection(&mutex);
        return;
    }

    isListening = false;
    while (isWaiting) {
        SleepConditionVariableCS(&waitingFinished, &mutex, INFINITE);
    }

    closesocket(socketDescriptor);
    socketDescriptor = INVALID_SOCKET;

    LeaveCriticalSection(&mutex);
}

bool PassiveSocket::isListeningNow() {
    EnterCriticalSection(&mutex);
    bool listening = isListening;
    LeaveCriticalSection(&mutex);
    return listening;
}

bool PassiveSocket::waitForClient(ActiveSocket* clientSock) {
    EnterCriticalSection(&mutex);
    if (isWaiting) {
        LeaveCriticalSection(&mutex);
        std::cerr << "PassiveSocket is waiting for a client. Cannot call waitForClient again." << std::endl;
        return false;
    }

    isWaiting = true;
    LeaveCriticalSection(&mutex);

    sockaddr_in clientAddress{};
    int clientAddressLength = sizeof(clientAddress);

    while (isListeningNow()) {
        SOCKET clientSocket = accept(socketDescriptor, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);

        if (clientSocket == INVALID_SOCKET) {
            perror("accept failed.");
            return false;
        }

        clientSock->setSocketDescriptor(clientSocket);

        EnterCriticalSection(&mutex);
        isWaiting = false;
        LeaveCriticalSection(&mutex);
        WakeConditionVariable(&waitingFinished);

        return true;
    }

    EnterCriticalSection(&mutex);
    isWaiting = false;
    WakeConditionVariable(&waitingFinished);
    LeaveCriticalSection(&mutex);

    return false;
}