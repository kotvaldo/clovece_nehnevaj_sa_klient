#ifndef PASSIVE_SOCKET_H
#define PASSIVE_SOCKET_H

#include <iostream>
#include <winsock2.h>

class ActiveSocket;

struct PassiveSocket {
    SOCKET socketDescriptor;
    bool isListening;
    bool isWaiting;
    CRITICAL_SECTION mutex{};
    CONDITION_VARIABLE waitingFinished{};


    PassiveSocket();

    ~PassiveSocket();

    bool startListening(short port);

    void stopListening();

    bool isListeningNow();

    bool waitForClient(ActiveSocket* clientSock);
};

#endif // PASSIVE_SOCKET_H