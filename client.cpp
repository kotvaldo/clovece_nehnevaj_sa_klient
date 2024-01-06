//
// Created by adams on 5. 1. 2024.
//

#include "client_socket.h"

#define HOSTNAME = "frios2.fri.uniza.sk"
#define PORT = 11000

int main(int argc, char *argv[])
{
    client_socket clientSocket;
    clientSocket.create_connection(HOSTNAME, PORT);

    return 0;
}