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
    sockaddr_in cli_addr;
    SOCKET connectSocket;
    pthread_mutex_t mutex;
    pthread_cond_t waiting_finished;

    client_socket(SOCKET socket);
    ~client_socket();
    void create_connection(const char* host_name, int port);
    void send_data(struct client_socket* clientSocket);
};


#endif //CLOVECE_NEHNEVAJ_SA_KLIENT_2_CLIENT_SOCKET_H
