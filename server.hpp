#ifndef SERVE_HPP
#define SERVE_HPP

#include <cstdio>
#include <mutex>
#include <queue>
#include <stdint.h>
#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock.h>
#endif

#ifdef _LINUX_
#include <socket.h>
#endif

struct NetMsg {
  int sock;
  std::string msg;
  NetMsg(int s, std::string str) : sock{s}, msg{str} { msglen = str.length(); }
  void setLen() { msglen = msg.length(); }
  int getLen() { return msglen; }

private:
  int msglen;
};

class sockserv {

public:
  sockserv(const uint16_t port_number, const char *addr)
      : port{port_number}, ipadd{addr} {}

  fd_set rSet, wSet, eSet; // select call sets

  int uSock;                // socket descriptor
  struct timeval tv;        // time out data
  std::vector<int> clients; // clients list

  bool initop();                             // initialize
  bool sbind();                              // bind socket
  bool slisten(int quelen);                  // listen
  bool blstate(bool st);                     // set blocking state
  bool set_opt(int opt, const char *optval); // set options
  void set_tv(long sec, long usec);          // set timeout

  void zeroSet();
  void ator(int sock);
  void atoe(int sock);
  void atow(int sock);

  void run_server(); // execution
  int performSel();  // select on uSock
  bool newCon();     // accept new client

  NetMsg *rcvMsg();
  void sendMSg(NetMsg *);

private:
  const uint16_t port;      // port number
  const char *ipadd[15];    // ip address
  struct sockaddr_in aSock; // socket addr descriptor

  std::queue<NetMsg> outMsg; // outgoing msg queue
  std::mutex fdGuard;

  // initilizers
#ifdef _WIN32
  WSAData wsd;
  bool init_wsa();
#endif
  bool init_sock();
  void init_addr();
};

#endif
