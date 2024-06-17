#include "Database.h"
#include <iostream>
#include <string>

bool Database::connectDatabase(const string& host, const string& user, const string& password, const string& dbName) {
    mysql_init(&mysql);

    if (&mysql == nullptr) {
        std::cerr << "Error: can't create MySQL-descriptor" << std::endl;
        exit(1);
    }

    if (!mysql_real_connect(&mysql, host.c_str(), user.c_str(), password.c_str(), dbName.c_str(), 0, NULL, 0)) {
        std::cerr << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;
        return false;
    }

    std::cout << "Success!" << std::endl;
    return true;
}

void Database::disconnect() {
    mysql_close(&mysql);
}

bool Database::checkLogin(string login, string password) {
    string query = "SELECT passwords.password_hash FROM users "
        "JOIN passwords ON users.user_id = passwords.user_id "
        "WHERE users.login = '" + login + "'";

    if (mysql_query(&mysql, query.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (mysql_fetch_row(res) != nullptr) {
            mysql_free_result(res);
            return true;
        }
        mysql_free_result(res);
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return false;
}

string Database::getName(string login) {
    string query = "SELECT name FROM users WHERE login = '" + login + "'";

    if (mysql_query(&mysql, query.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        string name = row ? row[0] : "";
        mysql_free_result(res);
        return name;
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return "";
}

bool Database::checkLoginExists(const string& login) {
    string query = "SELECT login FROM users WHERE login = '" + login + "'";

    if (mysql_query(&mysql, query.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        bool exists = mysql_fetch_row(res) != nullptr;
        mysql_free_result(res);
        return exists;
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return false;
}

bool Database::checkNameExists(const string& name) {
    string query = "SELECT name FROM users WHERE name = '" + name + "'";

    if (mysql_query(&mysql, query.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        bool exists = mysql_fetch_row(res) != nullptr;
        mysql_free_result(res);
        return exists;
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return false;
}

bool Database::registerUser(string login, string name, string password) {
    string registerUserQuery = "INSERT INTO users (name, login) VALUES ('" + name + "', '" + login + "')";
    string addUserPasswordQuery = "INSERT INTO passwords (user_id, password_hash) VALUES (LAST_INSERT_ID(), '" + password + "')";

    if (mysql_query(&mysql, registerUserQuery.c_str()) == 0 && mysql_query(&mysql, addUserPasswordQuery.c_str()) == 0) {
        return true;
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return false;
}

bool Database::addMessage(string from, string to, string message) {
    string getFromId = "SELECT user_id FROM users WHERE name = '" + from + "'";
    if (mysql_query(&mysql, getFromId.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        int fromId = atoi(row[0]);
        mysql_free_result(res);

        string getToId = "SELECT user_id FROM users WHERE name = '" + to + "'";
        if (mysql_query(&mysql, getToId.c_str()) == 0) {
            res = mysql_store_result(&mysql);
            row = mysql_fetch_row(res);
            int toId = atoi(row[0]);
            mysql_free_result(res);

            string addMsg = "INSERT INTO messages (from_id, to_id, message) VALUES (" + std::to_string(fromId) + ", " + std::to_string(toId) + ", '" + message + "')";
            if (mysql_query(&mysql, addMsg.c_str()) == 0) {
                return true;
            }
            else {
                std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
            }
        }
        else {
            std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
        }
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return false;
}

//bool Database::addMessage(string from, string to, string message) {
//    if (from.empty()) {
//        std::cerr << "Error: 'from' user is empty." << std::endl;
//        return false;
//    }
//
//    std::cout << "Attempting to add message from: " << from << " to: " << to << std::endl;
//
//    // Fetch 'from' user ID
//    string getFromId = "SELECT user_id FROM users WHERE name = '" + from + "'";
//    if (mysql_query(&mysql, getFromId.c_str()) != 0) {
//        std::cerr << "MySQL query error for getFromId: " << mysql_error(&mysql) << std::endl;
//        return false;
//    }
//    res = mysql_store_result(&mysql);
//    if (res == nullptr) {
//        std::cerr << "MySQL store result error for getFromId: " << mysql_error(&mysql) << std::endl;
//        return false;
//    }
//    row = mysql_fetch_row(res);
//    if (row == nullptr) {
//        std::cerr << "Error: No user found with name '" << from << "'" << std::endl;
//        mysql_free_result(res);
//        return false;
//    }
//    int fromId = atoi(row[0]);
//    mysql_free_result(res);
//
//    // Fetch 'to' user ID
//    string getToId = "SELECT user_id FROM users WHERE name = '" + to + "'";
//    if (mysql_query(&mysql, getToId.c_str()) != 0) {
//        std::cerr << "MySQL query error for getToId: " << mysql_error(&mysql) << std::endl;
//        return false;
//    }
//    res = mysql_store_result(&mysql);
//    if (res == nullptr) {
//        std::cerr << "MySQL store result error for getToId: " << mysql_error(&mysql) << std::endl;
//        return false;
//    }
//    row = mysql_fetch_row(res);
//    if (row == nullptr) {
//        std::cerr << "Error: No user found with name '" << to << "'" << std::endl;
//        mysql_free_result(res);
//        return false;
//    }
//    int toId = atoi(row[0]);
//    mysql_free_result(res);
//
//    // Using parameterized queries to prevent SQL injection
//    MYSQL_STMT* stmt;
//    MYSQL_BIND bind[3];
//
//    stmt = mysql_stmt_init(&mysql);
//    if (!stmt) {
//        std::cerr << "Could not initialize statement handler" << std::endl;
//        return false;
//    }
//
//    string addMsg = "INSERT INTO messages (from_id, to_id, message) VALUES (?, ?, ?)";
//    if (mysql_stmt_prepare(stmt, addMsg.c_str(), addMsg.length()) != 0) {
//        std::cerr << "Could not prepare statement: " << mysql_stmt_error(stmt) << std::endl;
//        mysql_stmt_close(stmt);
//        return false;
//    }
//
//    memset(bind, 0, sizeof(bind));
//
//    bind[0].buffer_type = MYSQL_TYPE_LONG;
//    bind[0].buffer = &fromId;
//    bind[0].is_null = 0;
//    bind[0].length = 0;
//
//    bind[1].buffer_type = MYSQL_TYPE_LONG;
//    bind[1].buffer = &toId;
//    bind[1].is_null = 0;
//    bind[1].length = 0;
//
//    bind[2].buffer_type = MYSQL_TYPE_STRING;
//    bind[2].buffer = (char*)message.c_str();
//    bind[2].buffer_length = message.length();
//    bind[2].is_null = 0;
//    bind[2].length = 0;
//
//    if (mysql_stmt_bind_param(stmt, bind) != 0) {
//        std::cerr << "Could not bind parameters: " << mysql_stmt_error(stmt) << std::endl;
//        mysql_stmt_close(stmt);
//        return false;
//    }
//
//    if (mysql_stmt_execute(stmt) != 0) {
//        std::cerr << "Could not execute statement: " << mysql_stmt_error(stmt) << std::endl;
//        mysql_stmt_close(stmt);
//        return false;
//    }
//
//    mysql_stmt_close(stmt);
//    std::cout << "Message successfully added." << std::endl;
//    return true;
//}


vector<string> Database::getAllMessages(string name) {
    string getAllMessagesQuery = "SELECT u1.name AS from_name, u2.name AS to_name, m.message "
        "FROM messages m "
        "JOIN users u1 ON u1.user_id = m.from_id "
        "JOIN users u2 ON u2.user_id = m.to_id "
        "WHERE u1.name = '" + name + "' OR u2.name = '" + name + "'";

    vector<string> resultVector;

    if (mysql_query(&mysql, getAllMessagesQuery.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        resultVector = convertResToVector(res);
        mysql_free_result(res);
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return resultVector;
}

vector<string> Database::getAllUsers() {
    string getAllUsersQuery = "SELECT name FROM users";
    vector<string> resultVector;

    if (mysql_query(&mysql, getAllUsersQuery.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        resultVector = convertResToVector(res);
        mysql_free_result(res);
    }
    else {
        std::cerr << "MySQL query error: " << mysql_error(&mysql) << std::endl;
    }

    return resultVector;
}

vector<string> Database::convertResToVector(MYSQL_RES* res) {
    vector<string> resultVector;

    while ((row = mysql_fetch_row(res)) != nullptr) {
        for (int i = 0; i < mysql_num_fields(res); i++) {
            if (row[i] != nullptr) {
                resultVector.push_back(string(row[i]));
            }
        }
    }

    return resultVector;
}
