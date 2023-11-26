#include "myServer.h"

ServerFramework::ServerFramework() : epollFd_(-1), listenFd_(-1) {}

void ServerFramework::start(int port) 
{
    // 创建日志系统
    Logger logger("server.log");
    logger.log("Server started.");

    // 创建数据库连接
    Database database;
    database.connect();

    // 创建监听套接字
    listenFd_ = createListenSocket(port);
    if (listenFd_ < 0) {
        logger.log("Failed to create listen socket.");
        return;
    }

    // 创建 epoll 实例
    epollFd_ = epoll_create1(0);
    if (epollFd_ == -1) {
        logger.log("Failed to create epoll instance.");
        return;
    }

    // 添加监听套接字到 epoll 中
    if (!addSocketToEpoll(listenFd_, EPOLLIN)) {
        logger.log("Failed to add listen socket to epoll.");
        return;
    }

    // 开始事件循环
    eventLoop(logger, database);
}

int ServerFramework::createListenSocket(int port) {
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) {
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(listenFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(listenFd);
        return -1;
    }

    if (listen(listenFd, SOMAXCONN) < 0) {
        close(listenFd);
        return -1;
    }

    return listenFd;
}

bool ServerFramework::addSocketToEpoll(int socketFd, uint32_t events) {
    epoll_event event{};
    event.events = events;
    event.data.fd = socketFd;

    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, socketFd, &event) == -1) 
    {
        return false;
    }

    return true;
}

void ServerFramework::eventLoop(Logger& logger, Database& database) {
    std::vector<epoll_event> events(10);

    while (true) {
        int numEvents = epoll_wait(epollFd_, events.data(), static_cast<int>(events.size()), -1);
        if (numEvents == -1) 
        {
            logger.log("Error in epoll_wait.");
            break;
        }

        for (int i = 0; i < numEvents; ++i) 
        {
            int fd = events[i].data.fd;
            if (fd == listenFd_) 
            {
                // 监听套接字有新的连接
                acceptConnection(logger, database);
            } 
            else 
            {
                // 客户端可读
                processClientRequest(fd, logger, database);
            }
        }
    }

    close(listenFd_);

    logger.log("Server stopped.");
}

void ServerFramework::acceptConnection(Logger& logger, Database& database) {
    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = accept(listenFd_, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
    if (clientFd < 0) {
        logger.log("Failed to accept new connection.");
        return;
    }

    // 处理新连接
    logger.log("Accepted new connection from " + std::string(inet_ntoa(clientAddr.sin_addr)));

    // 将客户端套接字添加到 epoll 中
    if (!addSocketToEpoll(clientFd, EPOLLIN | EPOLLET)) {
        logger.log("Failed to add client socket to epoll.");
        return;
    }
}

std::string ServerFramework::getRequestType(const std::string& request)
{
    std::istringstream iss(request);
    std::string firstLine;
    std::getline(iss, firstLine);
    std::istringstream iss2(firstLine);
    std::string requestType;
    iss2 >> requestType;
    return requestType;
}
std::string ServerFramework::getHeaderValue(const std::string& request, const std::string& headerName)
{
    std::istringstream iss(request);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find(headerName) == 0) {
            std::istringstream iss2(line);
            std::string header, value;
            iss2 >> header >> value;
            return value;
        }
    }
    return "";
}
std::string ServerFramework::getJsonData(const std::string& request)
{
    size_t jsonStart = request.find("\r\n\r\n") + 4;
    return request.substr(jsonStart);
}
std::string ServerFramework::handleRequest(const std::string& request)
{
    std::cout << "handleRequest\n" << request;
    std::string requestType = getRequestType(request);
    std::string host = getHeaderValue(request, "Host:");
    std::string contentType = getHeaderValue(request, "Content-Type:");
    std::string contentLength = getHeaderValue(request, "Content-Length:");
    std::string jsonData = getJsonData(request);

    std::cout << requestType << " " << host << " " << contentType << " " << contentLength << " " << jsonData;

    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    response += "Request Type: " + requestType + "\n";
    response += "Host: " + host + "\n";
    response += "Content-Type: " + contentType + "\n";
    response += "Content-Length: " + contentLength + "\n";
    response += "JSON Data: " + jsonData + "\n";

    return response;
}

void ServerFramework::processClientRequest(int clientFd, Logger& logger, Database& database) {
    char buffer[1024 * 8];
    ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) 
    {
        // 客户端关闭连接或发生错误
        close(clientFd);
        return;
    }

    buffer[bytesRead] = '\0';

    // 处理客户端请求
    std::string request(buffer);
    logger.log("Received request: " + request);
    const char* comm = "select * from table";
    std::cout << buffer << std::endl;
    if (bytesRead > 0) 
    {
        std::string request(buffer, bytesRead);
        std::string strResponse = R"(
        {
            "name": "John Doe",
            "age": 30,
            "isStudent": false,
            "courses": ["Math", "Science", "History"],
            "address": 
            {
                "street": "123 Main St",
                "city": "Anytown"
            }
        })";
        std::string response = handleRequest(request);
        write(clientFd, strResponse.c_str(), response.length());
    }
    // 执行数据库查询
    /*database.executeQuery(comm);

    // 发送响应给客户端
    std::string response = "Response: " + request;
    write(clientFd, response.c_str(), response.length());*/
}