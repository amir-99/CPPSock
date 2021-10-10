// ClienHandler
//

#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

// import socket option
#ifdef _WIN32
#include <winsock.h>
#endif
#ifdef _LINUX_
#include <sys/socket.h>
#endif

#include <queue>
#include <string>

struct NetMsg {
  // simple message struct

public:
  std::string msg_; // message
  int sockFd_;      // source socket descriptor

  // len_ set and get
  int len() { return len_; }
  void setLen() { len_ = msg_.length(); }

  // constructors
  NetMsg(int sockFd, std::string msg) : sockFd_{sockFd}, msg_{msg} {
    len_ = msg.length();
  }
  NetMsg(int sockFd, char *msg) : sockFd_{sockFd} {
    msg_ = std::string(msg);
    len_ = strlen(msg);
  }
  NetMsg() = delete; // empty init not allowed

private:
  int len_; // message length
};

class ClientHandler {
  // handle each client

public:
  bool healthStat; // health check flag

  // constructors
  ClientHandler(int sockFd, int sec, int usec)
      : sockFd_{sockFd}, healthStat{true} {}
  ClientHandler() = delete;

  void setTimeVal(int sec, int usec); // set tmv
  void runClient();                   // main event loop

private:
  int sockFd_;                         // socket file descriptor
  fd_set readSet, writeSet, exceptSet; // fd set for select
  struct timeval tmv;                  // used for time out
  std::queue<struct NetMsg> msqQ_;     // outgoing messages

  void sendData(struct NetMsg msg); // send data to client
  struct NetMsg recvData();         // recieve data from client
};

#endif
