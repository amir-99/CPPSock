// TCPSercer Class implementation

#include "TCPServer.hpp"

using namespace std;

bool TCPServer::initOp() {

#ifdef _WIN32
  // WSA
  if ((WSAStartup(MAKEWORD(2, 2), &this->wsd)) < 0) {
    puts("WSA Strat up failed");
    return false;
  }
#endif

  // Socket
  if ((sockFd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    puts("Socket init failed");
    return false;
  }

  // Address
  addrSock_.sin_family = AF_INET;
  addrSock_.sin_port = htons(port_);
  addrSock_.sin_addr.s_addr = inet_addr((const char *)ipAddr_);
  memset(&addrSock_.sin_zero, 0, 8);

  // set non blocking
  unsigned long optval = 1;
  if (ioctlsocket(sockFd_, FIONBIO, &optval))
    puts("setting non blocking failed");

  // set reuseable
  if (!(setsockopt(sockFd_, SOL_SOCKET, SO_REUSEADDR, (const char *)0,
                   sizeof(int))))
    puts("setting reusable opt failed!");

  return true;
}

void TCPServer::runServer() {

  // initialize
  this->initOp();

  // bind
  if ((bind(sockFd_, (sockaddr *)&addrSock_, sizeof(sockaddr))) < 0) {
    puts("bind failed");
    exit(EXIT_FAILURE);
  }

  // listen
  if ((listen(sockFd_, 5)) < 0) {
    puts("Failed to start listn proc");
    exit(EXIT_FAILURE);
  }

  // running log
  puts("setver running ...");

  // event loop
  int respone = 0, tmpSock = 0;
  while (healthStat_) {

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&exceptSet);

    FD_SET(sockFd_, &readSet);
    FD_SET(sockFd_, &writeSet);

    respone = select(sockFd_ + 1, &readSet, &writeSet, &exceptSet, &tmv_);

    // normal event on socket
    if (respone > 0) {

      // read op
      if (FD_ISSET(sockFd_, &readSet)) {
        puts("incoming connection");
        tmpSock = accept(sockFd_, NULL, NULL);
        if (tmpSock > 0) {
          clientsList_.push_front(ClientHandler(tmpSock, 2, 0));
          thread(&ClientHandler::runClient, clientsList_.front()).detach();
        } else
          puts("couldn't accept new connection");
      }

      // execution op
      if (FD_ISSET(sockFd_, &exceptSet)) {
        puts("Socket corrupted!");
        healthStat_ = false;
      }
    } else if (respone < 0) {
      puts("Socket corrupted!");
      healthStat_ = false;
    }
  }
}
