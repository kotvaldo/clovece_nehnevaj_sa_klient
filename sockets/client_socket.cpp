//
// Created by Lenovo IdeaPad 310 on 6. 1. 2024.
//

#include "client_socket.h"

bool client_socket_init(struct client_socket *clientSocket, char *hostName, int port) {
    // Initialize Winsock
    WSADATA wsaData;
    int result = 0;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Failed to initialize Winsock." << endl;
        return false;
    } else {
        cout << "Winsock initialized" << endl;
    }

    ZeroMemory(&clientSocket->hints, sizeof(clientSocket->hints));
    clientSocket->hints.ai_family = AF_UNSPEC;
    clientSocket->hints.ai_socktype = SOCK_STREAM;
    clientSocket->hints.ai_protocol = IPPROTO_TCP;

    result = getaddrinfo(hostName, std::to_string(port).c_str(), &clientSocket->hints, &clientSocket->result);
    if (result != 0) {
        cout << "Host not exist !" << endl;
        WSACleanup();
        return false;
    } else {
        cout << "Host exists !" << endl;
    }

    clientSocket->socket_descriptior = socket(AF_INET, clientSocket->result->ai_socktype, clientSocket->result->ai_protocol);
    if (clientSocket->socket_descriptior == INVALID_SOCKET) {
        WSACleanup();
        cout << "Socket Failed to create" << endl;
    }

    cout << "Client socket was successfully initialized" << endl;
    return true;
}

bool client_socket_connect(struct client_socket *clientSocket) {
    if (connect(clientSocket->socket_descriptior, clientSocket->result->ai_addr, (int)clientSocket->result->ai_addrlen) == SOCKET_ERROR) {
        cout << "Error while connecting." << endl;
        closesocket(clientSocket->socket_descriptior);
        WSACleanup();
        return false;
    }

    cout << "Client  was successfully connected " << endl;
    return true;
}