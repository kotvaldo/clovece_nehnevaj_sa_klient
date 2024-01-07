//
// Created by adams on 5. 1. 2024.
//

#include "sockets/client_socket.h"
#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char *argv[]) {

    char *hostname = (char *) "frios2.fri.uniza.sk";
    int port = 11000;
    client_socket clientSocket = {};

    bool connected = false;
    while (true) {
        if (client_socket_init(&clientSocket, hostname, port)) {
            connected = client_socket_connect(&clientSocket);
            if (connected) { break; }
            else {
                cout << endl;
                cout << "Pausing for 5 seconds" << endl;
                this_thread::sleep_for(std::chrono::seconds(5));

            }
        }
    }


    client_socket_read(&clientSocket);
    if (clientSocket.data.back() != "error") {
        client_socket_write(&clientSocket, "message");
    }

    for (int i = 0; i < clientSocket.data.size(); ++i) {
        cout << clientSocket.data[i] << endl;
    }

    system("pause");
    client_socket_destroy(&clientSocket);

    return 0;
}