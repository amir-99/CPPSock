#include "server.hpp"
#include <winsock.h>

using namespace std;

int main() {

  const char *addr = "127.0.0.1";
  const uint16_t port = 4580;

  // def socket
  sockserv mSrv(port, addr);

  // init socket
  if (!mSrv.initop()) {
    puts("failed to init");
#ifdef _WIN32
    WSACleanup();
#endif
    exit(EXIT_FAILURE);
  }

  // set non-blocking
  if (!mSrv.blstate(false)) {
    puts("failed to set non-blocking state");
#ifdef _WIN32
    WSACleanup();
#endif
    exit(EXIT_FAILURE);
  }

  // set reuseable option
  if (!mSrv.set_opt(SO_REUSEADDR, (const char *)0)) {
    puts("failed to set opt");
#ifdef _WIN32
    WSACleanup();
#endif
    exit(EXIT_FAILURE);
  }

  // set timeout opt
  mSrv.set_tv(10, 0);

  // bind socket
  if (!mSrv.sbind()) {
    puts("failed to bind");
#ifdef _WIN32
    WSACleanup();
#endif
    exit(EXIT_FAILURE);
  }

  // listen
  if (!mSrv.slisten(5)) {
    puts("failed to bind");
#ifdef _WIN32
    WSACleanup();
#endif
    exit(EXIT_FAILURE);
  }

  int sel_stat = 0;
  bool clStat;
  puts("waiting ...");
  while (true) {
    sel_stat = mSrv.performSel();
    if (sel_stat > 0) {
      // new clients
      if (FD_ISSET(mSrv.uSock, &mSrv.rSet)) {
        puts("incomming connection ...");
        sel_stat = mSrv.newCon();
        if (!sel_stat) {
          puts("something went wrong with accept proccess");
          int err = WSAGetLastError();
          printf("error code:%d\n", err);
#ifdef _WIN32
          WSACleanup();
#endif
          exit(EXIT_FAILURE);
        }
      } else if (FD_ISSET(mSrv.uSock, &mSrv.eSet)) {
        puts("Socket Failure");
#ifdef _WIN32
        WSACleanup();
#endif
        exit(EXIT_FAILURE);
      }
    }
    if (sel_stat == 0)
      puts("no req");
  }
  return 0;
}
