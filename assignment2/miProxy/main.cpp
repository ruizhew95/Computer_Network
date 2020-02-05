#include "parser.hpp"
#include "server.hpp"

int main(int argc, char** argv){
  cmdArgs args = parser(argc, argv);

  MiProxyServer srv(args.listenPort, args.wwwIp,
      args.log, args.alpha);
  srv.Start();
}
