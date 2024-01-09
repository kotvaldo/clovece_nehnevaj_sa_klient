//
// Created by adams on 5. 1. 2024.
//

#include "sockets/client_socket.h"
#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

typedef struct client_data {
    client_socket *clientSocket = nullptr;
    bool connected = false;
    pthread_mutex_t *mutex;
    pthread_cond_t *isCas;
} CLIENT_DATA;

void deserialize_and_print(const string& serializedData) {
    char result[11][11];

    istringstream iss(serializedData);

    // Read the serialized data and populate the doska structure
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            iss >> result[i][j];
        }
    }

    cout << endl;
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            cout << result[i][j] << ' ';
        }
        cout << endl;
    }

}


void spracuj_pokyn(struct client_socket *client) {
    string postup{};

    if (client->data.empty()) {
        // Handle the case where client data is empty
        return;
    }

    if (client->data.back().length() >= 50) {
        deserialize_and_print(client->data.back());
    } else if (client->data.back() == "vypis") {
        cout << endl;
        cout << "Si na tahu:" << endl;
        cout << "Vyber z moznosti: " << endl;
        cout << "   1. Hod kockou" << endl;
        cout << "   5. Koniec hry" << endl;
        cout << endl;

        cout << "Zadajte ciselnu moznost." << endl;
        cin >> postup;

        while (postup != "1" && postup != "5") {
            cout << "Zle zadana moznost. Zadajte znova: ";
            cin.clear();  // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore incorrect input
            cin >> postup;
        }

        client_socket_write(client, postup);

    } else if (client->data.back() == "hod") {
        client_socket_read(client);
        cout << "Hodil si " << client->data.back();
        cout << "Vyber z moznosti: " << endl;
        cout << "   3. Posun" << endl;
        cout << "   5. Koniec hry" << endl;
        cout << endl;

        cout << "Zadajte ciselnu moznost." << endl;
        cin >> postup;

        while (postup != "3" && postup != "5") {
            cout << "Zle zadana moznost. Zadajte znova: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> postup;
        }

        client_socket_write(client, postup);
    } else if (client->data.back() == "hodsestku") {
        cout << "Hodil si šestkou !: " << endl;
        cout << "   3. Posun" << endl;
        cout << "   4. Postav noveho panacika" << endl;
        cout << "   5. Koniec hry" << endl;
        cout << endl;

        cout << "Zadajte ciselnu moznost." << endl;
        cin >> postup;

        while (postup != "3" && postup != "4") {
            cout << "Zle zadana moznost. Zadajte znova: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> postup;
        }

        client_socket_write(client, postup);
    } else {
        cout << client->data.back() << endl;
    }
}

void *spracuj(void *data) {
    auto client = (CLIENT_DATA *)data;
    pthread_mutex_lock(client->mutex);
    while(client->clientSocket->data.back() != "koniec") {
        client_socket_read(client->clientSocket);
        spracuj_pokyn(client->clientSocket);
    }
    pthread_mutex_unlock(client->mutex);
    return nullptr;
}


int main(int argc, char *argv[]) {

    char *hostname = (char *) "frios2.fri.uniza.sk";
    int port = 13000;
    client_socket clientSocket;

    pthread_t hrac;
    pthread_mutex_t mutex;
    pthread_cond_t isCas;

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&isCas, nullptr);

    CLIENT_DATA client = {
            &clientSocket, false, &mutex, &isCas
    };


    while (true) {
        if (client_socket_init(&clientSocket, hostname, port)) {
            client.connected = client_socket_connect(&clientSocket);
            if (client.connected) { break; }
            else {
                cout << endl;
                cout << "Pausing for 5 seconds" << endl;
                this_thread::sleep_for(chrono::seconds(5));

            }
        }
    }

    pthread_create(&hrac, nullptr, spracuj, &client);
    pthread_join(hrac, nullptr);
    client_socket_destroy(client.clientSocket);
    return 0;
}