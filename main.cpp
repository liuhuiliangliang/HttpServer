#include <iostream>
#include "myServer.h"
int main() 
{
    /*
    string user = "debian-sys-maint";
    string passwd = "mKvYl9M6tm1JAfw6";
    string databasename = "MyServerDB";
    */
    ServerFramework server;
    server.start(8001);
    return 0;
}