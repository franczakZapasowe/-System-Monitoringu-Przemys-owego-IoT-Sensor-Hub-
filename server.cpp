#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Structures.h"


int main() {

    int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr<<"Swerwer socket creation failed"<<std::endl;
        return 1;
    }

    constexpr int PORT = 5000;
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; //narazie adje tak ale domyslam sie ze bedzie trzeba dac tu ip clienta zeby nie kazde mog do nas wejsc

    auto server_addr = reinterpret_cast<sockaddr *>(&serverAddr);
    if (bind(serverSocket, server_addr, sizeof(serverAddr)) == -1) {
        std::cerr<<"bind failed"<<std::endl;
        return 1;
    }

    if (listen(serverSocket, 10) ==-1) {
        std::cerr<<"Listening failed\n";
        return 1;
    }

    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr<<"accept failed"<<std::endl;
        return 1;
    }

    MachineState machineState{};
    int recive = recv(clientSocket, &machineState, sizeof(MachineState), 0);
    if (recive > 0 ) std::cout<<"Id: "<<machineState.m_id<<" temeprature: "<<machineState.m_temperature<<std::endl;
    close(clientSocket);
    close(serverSocket);

}