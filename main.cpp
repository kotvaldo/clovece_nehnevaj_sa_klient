#include <iostream>
#include <winsock2.h>


int main() {
    // Inicializácia servera

    // Vytvorenie socketu
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        std::cerr << "Chyba pri inicializácii Winsock\n";
        return -1;
    }

    // Vytvorenie socketu
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Chyba pri vytváraní socketu\n";
        WSACleanup();
        return -1;
    }

    // Nastavenie adresy servera
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Zvolte vhodný port
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Pripojenie socketu k adrese
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Chyba pri viazaní socketu na adresu\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Čakanie na pripojenie hráčov
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Chyba pri čakaní na pripojenie hráčov\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Čakám na hráčov...\n";

    // Akceptovanie pripojenia
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Chyba pri akceptovaní pripojenia\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Hráč pripojený!\n";

    // Spravujte hru a komunikáciu s hráčmi

    // Zatvorenie socketov
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}