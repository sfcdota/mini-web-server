//#include "includes/allowed_library_includes.hpp"
//
//char webpage[] =
//    "HTTP/1.1 200 OK\r\n"
//    "Content-Type text/html; charset=UTF-8\r\n\r\n"
//    "<!DOCTYPE html>\r\n"
//    "<html><head><title>webserv</title>\r\n"
//    "<style>body {background-color: #FFFF00}</style></head>\r\n"
//    "<body><center><h1>Hello world!</h1><br>\r\n";
//std::vector<int> client_read;
//
///// Guard for libc function errors
///// \param retval returned function value, that will go through this function if success.
///// \param rw_operation enables check for errno only when false (not read-write operation).
///// \return
//int guard(int retval, bool rw_operation)
//{
//  if (retval == -1) {
//    if (!rw_operation && EWOULDBLOCK != errno) {
//      std::cout << strerror(errno) << std::endl;
//      for (std::vector<int>::iterator it = client_read.msg_pos(); it != client_read.end(); it++) {
//        close(*it);
//      }
//      exit(EXIT_FAILURE);
//    }
//  }
//  return retval;
//}
//
//void print_vec(std::vector<int> vec) {
//  std::cout << "New select iteration. Client list now = ";
//
//  for (std::vector<int>::iterator it = client_read.msg_pos(); it != client_read.end(); it++) {
//    std::cout << *it << ",";
//  }
//  std::cout << std::endl;
//}
//
//int main(int argc, char **argv) {
//  struct sockaddr_in server_addr = {}, client_addr = {};
//  socklen_t sin_len = sizeof(client_addr);
//  int fd_server, fd_client;
//  int fdimg;
//  int opt_value = 1;
//
//  char *buf = (char *)ft_calloc(2048, sizeof(char));
//  fd_server = guard(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0), false);
//  fcntl(fd_server, F_SETFL, O_NONBLOCK);
//  guard(setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt_value, sizeof(int)), false);
//
//  server_addr.sin_family = AF_INET;
//  server_addr.sin_addr.s_addr = INADDR_ANY;
//  server_addr.sin_port = htons(8080);
//  guard(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)), false);
//  guard(listen(fd_server, 32), false);
//  std::vector<int> client_write;
//  timeval timeout = {};
//  fd_set read_master_set, read_working_set; // for now used only read_fd
//  FD_ZERO(&read_master_set);
//  FD_ZERO(&read_working_set);
//  FD_SET(fd_server, &read_master_set);
//  client_read.push_back(fd_server);
//  int select_value;
//  int max_fd = fd_server;
//  int status;
//  bool empty_line;
//  while (true) {
//    ft_memcpy(&read_working_set, &read_master_set, sizeof(read_master_set));
//    timeout.tv_sec = 3;
//    print_vec(client_read);
//    select_value = guard(select(max_fd + 1, &read_working_set, NULL, NULL, &timeout), false);
//    if (select_value == 0) {
//      std::cout << "No incoming connections last 3 seconds" << std::endl;
//    }
//    else {
//      for (std::vector<int>::iterator it = client_read.msg_pos(); it != client_read.end() && select_value > 0; it++) {
//        if (FD_ISSET(*it, &read_working_set)) {
//          if (*it == fd_server) {
//            std::cout << "Listening socket is ready for incoming connections" << std::endl;
//            while ((fd_client = guard(accept(fd_server, (struct sockaddr *) &client_addr, &sin_len), false)) != -1) {
//              std::cout << "New connection accepted with fd = " << fd_client << std::endl;
//              fcntl(fd_client, F_SETFL, O_NONBLOCK);
//              FD_SET(fd_client, &read_master_set);
//              client_read.push_back(fd_client);
//            }
//            max_fd = *std::max_element(client_read.msg_pos(), client_read.end());
//            std::cout << "Ended handle of incoming connections with max_fd = " << max_fd << std::endl;
//            select_value--;
//          }
//          else {
//            std::cout << "Trying to read from client with socked fd = " << *it << std::endl;
//            empty_line = false;
//            while (!empty_line && (status = (guard(get_next_line(*it, &buf), true))) > 0) {
//              std::cout << status << " bytes received from client with socket fd = " << *it << std::endl;
//              std::cout << std::endl << "Those bytes are:" << std::endl << buf << std::endl << "*****************"
//              << std::endl;
//              if ((empty_line = !strncmp("\r\n\r\n", &buf[status - 4], 4)))
//                std::cout << "Checking for empty line of client request... &buf[status - 4] =$" << &buf[status - 4]
//                << "$" << std::endl;
//              if (empty_line)
//                std::cout << "Client empty line found! Server will close connection" << std::endl;
//              if ((status = guard(send(*it, webpage, sizeof(webpage), 0), true)) != -1)
//                std::cout << status << " bytes answered to client with socket fd = " << *it << std::endl;
//              memset(buf, 0, 2048);
//            }
//            if (!status || empty_line) {
//              std::cout << (empty_line ? "Server" : "Client") << " closed connection with socket fd = " << *it
//              << std::endl;
//              close(*it);
//              FD_CLR(*it, &read_master_set);
//              if (max_fd == *it)
//                max_fd = *std::max_element(client_read.msg_pos(), client_read.end());
//              client_read.erase(it--);
//            }
//          } // End of else when *it != fd_server
//        } // End of if FD_ISSET()
//      } // End of for cycle
//    } // End of else when select_value != 0
//  }
//
//  return 0;
//}
