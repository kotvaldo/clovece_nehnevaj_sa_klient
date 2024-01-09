//
// Created by Lenovo IdeaPad 310 on 6. 1. 2024.
//

#ifndef CLOVECE_NEHNEVAJ_SA_KLIENT_2_CLIENT_SOCKET_H
#define CLOVECE_NEHNEVAJ_SA_KLIENT_2_CLIENT_SOCKET_H

#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>


using namespace std;

struct client_socket {
    SOCKET socket_descriptor = INVALID_SOCKET;
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    bool is_reading = false;
    pthread_mutex_t mutex_reading;
    pthread_mutex_t mutex_writing;
    pthread_mutex_t mutex_received_data;
    vector<string> data = {"init"};
};

bool client_socket_init(struct client_socket *clientSocket, char* hostname, int port);
bool client_socket_connect(struct client_socket *clientSocket);
void client_socket_destroy(struct client_socket *self);
void client_socket_read(struct client_socket *self);
void client_socket_write(struct client_socket *clientSocket, string message);
bool client_socket_is_reading(struct client_socket *self);
void client_socket_stop_reading(struct client_socket *self);
#endif //CLOVECE_NEHNEVAJ_SA_KLIENT_2_CLIENT_SOCKET_H