//
// Created by adams on 5. 1. 2024.
//

#include "sockets/client_socket.h"

#define HOSTNAME 0 = (char*)"frios2.fri.uniza.sk"
#define PORT = 11000

int main(int argc, char *argv[]) {

    char* hostname = (char*)"frios2.fri.uniza.sk";
    client_socket player1 = {1};
    client_socket player2 = {2};
    client_socket player3 = {3};
    client_socket player4 = {4};

    client_socket_init(&player1, hostname, 11000);
    client_socket_init(&player2, hostname, 11000);
    client_socket_init(&player3, hostname, 11000);
    client_socket_init(&player4, hostname, 11000);

    if(client_socket_connect(&player1) && client_socket_connect(&player2) && client_socket_connect(&player3) && client_socket_connect(&player4) ) {
        cout <<  "Všetky boli pripojené" << endl;
    }

    return 0;
}