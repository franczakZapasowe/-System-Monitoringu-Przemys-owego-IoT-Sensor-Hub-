#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Structures.h"

int main() {

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr<<"Client socket creation failed"<<std::endl;
        return 1;
    }
    std::cout<<"Client socket created"<<std::endl;

    constexpr int PORT = 5000;
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)!=1) {
        std::cerr<<"Invalid server address "<<std::endl;
        return 1;
    }

    if(connect(clientSocket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address))==-1) {
        std::cerr<<"Failed to connect to server"<<std::endl;
        return 1;
    }
    std::cout<<"Connected to server"<<std::endl;


    MachineState myMachine;
    myMachine.m_id = 1;
    myMachine.m_temperature = 12.5f;
    myMachine.m_flag= true;

    send (clientSocket,&myMachine, sizeof(MachineState),0);

    close(clientSocket);
}