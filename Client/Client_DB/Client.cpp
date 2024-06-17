#include "Client.h"

using namespace std;

void Client::setIP(const std::string& ip)
{
    IP = ip;
}

void Client::setPort(int port)
{
    PORT = port;
}

void Client::createSocket()
{
#if defined (_WIN32) || (_WIN64)
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "Failed to initialize Winsock!" << endl;
        exit(1);
    }

    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == INVALID_SOCKET)
    {
        cerr << "Creation of Socket failed!" << endl;
        exit(1);
    }
#elif defined (__linux__)
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        cerr << "Creation of Socket failed!" << endl;
        exit(1);
    }
#endif
}

void Client::connectionToServer() {
    serveraddress.sin_addr.s_addr = inet_addr(IP.c_str());
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

#if defined (_WIN32) || (_WIN64) 
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

    if (connection == SOCKET_ERROR) {
        cerr << "Connection with the server failed.!" << endl;
        exit(1);
    }
#elif defined (__linux__)
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

    if (connection == -1) {
        cerr << "Connection with the server failed.!" << endl;
        exit(1);
    }
#endif
}

void Client::closeSocket()
{
#if defined (_WIN32) || (_WIN64)
    closesocket(socket_file_descriptor);
    WSACleanup();
#elif defined (__linux__)
    close(socket_file_descriptor);
#endif
}

char* Client::readData()
{
#if defined (_WIN32) || (_WIN64)
    memset(message, '\0', MESSAGE_LENGTH);
    recv(socket_file_descriptor, message, sizeof(message), 0);
#elif defined (__linux__)
    bzero(message, sizeof(message));
    read(socket_file_descriptor, message, sizeof(message));
#endif

    return message;
}

void Client::writeData(string clientMessage)
{
#if defined (_WIN32) || (_WIN64)
    memset(message, '\0', MESSAGE_LENGTH);
    strcpy_s(message, clientMessage.c_str());

    send(socket_file_descriptor, message, sizeof(message), 0);
#elif defined (__linux__)
    bzero(message, sizeof(message));
    strcpy(message, clientMessage.c_str());

    write(socket_file_descriptor, message, sizeof(message));
#endif
}


