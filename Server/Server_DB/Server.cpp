#include "Server.h"

void Server::createSocket()
{
#if defined (_WIN32) || (_WIN64)
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Winsock initialization error!" << endl;
        exit(1);
    }

    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == INVALID_SOCKET) {
        cerr << "Creation of Socket failed!" << endl;
        exit(1);
    }
    else {
        cout << "Socket creation was successful.!" << endl;
    }
#elif defined (__linux__)
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        cerr << "Creation of Socket failed!" << endl;
        exit(1);
    }
    else {
        cout << "Socket creation was successful.!" << endl;
    }
#endif
}

void Server::bindSocket()
{
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) {
        cerr << "Socket binding failed.!" << endl;
        exit(1);
    }
    else {
        cout << "The socket binding was successful!" << endl;
    }
}

void Server::closeSocket()
{
#if defined (_WIN32) || (_WIN64)
    closesocket(sockert_file_descriptor);
    WSACleanup();
#elif defined (__linux__)
    close(sockert_file_descriptor);
#endif 
}

void Server::dataReceivingServer()
{
    connection_status = listen(sockert_file_descriptor, 5);

    if (connection_status == -1) {
        cerr << "Socket is unable to listen for new connections.!" << endl;
        exit(1);
    }
    else {
        cout << "Server is listening for new connection: " << endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);

    if (connection == -1) {
        cerr << "Server is unable to accept the data from client.!" << endl;
        exit(1);
    }
    else {
        cout << "The client has joined!" << endl;
    }
}


char* Server::readData()
{

#if defined (_WIN32) || (_WIN64)
    memset(message, '\0', MESSAGE_LENGTH);

    recv(connection, message, sizeof(message), 0);
#elif defined (__linux__)
    bzero(message, MESSAGE_LENGTH);

    read(connection, message, sizeof(message));
#endif

    cout << "Data received from client: " << message << endl;
    return message;
}

void Server::writeData(string serverMessage)
{
#if defined (_WIN32) || (_WIN64)
    memset(message, '\0', MESSAGE_LENGTH);
    strcpy_s(message, serverMessage.c_str());

    int bytesSent = send(connection, message, sizeof(message), 0);

    if (bytesSent >= 0) {
        cout << "Data successfully sent to the client.!" << message << endl;
    }
#elif defined (__linux__)
    bzero(message, MESSAGE_LENGTH);
    strcpy(message, serverMessage.c_str());

    ssize_t bytes = write(connection, message, sizeof(message));

    if (bytes >= 0) {
        cout << "Data successfully sent to the client.!" << message << endl;
    }
#endif
}