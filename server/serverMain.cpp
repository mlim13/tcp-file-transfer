#include "server.h"
#include <iostream>
#include <unistd.h>

using namespace std;
int main() {
    Server myServer = Server();
    myServer.loadAddress();
    cout << myServer.getHostname() << "\n";
    myServer.createSocket();
    myServer.listenForConnections();
    
    //cout << myServer.getPrintableIP() << "\n";
    //cout << myServer.getPrintablePort() << "\n";

    myServer.receiveall();
    cout << "Server done.\n";
}