//
// Created by Lenovo IdeaPad 310 on 6. 1. 2024.
//

#include <sstream>
#include "client_socket.h"
#define SOCKET_TERMINATE_CHAR '\0'
bool client_socket_init(struct client_socket *clientSocket, char *hostName, int port) {
    // Initialize Winsock

    clientSocket->is_reading = false;
    pthread_mutex_init(&clientSocket->mutex_reading, nullptr);
    pthread_mutex_init(&clientSocket->mutex_writing, nullptr);
    pthread_mutex_init(&clientSocket->mutex_received_data, nullptr);

    WSADATA wsaData;
    int result;
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

    clientSocket->socket_descriptor = socket(AF_INET, clientSocket->result->ai_socktype, clientSocket->result->ai_protocol);
    if (clientSocket->socket_descriptor == INVALID_SOCKET) {
        WSACleanup();
        cout << "Socket Failed to create" << endl;
    }

    cout << "Client socket was successfully initialized" << endl;
    return true;
}

void client_socket_destroy(struct client_socket *self) {
    pthread_mutex_destroy(&self->mutex_reading);
    pthread_mutex_destroy(&self->mutex_writing);
    pthread_mutex_destroy(&self->mutex_received_data);
    self->data.clear();
    self->is_reading = false;
    if (self->socket_descriptor >= 0) {
        closesocket(self->socket_descriptor);
    }
}

bool client_socket_connect(struct client_socket *clientSocket) {
    if (connect(clientSocket->socket_descriptor, clientSocket->result->ai_addr, (int)clientSocket->result->ai_addrlen) == SOCKET_ERROR) {
        cout << "Error while connecting." << endl;
        closesocket(clientSocket->socket_descriptor);
        WSACleanup();
        return false;
    }

    cout << "Client  was successfully connected " << endl;
    return true;
}

void client_socket_stop_reading(struct client_socket *self) {
    pthread_mutex_lock(&self->mutex_reading);
    self->is_reading = false;
    pthread_mutex_unlock(&self->mutex_reading);
}

bool client_socket_is_reading(struct client_socket *self) {
    pthread_mutex_lock(&self->mutex_reading);
    bool can_read = self->is_reading;
    pthread_mutex_unlock(&self->mutex_reading);
    return can_read;
}
void client_socket_read(struct client_socket *self) {
    pthread_mutex_lock(&self->mutex_reading);

    if (self->is_reading) {
        pthread_mutex_unlock(&self->mutex_reading);
        fprintf(stderr, "active_socket is reading. Cannot call start_reading again.\n");
        return;
    }

    self->is_reading = true;
    pthread_mutex_unlock(&self->mutex_reading);

    fd_set sockets;
    struct timeval tv;
    tv.tv_sec = 1;  // Set an appropriate waiting time

    while (client_socket_is_reading(self)) {
        FD_ZERO(&sockets);
        FD_SET(self->socket_descriptor, &sockets);

        int result = select(0, &sockets, nullptr, nullptr, &tv);

        if (result == SOCKET_ERROR) {
            perror("select failed.");
            client_socket_stop_reading(self);
            closesocket(self->socket_descriptor);
            self->socket_descriptor = INVALID_SOCKET;
            break;
        }

        if (result > 0) {
            char buffer[1024] = {};  // Initialize buffer to 0

            ssize_t bytesRead = recv(self->socket_descriptor, buffer, sizeof(buffer), 0);

            if (bytesRead > 0) {
                stringstream ss;

                for (size_t i = 0; i < bytesRead; ++i) {
                    char ch = buffer[i];

                    if (ch == SOCKET_TERMINATE_CHAR) {
                        self->data.push_back(ss.str());
                        ss.str("");
                        client_socket_stop_reading(self);
                    } else {
                        ss << ch;
                    }
                }
            } else {
                client_socket_stop_reading(self);
                break;
            }
        }
    }
}


void client_socket_write(struct client_socket *clientSocket, string message) {
    size_t data_length = message.length();
    char* buffer = (char*)calloc(data_length + 1, sizeof(char));
    memcpy(buffer, message.c_str(), data_length);
    buffer[data_length] = SOCKET_TERMINATE_CHAR;

    int result = send(clientSocket->socket_descriptor, buffer, data_length + 1, 0 );
    if (result == SOCKET_ERROR) {
        throw std::runtime_error("send failed with error: " + std::to_string(WSAGetLastError()) + "\n");
    }
    free(buffer);
    buffer = nullptr;
}