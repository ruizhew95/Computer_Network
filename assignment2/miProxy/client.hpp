#pragma once
#include "throughput.hpp"
#include "socket.hpp"

#include <memory>
#include <chrono>

class Request;

class Client{
  public:
  Client(int fd, int alpha);
  std::shared_ptr<Socket> sock;
};

class Request{
  public:
  Request(std::shared_ptr<Client> c):cli(c){}

  /* Recieves generic HTTP response into content */
  void recvResponse();

  /* Clear content then recieve a manifest file into content. Does not parse
   * manifest, only recieves it. */
  void recvManifest(); // TODO(ruizhew95)
  /* Clear content then recieve a http headers into context */
  void recvHTTPHeaders();
  /* Forward all of content to the passed in socket */
  void Forward(std::shared_ptr<Socket>);

  // Look through content, determine if it holds an HTTP GET for a manifest
  bool isManifest() const {
    //check for .f4m extension in content;
    std::string f4mExt = ".f4m";
    std::size_t found = content.find(f4mExt);
    return (found != std::string::npos);
  }

  // Look through content, determine if it holds an HTTP GET for a video chunk
  bool isVideoChunk() const {
    //check content for "Seg" and "-Frag"
    std::string segExt = "Seg";
    std::string fragExt = "-Frag";

    std::size_t found1 = content.find(segExt);
    std::size_t found2 = content.find(fragExt);

    return (found1 != std::string::npos && found2 != std::string::npos);
  }

  /* Convert the request in content from a manifest to a manifest_nolist
   * request. Should first check isManifest() */
  void makeNolist() {
    if (!isManifest()){
      std::cout << "ERROR: tried to convert manifest request to manifest_nolist but";
      std::cout << " wasn't given manifest file" << std::endl;
      exit(1);
    }

    std::string f4mExt = ".f4m";
    std::size_t found = content.find(f4mExt);

    std::string noList = "_nolist";
    content.insert(found, noList);
  }

  /* Convert the request in content from a video sequence request to a chosen
   * bitrate. Should check isVideoChunk() and should pick a bitrate based on tc
   * and availableBitrates */
  void SubBitrate(int); // TODO(ruizhew95)

  /* Set start to now */
  void markStart(){ start_time = std::chrono::system_clock::now(); }
  /* Set end to now */
  void markEnd(){ end_time = std::chrono::system_clock::now(); }
  // Returns how long the request took
  std::chrono::duration<double, std::milli> Duration() const;

  // Provide access to content
  const std::string& Content() const{return content;}
  std::string& MutableContent(){return content;}

  private:
  std::shared_ptr<Client> cli;
  std::string content;
  int contentLength;

  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_time;

  std::string chunk_name;//added by ruizhew95
};
