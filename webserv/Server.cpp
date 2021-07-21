//
// Created by sfcdo on 20.05.2021.
//

#include "Server.hpp"



Server::Server(const std::vector<ServerConfig> &config, const ssize_t & INPUT_BUFFER_SIZE)
    : AServer(config, INPUT_BUFFER_SIZE) {}

const std::string Server::PrintLog(const ServerLoggingOptions &option) const {
  if (option == ServerLoggingOptions::ZERO)
    return std::string();
  std::stringstream ss;
  ss << "Request: ";
  if (option == ServerLoggingOptions::SERVERELEMENT_COUNT)
    ss << "server list size = " <<  server.size();
  else if (option == ServerLoggingOptions::TIMEOUT)
    ss << "server waiting for incoming clients";
  else if (option == ServerLoggingOptions::READELEMENT_COUNT)
    ss << "read list size = " << read.size();
  else if (option == ServerLoggingOptions::WRITELEMENT_COUNT)
    ss << "write list size = " << write.size();
  else if (option == ServerLoggingOptions::EVERYELEMENT_COUNT)
    ss << "server list size = " <<  server.size() << std::endl
       << "read list size = " << read.size() << std::endl
       << "write list size = " << write.size();
  else if (option == ServerLoggingOptions::STATUS)
    ss << "server status = " << status;
  else if (option == ServerLoggingOptions::INITIALIZING)
    ss << "server initializing ... ";
  else if (option == ServerLoggingOptions::ACCEPTING)
    ss << "server accepted connection ... ";
  else if (option == ServerLoggingOptions::READING)
    ss << "server trying to read from client socket ... ";
  else if (option == ServerLoggingOptions::WRITING)
    ss << "server trying to write to client socket ... ";
  ss << std::endl;
  return ss.str();
}

/// Guard for libc function errors
/// \param retval returned function value, that will go through this function if success.
/// \param rw_operation enables check for errno only when false (not read-write operation).
/// \return inputed retval if not exited
int Server::Guard(ssize_t retval) {
  if (retval == -1) {
      if (EWOULDBLOCK != errno && EAGAIN != errno) {
        std::cout << strerror(errno) << std::endl;
        for (read_iterator it = read.begin(); it != read.end(); it++)
          close(it->GetClientFd());
        for (write_iterator it = write.begin(); it != write.end(); it++)
          close(it->GetClientFd());
        exit(EXIT_FAILURE);
      }
  }
  return retval;
}

void Server::Run() {
  logger.WriteLog(*this, ServerLoggingOptions::INITIALIZING);
  Initialize();
  logger.WriteLog(*this, ServerLoggingOptions::SERVERELEMENT_COUNT);
  while (true) {
    memcpy(&working_read_, &master_read_, sizeof(master_read_));
    memcpy(&working_write_, &master_write_, sizeof(master_write_));
    timeout_.tv_sec = TIMOUT_SEC;
    if ((status = Guard(select(FD_SETSIZE, &working_read_, &working_write_, NULL, &timeout_))) == 0) {
      logger.WriteLog(*this, ServerLoggingOptions::TIMEOUT);
    } else {
      AcceptConnections();
//      logger.WriteLog(*this, ServerLoggingOptions::READELEMENT_COUNT);
      SocketsRead();
//      logger.WriteLog(*this, ServerLoggingOptions::WRITELEMENT_COUNT);
      SocketsWrite();
    }
  }
}
void Server::AcceptConnections() {
  int client_fd;
  sockaddr_in addr = {};
  socklen_t slen;
  for (server_iterator it = server.begin(); it != server.end(); it++) {
    if (FD_ISSET(it->GetServerFd(), &working_read_)) {
      if ((client_fd = Guard(accept(it->GetServerFd(), (struct sockaddr *) &addr, &slen))) != -1) {
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        FD_SET(client_fd, &master_read_);
        read.push_back(ReadElement(it->GetServerFd(), client_fd, it->GetConfig(), addr));
      }

    }
  }
}

