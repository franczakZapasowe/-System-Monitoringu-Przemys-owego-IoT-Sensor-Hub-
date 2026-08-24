#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Structures.h"
#include <thread>

void handleClient(int clientSocket) {
    MachineState machineState{};
    ServerCommand comand{};
    comand.m_code=true;
    while (true) {
        int recive = recv(clientSocket, &machineState, sizeof(MachineState), 0);
        if (recive > 0 ) std::cout<<"Id: "<<machineState.m_id<<" temeprature: "<<machineState.m_temperature<<std::endl;
        else if (recive <= 0) break;
        if (machineState.m_temperature > 95) {
            comand.m_code=false;
            std::cout<<"Alarm\n";
        }
        send(clientSocket, &comand, sizeof(ServerCommand), 0);
    }
    close(clientSocket);
}

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
    while (true) {
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr<<"accept failed"<<std::endl;
            return 1;
        }

        std::thread t (handleClient, clientSocket);
        t.detach();
        std::cout<<"NumerWatku: "<<t.get_id()<<std::endl;
    }
}