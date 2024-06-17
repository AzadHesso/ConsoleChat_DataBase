#include <iostream>
#include "Server.h"
#include "Chat_Server.h"

using namespace std;

int main()
{
    string serverMessage, clientMessage;

    Server server;
    Chat chat;

    server.createSocket();
    server.bindSocket();
    server.dataReceivingServer();

    while (1) {
        clientMessage = server.readData();
        if (clientMessage == "exit") {
            break;
        }
        serverMessage = chat.messageProcessing(clientMessage);
        server.writeData(serverMessage);
    }

    server.closeSocket();

    return 0;
}