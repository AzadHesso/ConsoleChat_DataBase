#pragma once
#include<iostream>
#include<string.h>
#include<vector>

#if defined (_WIN32) || (_WIN64)
#include <winsock2.h>
#include <WS2tcpip.h>
#include <io.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined (__linux__)
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#endif


using namespace std;

#define MESSAGE_LENGTH 1024
#define PORT 7777

class Server
{
public:
#if defined (_WIN32) || (_WIN64)
    WSADATA wsaData;
    SOCKET sockert_file_descriptor, connection;
#elif defined (__linux__)
    int sockert_file_descriptor, connection;
#endif

    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int bind_status, connection_status;

    void createSocket();
    void bindSocket();
    void closeSocket();
    void dataReceivingServer();
    char* readData();
    void writeData(string serverMessage);

private:
    char message[MESSAGE_LENGTH];
    vector<int> client_sockets;
};