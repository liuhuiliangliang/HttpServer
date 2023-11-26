#ifndef SERVER_FRAMEWORK_H
#define SERVER_FRAMEWORK_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <string>

#include "Logger.h"
#include "DataBase.h"

class ServerFramework {
public:
    ServerFramework();
    void start(int port);

private:
    //创建Socket
    int createListenSocket(int port);
    //添加监听套接字到 epoll 中
    bool addSocketToEpoll(int socketFd, uint32_t events);
    void eventLoop(Logger& logger, Database& database);
    void acceptConnection(Logger& logger, Database& database);
    //处理客户端请求
    void processClientRequest(int clientFd, Logger& logger, Database& database);
    //获取请求类型
    std::string getRequestType(const std::string& request);
    //获取指定名称的头部字段值
    std::string getHeaderValue(const std::string& request, const std::string& headerName);
    //获取JSON数据部分
    std::string getJsonData(const std::string& request);
    //处理HTTP请求，生成响应数据
    std::string handleRequest(const std::string& request);

private:
    int epollFd_;
    int listenFd_;
};

#endif  // SERVER_FRAMEWORK_H