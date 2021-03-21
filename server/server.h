#include <vector>
#include <string>
using namespace std;
class Server {
    //private:
    public:
        struct addrinfo *serverInfo;
        int listenSocket;
        vector<int> sockets;
        string getPrintableIP();
        string getPrintablePort();
        string getHostname();
        void loadAddress();
        void createSocket();
        void listenForConnections();
        void receiveall();
    //public:
        Server();
        ~Server();
        void printAddressInfo();
};