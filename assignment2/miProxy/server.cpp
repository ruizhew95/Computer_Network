#include "server.hpp"
#include "socket.hpp"
#include "client.hpp"

#include <string>
#include <vector>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO, FD_SETSIZE macros
#include <sys/socket.h>
#include <arpa/inet.h>


MiProxyServer::MiProxyServer(int listenPort, const std::string&
    VideoServerHost, const std::string& logpath, double alpha){

  this->videoServerSocket = std::make_shared<Socket>();
  this->videoServerSocket->Connect(VideoServerHost, 80);

  this->listeningSocket = std::make_shared<Socket>();
  this->listeningSocket->Listen(listenPort);
}

void MiProxyServer::Start(){
  fd_set readyFDs;
  while (true){
    // Clear fd set
    FD_ZERO(&readyFDs);

    // Build fd set
    buildFDSet(&readyFDs);

    // Select next ready fd
    int numReady = selectReadyFD(&readyFDs);
    if (numReady < 0){
      std::cerr << "select error" << strerror(numReady) << '\n';
      continue;
    }

    // If Master, add as client
    if (msgOnListening(&readyFDs)) {
      int newClient = this->listeningSocket->Accept();
      if (newClient < 0){
        std::cerr << "accept error" << strerror(newClient) << '\n';
        continue;
      }

      // Make a client
      auto c = std::make_shared<Client>(newClient, alpha);

      // Map ip string to client
      this->clients.emplace(getPeername(newClient), c);
    } else {
      handleClientRequest(this->listeningSocket->Accept());
    }
  }
}

/* Adds the listening socket and all client sockets to a fdset */
void MiProxyServer::buildFDSet(fd_set* fds){
  // Add master to the set
  FD_SET(listeningSocket->FD(), fds);

  // Add all clients to the set
  for (const auto &c : clients){
    FD_SET(c.second->sock->FD(), fds);
  }
}

int MiProxyServer::selectReadyFD(fd_set* fds){
  int activity = select(FD_SETSIZE, fds , NULL , NULL , NULL);
  if ((activity < 0) && (errno!=EINTR)) {
      perror("select error");
      return -1;
  }
  return activity;
}

bool MiProxyServer::msgOnListening(fd_set* fds){
  return FD_ISSET(listeningSocket->FD(), fds);
}

std::string MiProxyServer::getPeername(int fd){
  // Get the ip of the client
  sockaddr_storage peer;
  socklen_t len = sizeof(peer);
  int err = getpeername(fd, (sockaddr*)&peer, &len);
  if (err < 0){
    std::cerr << "failed to get peer: " << strerror(err) << '\n';
    throw new std::exception();
  }

  if (peer.ss_family != AF_INET){
    std::cerr << "Only support ipv4\n";
    throw new std::exception();
  }


  // Get string of ip addr
  struct sockaddr_in *s = (struct sockaddr_in*) &peer;
  char ipstr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
  std::string hname(ipstr);
  return hname;
}

void MiProxyServer::handleClientRequest(int fd){
  std::shared_ptr<Client> cli = this->clients.at(getPeername(fd));
  Request req(cli);
  req.recvHTTPHeaders();

  // Manipulate request if necessary
  if (req.isManifest()){
    req.Forward(this->videoServerSocket);

    // Get manifest from server
    req.recvManifest();

    parseManifest(req.Content());

    // Resend with nolist
    req.makeNolist();
    req.Forward(this->videoServerSocket);

    req.recvManifest();
    req.Forward(cli->sock);
    return;
  }

  if (req.isVideoChunk()){
    // Modify to bitrate for the client
    req.SubBitrate(chooseBitrate());
    req.markStart();
    req.Forward(this->videoServerSocket);

    req.recvResponse();
    req.markEnd();
    
    // Sets recv time, calculates bitrate
    updateThroughtput(req);

    req.Forward(cli->sock);
    return;
  }

  // Not a manifest, or chunk, just pass along
  req.Forward(this->videoServerSocket);
  req.recvResponse();
  req.Forward(cli->sock);
}

void MiProxyServer::updateThroughtput(const Request& req){
  auto duration = req.Duration();
  tc->Add(duration.count(), req.Content().length());
}

void MiProxyServer::parseManifest(std::string buffer){
  std::string media = "<media";
  std::string bitrate = "bitrate=\"";
  for(std::size_t media_pos = buffer.find(media); media_pos != std::string::npos; media_pos = buffer.find(media, media_pos+6)){
    std::size_t bitrate_pos = buffer.find(bitrate, media_pos);
    unsigned pos_bitrate = bitrate_pos + bitrate.size();
    unsigned bitrate_length = buffer.find('"', pos_bitrate) - pos_bitrate;
    std::string bitrate_string = buffer.substr(pos_bitrate, bitrate_length);
    int bitrate = std::stoi(bitrate_string);
    availableBitrates.push_back(bitrate);
  }
}
int MiProxyServer::chooseBitrate(){
    //Then determine which bitrate to choose
    double bitrateConstraint = tc->Current()/1.5; //current_throughput should be calculated before
	if (availableBitrates[0]>=bitrateConstraint) //the lowest bitrate choice even higher than constraint, have to choose that
	{	
		bitrateToUse = availableBitrates[0];		//bitrateToUse is the bitrate choosen for that browser						 		
	}
	else
	{	
		for (int j = 0; j < (int)availableBitrates.size(); ++j)
		{
			if (availableBitrates[j]<bitrateConstraint)
			{
				bitrateToUse=availableBitrates[j];
			}
			else 
			{											
				break;
			}
		}
	}
    return bitrateToUse;
}
