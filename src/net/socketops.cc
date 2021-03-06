/**
 * Created by Crow on 11/21/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to immplmente the basic socket APIs
 */

#include "net/socketops.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "utility/logger.h"
#include "net/ip_address.h"

using namespace platinum;

int socket::Socket(Type type)
{
    int fd = ::socket(type, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (fd < 0) {
      LOG(ERROR) << "socket::Socket() => Socket Create Error";
      std::abort();
    }

  return fd;
}

bool socket::BindOrDie(int sockfd, const struct sockaddr *addr)
{
  if (::bind(sockfd, addr, sizeof(struct sockaddr))) {
    LOG(ERROR) << "socket::BindOrDie() => Bind Error";
    std::abort();
  }

  return true;
}

bool socket::ListenOrDie(int sockfd, int backlog)
{
  if (::listen(sockfd, backlog)) {
    LOG(ERROR) << "socket::Listen()";
    std::abort();
  }

  return true;
}

bool socket::Connect(int sockfd, const struct sockaddr *addr)
{
  if (::connect(sockfd, addr, sizeof(sockaddr))) {
    LOG(ERROR) << "scokaet::Connect() => Connect Error";
    std::abort();
  }

  return true;
}

int socket::Accept(int sockfd, IPAddress &address)
{
  sockaddr_in sockaddrin{};
  socklen_t len = sizeof(sockaddrin);
  int connfd = ::accept4(sockfd,
      reinterpret_cast<struct sockaddr *>(&sockaddrin),
      &len,
      SOCK_NONBLOCK | SOCK_CLOEXEC);

  if (connfd < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return -1;
    } else {
      LOG(ERROR) << "socket::Accept()";
      std::abort();
    }
  } else {
    address.set_port(::ntohs(sockaddrin.sin_port));
    address.set_ip(::inet_ntoa(sockaddrin.sin_addr));
    return connfd;
  }
}

sockaddr_in socket::GetSockName(int sockfd)
{
  sockaddr_in sockaddr{};
  socklen_t len = sizeof(sockaddr);
  if (::getsockname(sockfd, reinterpret_cast<struct sockaddr *>(&sockaddr), &len)) {
    LOG(ERROR) << "socket::GetSockName()";
    std::abort();
  }

  return sockaddr;
}

sockaddr_in socket::GetPeerName(int sockfd)
{
  sockaddr_in sockaddr{};
  socklen_t len = sizeof(sockaddr);
  if (::getsockname(sockfd, reinterpret_cast<struct sockaddr *>(&sockaddr), &len)) {
    LOG(ERROR) << "socket::GetPeerName() => Get Peer Name Error";
    std::abort();
  }

  return sockaddr;
}

bool socket::Close(int sockfd)
{
  if (::close(sockfd)) {
    LOG(ERROR) << "socket::Close() => Close Error";
    std::abort();
  }
  return true;
}

bool socket::ShutdownWrite(int sockfd)
{
  if (::shutdown(sockfd, SHUT_WR)) {
    LOG(ERROR) << "socket::ShutdownWrite() => Shutown Error";
    std::abort();
  }
  return true;
}