// ClienHandler implementation

#include "ClientHandler.hpp"

using namespace std;

void ClientHandler::setTimeVal(int sec, int usec) {
  tmv.tv_sec = sec;
  tmv.tv_usec = usec;
}

void ClientHandler::sendData(struct NetMsg msg) {
  int sent = 0;
  sent = send(sockFd_, msg.msg_.c_str(), msg.len(), 0);
  if (sent > 0)
    printf("sent %3d butes to %d", sent, sockFd_);
  else
    puts("failed to send data");
}

struct NetMsg ClientHandler::recvData() {
  char buff[1024] = {
      0,
  };
  int received = 0, tmpRcv;
  do {
    tmpRcv = recv(sockFd_, buff, 128, 0);
    received += tmpRcv;
  } while (tmpRcv > 0 || received < 1023);
  if (received > 0)
    return NetMsg(sockFd_, buff);
  else
    return NetMsg(sockFd_, "___FAIL_CODE___");
}

void ClientHandler::runClient() {
  int selStat;
  struct NetMsg newMsg(sockFd_, "connected to server");

  // init message
  this->sendData(newMsg);

  while (healthStat) {
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&exceptSet);

    FD_SET(sockFd_, &readSet);
    FD_SET(sockFd_, &writeSet);
    FD_SET(sockFd_, &exceptSet);

    selStat = select(sockFd_ + 1, &readSet, &writeSet, &exceptSet, &tmv);

    if (selStat > 0) {
      // read select
      if (FD_ISSET(sockFd_, &readSet)) {
        printf("New Message from %d;\n");
        newMsg = recvData();
        msqQ_.push(newMsg);
      }
      // exceptio select
      if (FD_ISSET(sockFd_, &exceptSet)) {
        printf("client%5d is out", sockFd_);
        healthStat = false;
      }
    } else if (selStat < 0) {
      puts("something went wrong with scoket");
      healthStat = false;
    }

    if (!msqQ_.empty()) {
      this->sendData(msqQ_.front());
      msqQ_.pop();
    }
  }
}
