//
// Created by Lenovo IdeaPad 310 on 6. 1. 2024.
//

#ifndef CLOVECE_NEHNEVAJ_SA_KLIENT_2_CLIENT_SOCKET_H
#define CLOVECE_NEHNEVAJ_SA_KLIENT_2_CLIENT_SOCKET_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

struct client_socket {
    int id;
    int socket_descriptior;
    pthread_mutex_t mutex;
    bool isConnecting;
    struct sockaddr_in serv_addr;
    struct hostent *server;

};

bool client_socket_init(struct client_socket *clientSocket, char* hostname, int port);
bool client_socket_connect(struct client_socket *clientSocket);
void client_socket_destroy(struct client_socket *clientSocket);

#endif //CLOVECE_NEHNEVAJ_SA_KLIENT_2_CLIENT_SOCKET_H
