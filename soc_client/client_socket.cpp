#include "client_socket.h"
#include <stdexcept>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <iostream>

#define SOCKET_TERMINATE_CHAR '\0'

using namespace std;

ClientSocket::ClientSocket(SOCKET socket) :
        connectSocket(socket) {

}



bool ClientSocket::createConnection(string hostName, long port) {
    WSADATA wsaData;
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        throw runtime_error("WSAStartup failed with error: " + to_string(iResult) + "\n");
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(hostName.c_str(), to_string(port).c_str(), &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        throw runtime_error("getaddrinfo failed with error: " + to_string(iResult) + "\n");
    }

    // Create a SOCKET for connecting to server
    SOCKET connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (connectSocket == INVALID_SOCKET) {
        WSACleanup();
        throw runtime_error("socket failed with error: " + to_string(WSAGetLastError()) + "\n");
    }

    // Connect to server
    iResult = connect(connectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        throw runtime_error("Unable to connect to server.\n");
    }
    
    return new ClientSocket(connectSocket);
}


ClientSocket::~ClientSocket() {
    if (this->connectSocket != INVALID_SOCKET) {
        closesocket(this->connectSocket);
        this->connectSocket = INVALID_SOCKET;
    }
    WSACleanup();
}

void ClientSocket::connectionOutput(int i) {
    cout << "Client " << i << " was successfully connected to server !";
}





#undef SOCKET_TERMINATE_CHAR