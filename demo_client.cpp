#include <cstdio>
#include <psdk_inc/_ip_types.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define PORT 4580
#define MY_IP "127.0.0.1"

using namespace std;

int main() {
  int mySocket, sockBd;
  WSADATA ws;
  struct sockaddr_in mySockAddr;
  struct timeval tv_sock;
  /* fd_set read_set, write_set, exception_set; */

  // WSA int
  if ((WSAStartup(MAKEWORD(2, 2), &ws)) < 0) {
    perror("WSA not initialized");
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  // Socket init
  if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("Socket not initialized");
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  // addr conf
  mySockAddr.sin_family = AF_INET;
  mySockAddr.sin_port = htons(PORT);
  mySockAddr.sin_addr.s_addr = inet_addr(MY_IP);
  // mySockAddr.sin_addr.s_addr = INADDR_ANY;
  memset(&mySockAddr.sin_zero, 0, 8);

  int stat = 0;
  stat =
      connect(mySocket, (const sockaddr *)&mySockAddr, (int)sizeof(sockaddr));
  if (stat < 0) {
    puts("Failed to connect");
    exit(EXIT_FAILURE);
  }

  char buff[255] = {
      0,
  };
  puts("trying to recvive data");
  recv(mySocket, buff, 255, 0);
  puts(buff);

  /* const char *msg = "hello world!"; */
  /* const int len = strlen(msg); */
  /* char msgLen[16]; */
  /* sprintf(msgLen, "__len__%8d", len); */
  /* send(mySocket, msgLen, strlen(msgLen), 0); */
  /* puts("sent len"); */
  /* send(mySocket, msg, strlen(msg), 0); */
  /* puts("sent msg"); */

  /* char buff[255]; */
  /* puts("waiting for reply"); */
  /* stat = recv(mySocket, buff, 255, 0); */
  /* puts(buff); */

  return 0;
}
