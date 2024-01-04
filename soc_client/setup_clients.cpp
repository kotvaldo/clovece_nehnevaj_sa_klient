
#include "../soc_server/passive_socket.h"
#include "../soc_server/active_socket.h"
#include "client_socket.h"


int main() {

    ClientSocket* pClientSocket = ClientSocket::createConnection("localhost", 12345);
    ClientSocket* pClientSocket1 = ClientSocket::createConnection("localhost", 12345);
    ClientSocket* pClientSocket2 = ClientSocket::createConnection("localhost", 12345);


}
