// TCPSercer class

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

// import socket option
#include <_timeval.h>
#ifdef _WIN32
#include <winsock.h>
#endif
#ifdef _LINUX_
#include <sys/socket.h>
#endif

#include <list>
#include <stdint.h>
#include <thread>

#include "ClientHandler.hpp"

class TCPServer {
  // TCP Server

public:
  int sockFd_;                                 // socket file descriptor
  std::list<class ClientHandler> clientsList_; // clients list
  fd_set readSet, writeSet, exceptSet;         // select sets
  bool healthStat_;                            // health check flag

  // constructors
  TCPServer(uint16_t port, const char *ipAddr, int tv[2])
      : port_{port}, ipAddr_{ipAddr}, healthStat_{true} {
    tmv_.tv_sec = tv[0];
    tmv_.tv_usec = tv[1];
  }
  TCPServer() = delete;

  void runServer();

private:
  uint16_t port_;               // port number
  const char *ipAddr_[15];      // ip address
  struct sockaddr_in addrSock_; // socket address descriptor
  struct timeval tmv_;          // used for timeout

#ifdef _WIN32
  WSAData wsd; // Windows socker descriptor
#endif

  bool initOp(); // initialize socket
};

#endif
