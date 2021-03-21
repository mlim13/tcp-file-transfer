#include "client.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

Client::Client() {

}

void Client::connectToServer(string hostname) {
    struct addrinfo hints;
    struct addrinfo *res, *iterator;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(hostname.c_str(), "4000", &hints, &res)) != 0) {
        std::cout << "Unable to get address information" << "\n";
    }
    // Note: serverInfo could be a linked list with multiple entrie
    // We just assume the first entry is good and use that

    this->sendSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    connect(this->sendSocket, res->ai_addr, res->ai_addrlen);
}

int Client::sendall(char *buf, int len) {
    int total = 0;        // how many bytes we've sent
    int bytesleft = len; // how many we have left to send
    int n;

    while(total < len) {
        n = send(this->sendSocket, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    //*len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 on failure, 0 on success
}

void Client::sendFile(std::string absolute_path) {
    // open file
    ifstream fileToSend;
    fileToSend.open(absolute_path, ios::in); // ios::ate flag sets initial position to end
    int size = 512;
    char memblock[size]; 
    while (fileToSend.good()) {
        fileToSend.read (memblock, size);
        sendall(memblock, size);
    }
    cout << "Send complete\n";
}

Client::~Client() {

}