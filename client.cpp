//
// Created by adams on 5. 1. 2024.
//

#include "sockets/client_socket.h"
#include <iostream>
#include <thread>

using namespace std;

struct client_data {
    client_socket *clientSocket = nullptr;
    bool connected = false;


};

void spracuj_pokyn(struct client_socket *client) {
    string postup{};
    if (client->data.back().length() >= 100) {
        cout << client->data.back() << endl;
    } else if (client->data.back() == "vypis") {
        cout << endl;
        cout << "Si na tahu:" << endl;
        cout << "Vyber z moznosti: " << endl;
        cout << "   1. Hod kockou" << endl;
        cout << "   5. Koneic hry" << endl;
        cout << endl;

        cout << "Zadajte ciselnu moznost." << endl;
        cin >> postup;
        while (postup != "1" || postup != "5") {
            cout << "Zle zadana moznost.";
            cin >> postup;
        }
        client_socket_write(client, postup);

    } else if (client->data.back() == "hod") {
        client_socket_read(client);
        cout << "Hodil si " << client->data.back();
        cout << "Vyber z moznosti: " << endl;
        cout << "   3. Posun : " << endl;
        cout << "   5. Koneic hry" << endl;
        cout << endl;

        cout << "Zadajte ciselnu moznost." << endl;
        cin >> postup;
        while (postup != "3" || postup != "5") {
            cout << "Zle zadana moznost.";
            cin >> postup;
        }
        client_socket_write(client, postup);
    } else if (client->data.back() == "hod_sestkou") {
        cout << "Hodil si šestkou !: " << endl;
        cout << "   3. Posun" << endl;
        cout << "   4. Postav noveho panacika" << endl;
        cout << "   5. Koneic hry" << endl;
        cout << endl;

        cout << "Zadajte ciselnu moznost." << endl;
        cin >> postup;
        while (postup != "3" || postup != "4" || postup != "5") {
            cout << "Zle zadana moznost.";
            cin >> postup;
        }
        client_socket_write(client, postup);
    }

}


int main(int argc, char *argv[]) {

    char *hostname = (char *) "frios2.fri.uniza.sk";
    int port = 11000;
    client_socket clientSocket;
    client_data client = {&clientSocket};


    while (true) {
        if (client_socket_init(&clientSocket, hostname, port)) {
            client.connected = client_socket_connect(&clientSocket);
            if (client.connected) { break; }
            else {
                cout << endl;
                cout << "Pausing for 5 seconds" << endl;
                this_thread::sleep_for(std::chrono::seconds(5));

            }
        }
    }

    while (client.clientSocket->data.back() != "end") {
        client.clientSocket->data.clear();
        client_socket_read(client.clientSocket);
        spracuj_pokyn(client.clientSocket);
    }
    client_socket_destroy(client.clientSocket);
    return 0;
}