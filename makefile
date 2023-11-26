CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11

SRCS = main.cpp Logger.cpp DataBase.cpp myServer.cpp -lmysqlclient
OBJS = $(SRCS:.cpp=.o)
EXEC = myServer

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
