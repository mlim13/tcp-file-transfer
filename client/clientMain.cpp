#include "client.h"
#include <iostream>
#include <unistd.h>

using namespace std;
int main(int argc, char *argv[]) {
    Client myClient = Client();
    myClient.connectToServer(string(argv[1]));
    myClient.sendFile("dummy.txt");
    cout << "Client done.\n";
}