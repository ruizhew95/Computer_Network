#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <cstring>
#include <netdb.h>
#include <sys/socket.h>

#define MAXACCEPTQUEUE 10

/* Socket provides a c++ wrapper to standard C sockets */
class Socket{
  public:
  /* Construct a new socket */
  Socket(){
    _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_sockfd == -1){
      std::cerr << "Couldn't get a socket: " << strerror(_sockfd) << '\n';
      return;
    }

    int yes = 1;
    int opt = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (opt == -1){
      std::cerr << "Couldn't get a socket: " << strerror(_sockfd) << '\n';
      return;
    }
  }

  /* Wrap an existing socket */
  Socket(int fd){
    _sockfd = fd;
  }

  /* Set a socket to start listening from any host */
  void Listen(int port){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(_sockfd, (struct sockaddr *)& addr, sizeof(addr));

    int err = ::listen(_sockfd, MAXACCEPTQUEUE);
    if (err){
      std::cerr << "Cannot listen: " << strerror(err) << '\n';
    }
  }

  /* Accept a new connection from a host */
  int Accept(){
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    return accept(_sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
  }

  /* Connect from this socket to a host on a port */
  void Connect(const std::string& host, int port){
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons((u_short) port);
    struct hostent* sp = gethostbyname(host.c_str());
    memcpy(&server.sin_addr, sp->h_addr, sp->h_length);

    int err = ::connect(_sockfd, (struct sockaddr *) &server, sizeof(server));
    if (err < 0){
      std::cerr << "Cannot connect to: " << host << " " << strerror(err) <<
        '\n';
    }
  }


  /* Send data to a host we have connected to */
  int Send(const std::vector<char> data){
    return send(_sockfd, data.data(), data.size(), MSG_NOSIGNAL);
  };

  /* Recieve data from a host we have connected to */
  int Recv(std::vector<char> &data, int len){
    data.resize(len);
    return recv(_sockfd, data.data(), len, 0);
  };

  /* Return the filedescriptor of this socket */
  int FD() const { return _sockfd; }

  private:
  int _sockfd;
};
