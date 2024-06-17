#include <iostream>
#include "Sha1.h"
#include <string>
#include <cstring>
#include "Chat.h"


using namespace std;

string Chat::chatProcessing()
{
    string outMessage;

    while (!_sendMessage) {
        if (!_isCurrentUser) {
            if (_currentMenu == "") {
                showLoginMenu();
            }
            else if (_currentMenu == "loginUser") {
                outMessage = loginUser();
            }
            else if (_currentMenu == "registerUser") {
                outMessage = registerUser();
            }
            else if (_currentMenu == "exitChat") {
                outMessage = exitChat();
            }
        }
        else {
            if (_currentMenu == "") {
                showUserMenu();
            }
            else if (_currentMenu == "getChat") {
                outMessage = getChat();
            }
            else if (_currentMenu == "addMessage") {
                outMessage = addMessage();
            }
            else if (_currentMenu == "getUsers") {
                outMessage = getUsers();
            }
        }
    }

    return outMessage;
}

void Chat::messageProcessing(string serverMessage)
{
    _vectorFromMessage = messageToVector(serverMessage, ":");

    if (_vectorFromMessage[0] == "loginUser") {
        checkLogin();
    }
    else if (_vectorFromMessage[0] == "registerUser") {
        checkRegister();
    }
    else if (_vectorFromMessage[0] == "showChat") {
        showChat();
    }
    else if (_vectorFromMessage[0] == "addMessage") {
        checkAddMessage();
    }
    else if (_vectorFromMessage[0] == "showUsers") {
        showUsers();
    }
}

vector<string> Chat::messageToVector(string message, string delimiter)
{
    vector<string> resultVector;

    if (!message.empty()) {
        int start = 0;
        do {
            int index = message.find(delimiter, start);
            if (index == string::npos) {
                break;
            }

            int length = index - start;
            resultVector.push_back(message.substr(start, length));
            start += (length + delimiter.size());
        } while (true);

        resultVector.push_back(message.substr(start));
    }

    return resultVector;
}

string Chat::loginUser()
{
    string login, password, outMessage;
    _sendMessage = true;

    cout << "Login: ";
    cin >> login;
    cout << "Password: ";
    cin >> password;

    const char* passwordChar = password.c_str();

    uint* passHex = sha1(passwordChar, strlen(passwordChar));

    outMessage = "loginUser:" + login + ":" + to_string(*passHex);

    return outMessage;
}

void Chat::checkLogin()
{
    string checkUser;
    checkUser = _vectorFromMessage[1];
    _sendMessage = false;

    if (checkUser == "error") {
        _isCurrentUser = false;
        cerr << "Invalid password or invalid login!" << endl;
        showLoginMenu();
    }
    else if (checkUser == "ok") {
        _isCurrentUser = true;
        _currentMenu = "";
        _currentUserName = _vectorFromMessage[2];
    }
}

string Chat::registerUser()
{
    string login, password, name, outMessage;
    _sendMessage = true;

    cout << "Login: ";
    do {
        cin >> login;
    } while (login == "all");
    cout << "Password: ";
    cin >> password;
    cout << "Name: ";
    do {
        cin >> name;
    } while (login == "all");

    const char* passwordChar = password.c_str();

    uint* passHex = sha1(passwordChar, strlen(passwordChar));

    outMessage = "registerUser:" + login + ":" + to_string(*passHex) + ":" + name;

    return outMessage;
}

void Chat::checkRegister()
{
    string checkUser, isNameOrLogin;
    checkUser = _vectorFromMessage[1];
    isNameOrLogin = _vectorFromMessage[2];
    _sendMessage = false;

    if (checkUser == "error") {
        _isCurrentUser = false;
        if (isNameOrLogin == "login") {
            cerr << "Login is busy!" << endl;
        }
        else if (isNameOrLogin == "name") {
            cerr << "Name is busy!" << endl;
        }
        showLoginMenu();
    }
    else if (checkUser == "ok") {
        _isCurrentUser = true;
        _currentMenu = "";
        _currentUserName = _vectorFromMessage[2];
    }
}

string Chat::exitChat()
{
    _sendMessage = true;
    cout << "Exiting the chat" << endl;
    return "exit";
}

void Chat::startMessage()
{
    cout << "   Welcome to the chat!" << endl;
}

void Chat::showLoginMenu()
{
    string outMessage;
    char operation;
    _sendMessage = false;

    cout << "Press (1) to Login or (2) to Register or (3) to Exit " << endl;
    cin >> operation;

    switch (operation)
    {
    case '1':
        _currentMenu = "loginUser";
        break;

    case '2':
        _currentMenu = "registerUser";
        break;

    case '3':
        _currentMenu = "exitChat";
        break;

    default:
        cerr << "Mistyped error.." << endl;
        _currentMenu = "";
        break;
    }
}

void Chat::showUserMenu()
{
    string outMessage;
    char operation;
    _sendMessage = false;

    cout << "Press (1) to Show chat or (2) to Add message or (3) to Show Users or (4) to exit " << endl;
    cin >> operation;

    switch (operation)
    {
    case '1':
        _currentMenu = "getChat";
        break;

    case '2':
        _currentMenu = "addMessage";
        break;

    case '3':
        _currentMenu = "getUsers";
        break;

    case '4':
        _isCurrentUser = false;
        _currentMenu = "";
        break;

    default:
        cerr << "Mistyped error.." << endl;
        _currentMenu = "";
        break;
    }
}

string Chat::addMessage()
{
    string from, to, text, outMessage;
    _sendMessage = true;

    // Установим значение from из текущего имени пользователя
    from = _currentUserName;

    cout << "Send Message to (name) or to (all): ";
    cin >> to;
    cout << "Text: ";
    cin.ignore();
    getline(cin, text);

    outMessage = "addMessage:" + from + ":" + to + ":" + text;

    return outMessage;
}


void Chat::checkAddMessage()
{
    _sendMessage = false;
    string checkMessage;
    checkMessage = _vectorFromMessage[1];

    if (checkMessage == "error") {
        _currentMenu = "";
        cerr << "Failed to send message to user!";
        cout << _vectorFromMessage[2] << "!!" << endl;
    }
    else if (checkMessage == "ok") {
        _currentMenu = "";
    }
}

string Chat::getUsers()
{
    string outMessage;
    _sendMessage = true;

    outMessage = "showUsers:" + _currentUserName;

    return outMessage;
}

string Chat::getChat()
{
    string outMessage;
    _sendMessage = true;

    outMessage = "showChat:" + _currentUserName + ":showChat";

    return outMessage;
}

void Chat::showChat()
{
    string from, to;
    _sendMessage = false;
    cout << "   Chat" << endl;
    if (_vectorFromMessage[1] == "empty") {
        cout << "empty" << endl;
    }
    else {
        int i;
        for (i = 1; i < _vectorFromMessage.size() - 1; i += 3) {
            cout << "Message from " << _vectorFromMessage[i];
            cout << " to " << _vectorFromMessage[i + 1] << endl;
            cout << "text: " << _vectorFromMessage[i + 2] << endl;
        }
    }

    _currentMenu = "";
}

void Chat::showUsers()
{
    _sendMessage = false;
    cout << "   All users" << endl;

    int i;
    for (i = 1; i < _vectorFromMessage.size() - 1; i++) {
        cout << _vectorFromMessage[i] << endl;

    }
    _currentMenu = "";
}