// exec

#include "TCPServer.hpp"

int main() {

  int time[2] = {2, 0};
  TCPServer myServer(4580, "127.0.0.1", time);
  myServer.runServer();

  return 0;
}
