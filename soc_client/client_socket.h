#ifndef SOCKETS_CLIENT_MY_SOCKET_H
#define SOCKETS_CLIENT_MY_SOCKET_H

#include <winsock2.h>
#include <string>

class ClientSocket {
public:
    static ClientSocket* createConnection(std::string hostName, long port);
    ~ClientSocket();
    static void connectionOutput(int i);
protected:
    explicit ClientSocket(SOCKET socket);
private:
    static const char * endMessage;
    SOCKET connectSocket;


};

#endif //SOCKETS_CLIENT_MY_SOCKET_H
