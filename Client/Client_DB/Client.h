#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
//#include "NetworkConfig.h"


#if defined (_WIN32) || (_WIN64)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined (__linux__)
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#endif

using namespace std;

#define MESSAGE_LENGTH 1024
//#define PORT 7777

class Client
{
public:
#if defined (_WIN32) || (_WIN64)
    WSADATA wsaData;
    SOCKET socket_file_descriptor, connection;
#elif defined (__linux__)
    int socket_file_descriptor, connection;
#endif
    struct sockaddr_in serveraddress, client;

    Client() = default;

    void createSocket();
    void connectionToServer();
    void closeSocket();
    char* readData();
    void writeData(string clientMessage);

    void setIP(const std::string& ip);
    void setPort(int port);

private:
    std::string IP;
    int PORT;
    char message[MESSAGE_LENGTH];
};