std::string update_string_to_see_crlf(std::string in) {
  size_t pos;
  while ((pos = in.find('\r')) != -1)
    in.replace(pos, 1, "\\r");
  while ((pos = in.find('\n')) != -1)
    in.replace(pos, 1, "\\n");
  return in;
}


void Server::SocketsRead() {
  for (read_iterator it = read.begin(); it != read.end(); it++) {
    if (FD_ISSET(it->GetClientFd(), &working_read_)) {
//      logger.WriteLog(*it, ReadElementLoggingOptions::CLIENT_IS_SET);
      status = buffer_reader_.GetClientMessage(it);
    }
    it->UpdateLastActionSeconds();
    if (!status || it->GetIdleSeconds() > 120) {
      ClearBrokenConnection(it->GetClientFd());
      read.erase(it--);
    } else if (it->GetRequest().IsFormed()) {
      logger.WriteLog(it->GetRequest(), RequestLoggingOptions::FULL_REQUEST);
      FD_CLR(it->GetClientFd(), &master_read_);
      FD_SET(it->GetClientFd(), &master_write_);
//      if (it->GetRequest().GetRequestLine().at("method") == "POST") {
//        bool k = 0;
//      }

      write.push_back(WriteElement(it->GetServerFd(), it->GetClientFd(),
                        it->GetRequest().IsCloseOnEnd(),
                                   Response(it->GetRequest()).GetResponse()));
      it->ClearRequest();
      if (it->GetRequest().IsCloseOnEnd()) {
        read.erase(it--);
      }
    }
  }
}


void Server::SocketsWrite() {
  int client_fd;
  for (write_iterator it = write.begin(); it != write.end(); it++) {
//    if (it->GetOutput().find("Content-Length: 0"))
    if (FD_ISSET(it->GetClientFd(), &working_write_)) {
      while ((status = send(it->GetClientFd(), &it->GetOutput().c_str()[it->GetSentBytes()],
                       it->GetOutLength() - it->GetSentBytes(), 0)) != -1
            ) {
        logger.WriteLog(*it, WriteElementLoggingOptions::OUTPUT);
        it->IncreaseSentBytes(status);
        logger.WriteLog(*it, WriteElementLoggingOptions::SENT_BYTES);
        if (it->GetSentBytes() == it->GetOutLength()) {
          FD_CLR(it->GetClientFd(), &master_write_);
          if (it->GetCloseOnEnd()) {
            logger.WriteLog(*it, WriteElementLoggingOptions::CLOSE_ON_END);
            close(it->GetClientFd());
          }
          else
              FD_SET(it->GetClientFd(), &master_read_);
          write.erase(it--);
        }
      }
    }
  }
}

Server::~Server() {}

void Server::Initialize() {
  int options_value = 1;
  FD_ZERO(&master_read_);
  FD_ZERO(&master_write_);
  FD_ZERO(&working_read_);
  FD_ZERO(&working_write_);
  for (std::vector<ServerConfig>::const_iterator it = config_.begin(); it != config_.end(); it++) {
    sockaddr_in addr = {};
    addr.sin_addr.s_addr = INADDR_ANY; // should be it->host
    addr.sin_family = AF_INET;
    addr.sin_port = htons(it->port);
    int server_fd = Guard(socket(AF_INET, SOCK_STREAM, 0));
    // SOCK_NONBLOCK on ubuntu only!

    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    Guard(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &options_value, sizeof(int)));
    // SO_REUSEPORT on ubuntu only!

    //todo save server addr
    Guard(bind(server_fd, (struct sockaddr *) &addr, sizeof(sockaddr_in)));
    Guard(listen(server_fd, MAX_CONNECTIONS));
    FD_SET(server_fd, &master_read_);
    server.push_back(ServerElement(server_fd, addr, *it));
  }
}

void Server::ClearBrokenConnection(int fd) {
  for(write_iterator wit = write.begin(); wit != write.end(); wit++)
    if (wit->GetClientFd() == fd) {
      FD_CLR(wit->GetClientFd(), &master_write_);
      FD_CLR(wit->GetClientFd(), &working_write_);
      write.erase(wit--);
    }
  FD_CLR(fd, &master_read_);
  FD_CLR(fd, &working_read_);
  close(fd);
}
