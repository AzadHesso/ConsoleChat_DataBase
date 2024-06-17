#pragma once
#include <string>

using namespace std;

class User
{
public:
    User(const string& login, const string& password, const string& name) :
        _login(login),
        _password(password),
        _name(name)
    {}

    User(const string& login, const string& password) :
        _login(login),
        _password(password)
    {}

private:
    string _login;
    string _password;
    string _name;
};