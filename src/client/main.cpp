// Adiciona todos lib padrão do C++
// https://gist.github.com/eduarc/6022859
#include <bits/stdc++.h>

#include "common/utils.h"

#include <sys/socket.h>

#define PORT 8080

class Client {
private:
  std::string host;
  int port;

  int sock;
public:
    Client(std::string host, int port){

        if ((sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            std::cerr << FAIL << "Error in trying to opening socket" << ENDC <<std::endl;
            exit(1);
        }
    };

    ~Client(){
        std::cout << WARNING << "Client died" << ENDC << std::endl;
    };

    void start();

};

void Client::start() {
    
}

int main(int argc, char const *argv[])
{
    std::vector<std::string> tokens;
    if (argc < 2)
    {
        std::cerr << FAIL << "Wrong number of argments" << std::endl;
        std::cerr << "\tUSAGE: ./clientTP3 <IP:port>" << ENDC << std::endl;
        exit(1);
    }
    else
    {
        tokens = utils::split_string(argv[1], ':');
    }

    // Create socket
    Client client(tokens[0], stoi(tokens[1]));

    client.start();

    return 0;
}