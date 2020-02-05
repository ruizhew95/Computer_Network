#pragma once
#include "socket.hpp"
#include "logger.hpp"
#include "client.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class MiProxyServer{
  public:
  MiProxyServer(int listenPort, const std::string& VideoServerHost, const
      std::string& logpath, double alpha);
  void Start();

  private:
  double alpha;
  std::shared_ptr<Socket> videoServerSocket;
  std::shared_ptr<Socket> listeningSocket;
  std::shared_ptr<ThroughputCalculator> tc;

  // Maps client ip to data
  std::unordered_map<std::string, std::shared_ptr<Client>> clients;
  std::vector<int> availableBitrates;

  int bitrateToUse; //added by ruizhew95, the chosen bitrate

  // Helpers
  void handleClientRequest(int fd);

  /* parseManifes should parse a string into availableBitrates */
  void parseManifest(std::string); // TODO(ruizhew95)

  /* updateThroughtput should calculate the delta of a request and add it to tc*/
  void updateThroughtput(const Request& req);

  /* Should look at current throughput from tc, and pick the largest bitrate
   * option which is less than tc->Current() */
  int chooseBitrate(); // TODO(ruizhew95)

  // Socket Helpers
  void buildFDSet(fd_set* fds);
  int selectReadyFD(fd_set* fds);
  bool msgOnListening(fd_set* fds);
  std::string getPeername(int);
};
