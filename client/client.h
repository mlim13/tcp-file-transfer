#include <string>

using namespace std;
class Client {
    //private:
    public:
        int sendSocket;
        void connectToServer(string hostname);
        int sendall(char *buf, int len);
    //public:
        Client();
        void sendFile(string absolute_path);
        ~Client();
};