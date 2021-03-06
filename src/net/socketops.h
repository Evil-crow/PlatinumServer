/**
 * Created by Crow on 11/21/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe socket APIs,
 *         They're in namespace socket, not Class Socket
 */

#ifndef PLATINUM_NET_SOCKETOPS_H
#define PLATINUM_NET_SOCKETOPS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace platinum {

class IPAddress;
namespace socket {

enum Type : int {
  INET = AF_INET,
  UNIX = AF_LOCAL,
};

int Socket(Type type);
bool BindOrDie(int sockfd, const struct sockaddr *addr);
bool ListenOrDie(int sockfd, int backlog);
int Accept(int sockfd, IPAddress &address);
bool Connect(int sockfd, const struct sockaddr *addr);
bool Close(int sockfd);
bool ShutdownWrite(int sockfd);
sockaddr_in GetSockName(int sockfd);
sockaddr_in GetPeerName(int sockfd);
sockaddr* SockaddrCast(sockaddr_in *sockaddr_);

}

}


#endif //PLATINUM_NET_SOCKETOPS_H
