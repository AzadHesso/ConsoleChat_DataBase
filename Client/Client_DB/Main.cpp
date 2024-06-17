#include <iostream>
#include "Client.h"
#include "Chat.h"
#include "NetworkConfig.h"

using namespace std;

int main()
{
	string ip;
	int port;

	loadConfigFromFile("network.confiq", ip, port);

	string clientMessage, serverMessage;

	Client client;
	client.setIP(ip);
	client.setPort(port);

	Chat chat;

	client.createSocket();
	client.connectionToServer();

	chat.startMessage();

	while (1) {
		clientMessage = chat.chatProcessing();
		client.writeData(clientMessage);
		if (clientMessage == "exit") {
			break;
		}
		serverMessage = client.readData();
		chat.messageProcessing(serverMessage);
	}

	client.closeSocket();

	return 0;
}