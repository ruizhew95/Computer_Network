#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sys/time.h>
#include <sys/socket.h>
#
void recv_and_find_contentLength(std::string buffer, clientDataFd Fds){
    buffer = "";
    std::string file_end = "\r\n\r\n";
    while(1){
        char temp_buffer = ' ';
        if(recv(Fds.client_data->server_fd, &temp_buffer, 1, 0)<0){
            std::cout<<"Error on receiving bytes"<<std::endl;
            exit(1);
        }
        else buffer = buffer + buf;
        
        //find file_end to ensure that all have been received
        std::size_t end_pos = buffer.find(file_end);
        if(end_pos != std::string::npos) break;
    }    

    std::string content_length = "Content-Length: ";
    std::string right_sign = "Keep-Alive: ";
    std::size_t content_pos = buffer.find(content_length);
    std::size_t right_pos = buffer.find(right_sign);

    if(content_pos != std::string::npos){
        Fds.client_data->content_length = std::stoi(buffer.substr(content_pos+16, right_pos-content_pos))    
        std::cout<<"Check content_length"<<std::endl;
        //HEADER has been processed, then content
        int count = 0;
        while(count < Fds.client_data->content_length){
            char temp_buffer = ' ';

            if(recv(Fds.client_data->server_fd, &temp_buffer, 1, 0)<0){
                std::cout<<"Error on receiving bytes"<<std::endl;
                exit(1);
            }
            else{
                buffer = buffer + buf;
                count++;
            } 
        }
    }
}
