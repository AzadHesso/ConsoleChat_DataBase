#pragma once
#include <iostream>
#include <vector>
#include <string.h>

#pragma comment(lib, "libmysql.lib")

#ifdef __linux__
#include <mysql/mysql.h>
#elif defined(_WIN32) || (_WIN64)
#include <mysql.h>
#else 
#error Not implemented!
#endif 

using namespace std;

constexpr auto db_host = "localhost";
constexpr auto db_user = "root";
constexpr auto db_password = "azadhesso";
constexpr auto db_name = "dbase";

class Database {
public:
    MYSQL mysql;
    MYSQL_RES* res;
    MYSQL_ROW row;

    bool connectDatabase(const string& host, const string& user, const string& password, const string& dbName);
    void disconnect();

    bool checkLogin(string login, string password);
    string getName(string login);

    bool checkLoginExists(const string& login);

    bool checkNameExists(const string& name);

    //string checkRegister(string login, string name, string password);
    bool registerUser(string login, string name, string password);

    bool addMessage(string from, string to, string message);
    vector<string> getAllMessages(string login);
    vector<string> getAllUsers();

private:
    vector<string> convertResToVector(MYSQL_RES* res);
};
