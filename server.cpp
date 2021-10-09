#include "server.hpp"
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <winsock.h>

using namespace std;

bool sockserv::initop() {
#ifdef _WIN32
  if (!(init_wsa()))
    return false;
#endif
  if (!(init_sock()))
    return false;
  init_addr();
  return true;
}

#ifdef _WIN32
bool sockserv::init_wsa() {
  if ((WSAStartup(MAKEWORD(2, 2), &this->wsd)) < 0)
    return false;
  return true;
}
#endif

bool sockserv::init_sock() {
  if ((uSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    return false;
  return true;
}

void sockserv::init_addr() {
  aSock.sin_family = AF_INET;
  aSock.sin_port = htons(port);
  aSock.sin_addr.s_addr = INADDR_ANY;
  memset(&aSock.sin_zero, 0, 8);
}

bool sockserv::blstate(bool st) {
  unsigned long optval;
  if (st) {
    optval = 1;
  } else {
    optval = 0;
  }
  if (ioctlsocket(uSock, FIONBIO, &optval))
    return false;
  return true;
}

bool sockserv::set_opt(int opt, const char *optval) {
  if (!(setsockopt(uSock, SOL_SOCKET, opt, optval, sizeof(int))))
    return false;
  return true;
}

bool sockserv::sbind() {
  int ret;
  if ((ret = bind(uSock, (sockaddr *)&aSock, sizeof(sockaddr))) < 0) {
    printf("bind exit code :%3d\n", ret);
    return false;
  }
  return true;
}

bool sockserv::slisten(int quelen) {
  if ((listen(uSock, quelen)))
    return false;
  return true;
}

void sockserv::set_tv(long sec, long usec) {
  tv.tv_sec = sec;
  tv.tv_usec = usec;
}

void sockserv::zeroSet() {
  FD_ZERO(&rSet);
  FD_ZERO(&wSet);
  FD_ZERO(&eSet);
}

void sockserv::atoe(int sock) {
  if (eSet.fd_count < 60)
    FD_SET(sock, &eSet);
}

void sockserv::atow(int sock) {
  if (wSet.fd_count < 60)
    FD_SET(sock, &wSet);
}

void sockserv::ator(int sock) {
  if (rSet.fd_count < 60)
    FD_SET(sock, &rSet);
}

int sockserv::performSel() {
  int sel_stat = 0;
  this->zeroSet();
  this->atoe(uSock);
  this->ator(uSock);
  for (auto iSock : clients) {
    this->atoe(iSock);
    this->ator(iSock);
  }
  sel_stat = select(uSock + 1, &rSet, &wSet, &eSet, &tv);
  return sel_stat;
}

bool sockserv::newCon() {
  int tmpSock = 0;
  puts("trying to coonet to new client");
  if ((tmpSock = accept(uSock, NULL, NULL)) <= 0) {
    puts("coulden't accept");
    return false;
  }
  puts("accepted");
  clients.push_back(tmpSock);
  send(tmpSock, "Connected to server", 20, 0);
  return true;
}

NetMsg *sockserv::rcvMsg() {
  // to be implemented
}

void sockserv::sendMSg(NetMsg *msg) {
  // to be implemented
}
