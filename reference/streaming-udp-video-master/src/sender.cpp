// This program attempts to gather video frames from a webcam or connected
// camera, encodes them into a compressed format (JPEG), and sends them over UDP
// to a receiver of the specified port number and address. The frames should be
// decodable on the other side and reconstructed into images.

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "protocols/basic_protocol.h"
#include "sender/sender_socket.h"
#include "util/util.h"
#include "video/video_capture.h"
#include "video/video_frame.h"

using udp_streaming_video::BasicProtocolData;
using udp_streaming_video::SenderSocket;
using udp_streaming_video::VideoCapture;
using udp_streaming_video::VideoFrame;
using std::vector;
//a little change
/*
void send_package(const SenderSocket socket, VideoCapture video_capture, BasicProtocolData protocol_data, int index);
void send_package(const SenderSocket socket, VideoCapture video_capture, BasicProtocolData protocol_data, int index){
    
    while(true){
      cv::Mat Origin_Mat = video_capture.GetFrameFromCamera().Frame_image_info();
      int rows = Origin_Mat.rows;
      int cols = Origin_Mat.cols;
      vector<vector<int>> row_vec = {{0,rows/2},{0,rows/2},{rows/2,rows},{rows/2,rows}};
      vector<vector<int>> col_vec = {{0,cols/2},{cols/2,cols},{0,cols/2},{cols/2,cols}};
      cv::Mat Sub_Mat(Origin_Mat,cv::Range(row_vec[index][0],row_vec[index][1]),cv::Range(col_vec[index][0],col_vec[index][1]));
      VideoFrame sub_frame(Sub_Mat);
      protocol_data.SetImage(sub_frame);//给frame中的video_frame中的mat赋值
      socket.SendPacket(protocol_data.PackageData());
      //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return;
}*/
void send_package(const SenderSocket socket, VideoCapture video_capture, BasicProtocolData protocol_data);
void send_package(const SenderSocket socket, VideoCapture video_capture, BasicProtocolData protocol_data){
    while(true){
      protocol_data.SetImage(video_capture.GetFrameFromCamera());//给frame中的video_frame中的mat赋值
      socket.SendPacket(protocol_data.PackageData());
      //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return;
}

/* File Name: server.c */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<iostream>
#include<string>
#define DEFAULT_PORT 8000
#define MAXLINE 4096

void split(char* str, const char* delimiter, char** substrings, int& num_substrings);


//args: "ls -l"; args_vec:{"ls","-l","null"}
//str: "ls -l" delimiter: " "
void split(char* str, const char* delimiter, char** substrings, int& num_substrings) {
	num_substrings = 0;
	char* token = strtok(str, delimiter);
	while (token != NULL && num_substrings < 10) {
		substrings[num_substrings++] = token;
		token = strtok(NULL, delimiter);
	}
}





int main(int argc, char** argv) {

  float rate = 1;//static_cast<float>(Resolution) / 100.0;
  //在这里开启server 开始接受信息
  std::vector<int> camera_index;
  for(int i=0;i<10;i++){
      cv::VideoCapture  cap(i);
      if(cap.isOpened()){
        int desiredWidth = 1920;
        int desiredHeight = 1080;
        cap.set(cv::CAP_PROP_FRAME_WIDTH,desiredWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT,desiredHeight);
        camera_index.push_back(i);
        std::cout<< "Camera "<< i << "Property:" <<std::endl;
        std::cout<< "Width: "<<cap.get(cv::CAP_PROP_FRAME_WIDTH)<<std::endl;
        std::cout<< "Height: "<<cap.get(cv::CAP_PROP_FRAME_HEIGHT)<<std::endl;
        cap.release();
        }
      else{
        std::cout<< "Camera "<< i <<"is bad!"<<std::endl;
        }
  }
  
  
  VideoCapture video_capture(false, rate,camera_index[0]);//false, 0.25
  //Get the information of the second camera.
  VideoCapture video_capture_second(false, rate, camera_index[1]);//false, 0.25
  BasicProtocolData protocol_data;
  //Second protocol data for the second camera.
  BasicProtocolData protocol_data2;

  std::thread threads_[10];
  int count = 0;
  int    socket_fd, connect_fd;
  struct sockaddr_in servaddr;
  char    buff[4096];
  int     n;
  std::string str = "quit\n";
  char quit[10];
  strcpy(quit,str.c_str());
  //初始化Socket
  if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
      printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
      exit(0);
  }
  //初始化
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
  servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT

  //将本地地址绑定到所创建的套接字上
  if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
      printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
      exit(0);
  }
  //开始监听是否有客户端连接
  if( listen(socket_fd, 10) == -1){
      printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
      exit(0);
  }
  printf("======waiting for client's request======\n");

  while(1){
  //阻塞直到有客户端连接，不然多浪费CPU资源。
      if((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){
          printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
          continue;
      }
      //父进程到这里已经连接了一个新的client（不然父进程会一直阻塞）（然后下文的fork会为这个client新建一个子进程）
      //接受新连接上的客户端传过来的数据
      //n = recv(connect_fd, buff, MAXLINE, 0);
      //向客户端发送回应数据

      //if(!fork()){ //fork=0,如果是子进程，进入
          //对于fork可以再多理解一下。 当fork运行就会出现一个新的自进程。对于原来的父进程fork return子进程的id；对于子进程fork会return0；
          //我倒是可以让所有的自进程进入这里的一个函数进行首发操作
      std::cout<< "Another CLients!" <<std::endl;
      n = recv(connect_fd, buff, MAXLINE, 0);
      /*if(!strcmp(quit,buff)){
          close(connect_fd);
          return 0;
      }*/
      if(n == 0)  //close的时候n==0
      {
          printf("有客户端断开连接\n");
          close(connect_fd);
          return 0;
      }
      //std::cout<< n <<std::endl;
      buff[n-1] = '\0';//如果用\n结尾会出问题，将\n变成\0

      printf("recv msg from client: %s\n", buff);
      char* args_vec[10];
      int length;
      split(buff, " ", args_vec, length);
      args_vec[length] = NULL;
      std::cout<< length <<std::endl;
      for(int i=0;i<length;i++){
          std::cout<<"!!:" << args_vec[i]<<std::endl;
      }
      //const char* command_path = NULL;

      //char command_path2[] = "/bin/ls";
      //execv(command_path2,args_vec);
      //thread 发送信息


      std::string Port = args_vec[1];
      std::string ip_address = args_vec[2];
      
      int port=0;

      std::istringstream ss(Port);
      ss >> port;
      const SenderSocket socket1(ip_address, port);//创建Socket
      const SenderSocket socket2(ip_address, port+1);//创建Socket
      std::cout<<"Send to IP:"<<ip_address <<" ,Port:"<< port <<std::endl;
      if(count<8){
        threads_[count] = std::thread(send_package, socket1, video_capture, protocol_data);
        count++;
        threads_[count] = std::thread(send_package, socket2, video_capture_second, protocol_data2);
        count++;
        }
      else{
        std::cout<< "Not enough thread!"<<std::endl;
      }
      close(connect_fd);
      //}
          
          //if(send(connect_fd, "Receive Your Love!\n", 26,0) == -1) perror("send error");//给自进程发送系信息
          //close(connect_fd);
          //exit(0);

      //所有的自进程都不会出来，父进程才能到这里。父进程不做信息处理工作，只处理accept连接
  }
      for(int i=0;i<count;i++){
        threads_[i].join();
      }
      close(socket_fd);

    /*
    const SenderSocket socket(ip_address, port);//创建Socket
    const SenderSocket socket2(ip_address, 8081);//创建Socket
    const SenderSocket socket_1(ip_address, 8081);//创建Socket
    const SenderSocket socket_2(ip_address, 8082);//创建Socket
    const SenderSocket socket_3(ip_address, 8083);//创建Socket
    std::cout << "Sending to " << ip_address
              << " on port " << port << "." << std::endl;*/

    /*
    std::thread threads[4];//创建4个线程
    threads[0] = std::thread(send_package, socket, video_capture, protocol_data);
    threads[1] = std::thread(send_package, socket2, video_capture, protocol_data);
    //threads[2] = std::thread(send_package, socket_2, video_capture, protocol_data, 2);
    //threads[3] = std::thread(send_package, socket_3, video_capture, protocol_data, 3);
    threads[0].join();
    threads[1].join();*/
    //threads[2].join();
    //threads[3].join();
    /*
     while (true) {  // TODO: break out cleanly when done.
      protocol_data.SetImage(video_capture.GetFrameFromCamera());//给frame中的video_frame中的mat赋值
      socket.SendPacket(protocol_data.PackageData());
    } */
    return 0;
}



