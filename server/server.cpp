#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

Server::Server() {
    this->sockets = vector<int>();
}

/*
Load the IP address and port into the Server instance properties
*/
void Server::loadAddress() {
    struct addrinfo hints;
    struct addrinfo *serverInfo, *iterator;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(NULL, "4000", &hints, &serverInfo)) != 0) {
        std::cout << "Unable to get address information" << "\n";
    }
    for (iterator = serverInfo; iterator != NULL; iterator = iterator->ai_next) {
        this->serverInfo = iterator;
        break;
        // There is probably a nicer way to handle this rather than breaking here
        // We just assume we are only interested in the first entry of the linked list
    }
}

/*
Returns the IP address in printable form (ie. a lovely string)
*/
string Server::getPrintableIP() {
    void *addr;
    char printableAddr[INET6_ADDRSTRLEN];
    if (this->serverInfo->ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)this->serverInfo->ai_addr;
        addr = &(ipv4->sin_addr);
    } else {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)this->serverInfo->ai_addr;
        addr = &(ipv6->sin6_addr);
    }
    inet_ntop(this->serverInfo->ai_family, addr, printableAddr, INET6_ADDRSTRLEN);
    return string(printableAddr);
}

/*
Get printable port
*/
string Server::getPrintablePort() {
    uint16_t addr;
    if (this->serverInfo->ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)this->serverInfo->ai_addr;
        addr = (ipv4->sin_port);
    } else {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)this->serverInfo->ai_addr;
        addr = (ipv6->sin6_port);
    }
    //inet_ntop(this->serverInfo->ai_family, addr, printableAddr, INET6_ADDRSTRLEN);
    return to_string(ntohs(addr));
}

/*
Get host name of the server
*/
string Server::getHostname() {
    char hostname[100];
    gethostname(hostname, 100);
    return string(hostname);
}

/*
Creates socket and binds to it.
Socket file descriptor stored as the instance variable listenSocket.
*/
void Server::createSocket() {
    // Build the socket
    int fd = socket(this->serverInfo->ai_family, this->serverInfo->ai_socktype, this->serverInfo->ai_protocol);
    if (fd < 0) {
        cout << "Error creating socket\n";
    }
    // Bind the socket
    if (bind(fd, this->serverInfo->ai_addr, this->serverInfo->ai_addrlen) < 0) {
        cout << "Error binding to socket\n";
    }
    this->listenSocket = fd;
}

/*
Listen to any incoming connections and delegate work 
*/
void Server::listenForConnections() {
    cout << "starting listen\n";
    listen(this->listenSocket, 5);
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    int newFd;
    newFd = accept(this->listenSocket, (struct sockaddr *)&their_addr, &addr_size);
    this->sockets.push_back(newFd);
    cout << "accepted\n";
}

/*
Receive all data (typically a file)
*/
void Server::receiveall() {
    // just pick first item in vector for now - will change
    int recvSocket = this->sockets[0];
    char buffer[512];
    recv(recvSocket, buffer, 512, 0);
    cout << "Received data is: " << string(buffer) << "\n";
}

Server::~Server() {

}