//
// Created by adams on 5. 1. 2024.
//

#include "sockets/client_socket.h"

#define HOSTNAME 0 = (char*)"frios2.fri.uniza.sk"
#define PORT = 11000

int main(int argc, char *argv[]) {

    char* hostname = (char*)"frios2.fri.uniza.sk";
    client_socket clientSocket = {1};
    client_socket_init(&clientSocket, hostname, 11000);
    client_socket_connect(&clientSocket);

    system("pause");
    return 0;
}