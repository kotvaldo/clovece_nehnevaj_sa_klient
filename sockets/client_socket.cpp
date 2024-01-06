//
// Created by Lenovo IdeaPad 310 on 6. 1. 2024.
//

#include "client_socket.h"

bool client_socket_init(struct client_socket *clientSocket, char *hostName, int port) {
    clientSocket->server = gethostbyname(hostName);
    if (clientSocket->server == nullptr) {
        cout << "Host was not found" << endl;
        return false;
    }


    bzero(reinterpret_cast<char *>(&clientSocket->serv_addr), sizeof(clientSocket->serv_addr));
    clientSocket->serv_addr.sin_family = AF_INET;
    std::memcpy(
            reinterpret_cast<char *>(&clientSocket->serv_addr.sin_addr.s_addr),
            clientSocket->server->h_addr,
            clientSocket->server->h_length
    );
    clientSocket->serv_addr.sin_port = htons(port);

    clientSocket->socket_descriptior = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket->socket_descriptior < 0) {
        cout << "Error creating socket !" << endl;
        return false;
    }

    cout << "Client socket " << clientSocket->id <<" was successfully initialized" << endl;
    return true;
};


bool client_socket_connect(struct client_socket *clientSocket) {
    if (connect(clientSocket->socket_descriptior, reinterpret_cast<struct sockaddr *>(&clientSocket->serv_addr), sizeof(clientSocket->serv_addr)) < 0)
    {
        cout << "Error while connecting.";
        return false;
    }

    cout << "Client " << clientSocket->id << " was successfully connected " << endl;
    return true;
};