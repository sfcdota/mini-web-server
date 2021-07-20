//
// Created by sfcdo on 20.05.2021.
//

#include "Server.hpp"



Server::Server(const std::vector<ServerConfig> &config, const ssize_t & INPUT_BUFFER_SIZE)
    : AServer(config, INPUT_BUFFER_SIZE) {}


/// Guard for libc function errors
/// \param retval returned function value, that will go through this function if success.
/// \param rw_operation enables check for errno only when false (not read-write operation).
/// \return inputed retval if not exited
int Server::Guard(ssize_t retval, bool rw_operation) {
  if (retval == -1) {
    if (!rw_operation) {
      if (EWOULDBLOCK != errno && EAGAIN != errno) {
        std::cout << strerror(errno) << std::endl;
        for (read_iterator it = read.begin(); it != read.end(); it++)
          close(it->GetClientFd());
        for (write_iterator it = write.begin(); it != write.end(); it++)
          close(it->GetClientFd());
        exit(EXIT_FAILURE);
      }
    } else {
        std::cout << "Recv/Send error occured with status = -1" << std::endl;
        return -1;
      }
  }
  return retval;
}

void Server::Run() {
  Initialize();
  while (true) {
    memcpy(&working_read_, &master_read_, sizeof(master_read_));
    memcpy(&working_write_, &master_write_, sizeof(master_write_));
    timeout_.tv_sec = TIMOUT_SEC;
    if ((status = Guard(select(FD_SETSIZE, &working_read_, &working_write_, NULL, &timeout_), false)) == 0) {
//      std::cout << "No incoming connections last " << timeout_.tv_sec << " seconds" << std::endl;
    } else {
//      std::cout << "Server ready for connections" << std::endl;
      AcceptConnections();
//      std::cout << std::setw(50) << "END OF ACCEPT CYCLE | read vector size = " << read.size() << " write vector size = " << write.size() << std::endl;
      SocketsRead();
//      std::cout << std::setw(50) << "END OF READ CYCLE | read vector size = " << read.size() << " write vector size = " << write.size() << std::endl;
      SocketsWrite();
//      std::cout << std::setw(50) << "END OF WRITE CYCLE | read vector size = " << read.size() << " write vector size = " << write.size() << std::endl;
    }
  }
}
void Server::AcceptConnections() {
  int client_fd;
  sockaddr_in addr = {};
  socklen_t slen;
  for (server_iterator it = server.begin(); it != server.end(); it++) {
    if (FD_ISSET(it->GetServerFd(), &working_read_)) {
//      std::cout << "Listening socket " << it->server_fd << " is ready for incoming connections" << std::endl;
//      if ((client_fd = Guard(accept(it->server_fd, (struct sockaddr *) &addr, &slen), false)) != -1) {
      if ((client_fd = Guard(accept(it->GetServerFd(), 0, 0), false)) != -1) {
//        PrintLog(it, "accepted client connection", client_fd);
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        FD_SET(client_fd, &master_read_);
        read.push_back(ReadElement(it->GetServerFd(), client_fd, it->GetConfig(), addr));
      }
//      std::cout << "Ended handle of incoming connections with max_fd = " << max_fd << std::endl;

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
      PrintLog(it, "client IS_SET for read", it->GetClientFd());
      status = buffer_reader_.GetClientMessage(it);
    }
    it->UpdateLastActionSeconds();
//    std::cout << "status = " << status << std::endl;
    if (!status || it->GetIdleSeconds() > 10) {
      ClearBrokenConnection(it->GetClientFd());
      read.erase(it--);
    } else if (it->GetRequest().IsFormed()) {
      it->GetRequest().Print();
      FD_CLR(it->GetClientFd(), &master_read_);
      FD_SET(it->GetClientFd(), &master_write_);
      write.push_back(WriteElement(it->GetServerFd(), it->GetClientFd(),
                        it->GetRequest().IsCloseOnEnd(),
                                   Response(it->GetRequest()).SendResponse()));
      if (it->GetRequest().IsCloseOnEnd()) {
//        PrintLog(it, "ended read by not keep alive behavior", it->fd);
//        std::cout << "Client_fd = " << it->fd << " read ended due not keep alive connection" << std::endl;
        read.erase(it--);
      }
      it->ClearRequest();
    }
  }
}


void Server::SocketsWrite() {
  int client_fd;
  for (write_iterator it = write.begin(); it != write.end(); it++) {
    if (FD_ISSET(it->GetClientFd(), &working_write_)) {
      if ((status = Guard(
          send(it->GetClientFd(), &it->GetOutput().c_str()[it->GetSentBytes()],
               it->GetOutLength() - it->GetSentBytes(), 0),
          true
      )) != -1) {
        std::cout << "sending response with size = " << it->GetOutLength() << ":" << std::endl;
//        PrintLog(it, it->output, it->fd);
//        std::cout << std::endl;
        it->IncreaseSentBytes(status);
        std::stringstream ss;
        if (!it->GetOutLength()) {
          ss << "[ WARNING! Response length = 0 ";
        }
        ss << status << "/" << it->GetOutLength() << " bytes send";
        PrintLog(it, ss.str(), it->GetClientFd());
        if (it->GetSentBytes() == it->GetOutLength()) {
          FD_CLR(it->GetClientFd(), &master_write_);
          if (it->GetCloseOnEnd()) {
//            PrintLog(it, "closed connection (disabled keep-alive)", it->fd);
//            std::cout << "closed client_fd = " << it->fd << " connection due not keep alive" << std::endl;
            close(it->GetClientFd());
          }
          else
              FD_SET(it->GetClientFd(), &master_read_);
          write.erase(it--);
        }
      }
//      if (status == -1)
//        PrintLog(it, "send returned -1 trying to x", it->fd);
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
    int server_fd = Guard(socket(AF_INET, SOCK_STREAM, 0), false);
    // SOCK_NONBLOCK on ubuntu only!

    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    Guard(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &options_value, sizeof(int)), false);
    // SO_REUSEPORT on ubuntu only!

    //todo save server addr
    Guard(bind(server_fd, (struct sockaddr *) &addr, sizeof(sockaddr_in)), false);
    Guard(listen(server_fd, MAX_CONNECTIONS), false);
    FD_SET(server_fd, &master_read_);
    server.push_back(ServerElement(server_fd, addr, *it));
  }
}

template<class Iterator>
void Server::PrintLog(Iterator it, const std::string & msg, int client_fd) {
//  std::cout << "Server #" << it->server_fd << " " <<
//            std::setw(90) <<  msg << std::setw(10) << "| Client#" << client_fd << std::endl;
}

long Server::GetTimeInSeconds() {
  gettimeofday(&timev, NULL);
  return timev.tv_sec;
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
