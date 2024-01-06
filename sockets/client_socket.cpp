//
// Created by Lenovo IdeaPad 310 on 6. 1. 2024.
//

#include "client_socket.h"

#define INVALID_SOCKET = -1

client_socket::client_socket(SOCKET socket) {
    connectSocket(socket);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&waiting_finished, NULL);
}

client_socket::~client_socket() {
    if (this->connectSocket != INVALID_SOCKET) {
        closesocket(this->connectSocket);
        this->connectSocket = INVALID_SOCKET;
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&waiting_finished);
}

void client_socket::create_connection(const char* hostName, int port) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    server = gethostbyname((char *)HOSTNAME);
    if (server == NULL) {
        throw runtime_error("Error, no such host");
    }

    bzero(reinterpret_cast<char *>(&serv_addr), sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(
            reinterpret_cast<char *>(&serv_addr.sin_addr.s_addr),
            server->h_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(PORT);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw runtime_error("Error creating socket");
    }

    if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0) {
        throw runtime_error("Error connecting to socket");
    } else {
        send_data();
    }

    close(sockfd);
}

void client_socket::send_data(struct client_socket *clientSocket) {
    pthread_mutex_lock(&clientSocket->mutex);
    string input = "";
    while(input == "") {
        cout << "Zdaja spravu: " << endl;
        cin >> input >> endl;
    }
    char *buffer = new char[input.length() + 1];
    memcpy(buffer, input.c_str(), input.length());
    buffer[input.length()] = SOCKET_TERMINATE_CHAR;

    int n = send(connectSocket, buffer, input.length() + 1, 0);
    if (n == SOCKET_ERROR) {
        throw runtime_error("send failed\n");
    }
    delete[] buffer;
    pthread_mutex_unlock(&clientSocket->mutex);
}