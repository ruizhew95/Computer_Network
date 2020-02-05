#include "client.hpp"

#include <chrono>

Client::Client(int fd, int alpha){
  // Wrap fd with sock
  this->sock = std::make_shared<Socket>(fd);
}

void Request::recvHTTPHeaders(){
  content.clear();
  contentLength = 0;
  std::vector<char> buff(0, 4);
  // Keep recieveing until we get double CLRF
  while (cli->sock->Recv(buff, 4) > 0 && strncmp(buff.data(), "\r\n\r\n", 4)){
    content += buff.data();
  }

  std::string content_length = "Content-Length: ";
  std::size_t content_pos = content.find(content_length);

  // If the header contains the Content-Length
  if(content_pos != std::string::npos)
    sscanf(content.substr(content_pos).c_str(), "%*s %d", &this->contentLength);
}

void Request::Forward(std::shared_ptr<Socket> sock){
  std::vector<char> buff;
  for (auto c : content)
    buff.push_back(c);

  sock->Send(buff);
}

std::chrono::duration<double, std::milli> Request::Duration() const {
  return std::chrono::duration<double, std::milli> (start_time - end_time);
}

void Request::recvResponse(){
  recvHTTPHeaders();
  if (contentLength){
    std::vector<char> buff(contentLength);
    int err = cli->sock->Recv(buff, contentLength);
    if (err < 0){
      std::cerr << "Failed to recieve\n";
      exit(1);
    }

    for (auto c : buff)
      content.push_back(c);
  }
}
void Request::recvManifest(){
      if(isManifest()){
        content.clear();
        std::vector<char> data;
        cli->sock->Recv(data, contentLength);
        for(int i=0; i<(int)data.size();i++){
            content += data[i];
        }
      }

  }

void Request::SubBitrate(int bitrateToUse){//have already been chosen in MiProxyServer
    //Fds.client_data->bitrate_request = *bitrate_browser;
      std::size_t left_pointer = content.find("vod/");
      std::size_t right_pointer = content.find("Seg");

      std::string bitrate_string = std::to_string(bitrateToUse); 
      content.replace(left_pointer+4, right_pointer-left_pointer-4, bitrate_string); //bitrate adapation

      //to find chunk name
      std::size_t name_left_pointer = content.find("http");
      std::size_t name_right_pointer = content.find("HTTP");
      chunk_name = content.substr(name_left_pointer, name_right_pointer-name_left_pointer);
      std::cout<<"Check chunk name "<<chunk_name<<std::endl;
}
