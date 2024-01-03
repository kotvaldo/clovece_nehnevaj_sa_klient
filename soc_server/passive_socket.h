//
// Created by adams on 3. 1. 2024.
//

#ifndef CLOVECENEZLOBSA_PASSIVE_SOCKET_H
#define CLOVECENEZLOBSA_PASSIVE_SOCKET_H

#include <winsock2.h>
#include "active_socket.h"

struct passive_socket {
    SOCKET socket_descriptor;
    bool is_listening;
    bool is_waiting;
    CRITICAL_SECTION mutex;
    CONDITION_VARIABLE waiting_finished;

};

void passive_socket_init(struct passive_socket *self);

void passive_socket_destroy(struct passive_socket *self);

bool passive_socket_start_listening(struct passive_socket *self, short port);

void passive_socket_stop_listening(struct passive_socket *self);

bool passive_socket_is_listening(struct passive_socket *self);

bool passive_socket_wait_for_client(struct passive_socket *self, struct active_socket *client_sock);


#endif //CLOVECENEZLOBSA_PASSIVE_SOCKET_H
