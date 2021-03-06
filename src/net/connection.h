/**
 * Created by Crow on 12/11/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe Class Connection
 */

#ifndef PLATINUM_NET_CONNECTION_H
#define PLATINUM_NET_CONNECTION_H

#include <memory>
#include <functional>

#include "net/ip_address.h"
#include "protocol/parser.hpp"
#include "utility/noncopyable.h"
#include "utility/buffer/buffer.h"
#include "utility/buffer/writequeue.h"

namespace platinum {

class Socket;
class IPAddress;
class EventLoop;
class Channel;
class Connection : public Noncopyable {
 public:
  using EventCallback = std::function<void ()>;
  using MessageCallback = std::function<long (Connection *, Buffer &, std::unique_ptr<Parser> &)>;
  using CloseCallback = std::function<void (int)>;

  Connection(EventLoop *loop, int fd, ParserType type);
  ~Connection() override;

  // move TCPConnection callback to Channel
  void ConnectionEstablished();
  void ConnectionDesctroyed();
  void ShutDownConnection();
  void SendData(const unsigned char *data, size_t total);
  void SendFile(const std::string &pathname, size_t total);
  void ForceClose();

  // copy TCPServer callback -> TCPConnection, can't move, we'll create much TCPConnection by TCPServer::*callback
  void set_connection_callback(const EventCallback &callback);
  void set_write_callback(const EventCallback &callback);
  void set_message_callback(const MessageCallback &callback);
  void set_close_callback(const CloseCallback &callback);

  void set_forward_fd(int fd);
  int forward_fd();
  int socket_fd();

 private:
  void HandleRead();                                   // deal with readable event, doing by Channel::read_callback_
  void HandleWrite();                                  // deal with writeable event, doing by Channel::write_callback_
  void HandleError();                                  // deal with error, ...
  void HandleClose();                                  // deal with hangup event, doing by Channel::close_callback_

  void ErrorCallback();                                // private function to record the error situation
  void ShutdownInLoop();                               // push task in Loop to shutdown the connection
  void ForceCloseInLoop();

  EventLoop *loop_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Parser> parser_;
  EventCallback connection_callback_;                  // register by user provide function -> OnConnection fucntion
  MessageCallback message_callback_;                   // register by user provide function -> OnMessage Function
  EventCallback write_callback_;                       // register by Connector -> to check connection status, may be add timer
  CloseCallback close_callback_;                       // register as TCPServer::RemoveConnection
  Buffer read_buffer_;
  WriteQueue write_queue_;
  int forward_fd_;
};

}

#endif //PLATINUM_NET_CONNECTION_H
