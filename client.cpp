//
// Created by adams on 5. 1. 2024.
//

#include "sockets/client_socket.h"

int main(int argc, char *argv[])
{
    client_socket clientSocket;
    clientSocket.create_connection("frios2.fri.uniza.sk", 11000);

    return 0;
}