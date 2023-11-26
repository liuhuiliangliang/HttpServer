#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <mysql/mysql.h>

class Database 
{
public:
    Database();
    ~Database();
    void init();
    void connect();
    void disconnect();
    void executeQuery(const char* query);
    void insert(const char* query);
    void del(const char* query);
    void update(const char* query);
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char *server = "localhost";
    const char *user = "debian-sys-maint";
    const char *password = "mKvYl9M6tm1JAfw6";
    const char *database = "MyServerDB";
};

#endif  // DATABASE_H
