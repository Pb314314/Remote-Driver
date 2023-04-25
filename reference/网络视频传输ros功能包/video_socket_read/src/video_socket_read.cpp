
//原理：客户端：利用OpenCV调取摄像头 --> 创建socket框架 --> 利用OpenCV将视频每一帧编码 --> 发送出去。
//      服务器端：创建socket框架 --> 接收到图片的字节序 --> 利用OpenCV的解码函数将每一帧图片解码 --> 显示图片或者说视屏


//！！！！因为ros里面用OpenCV 所以我就把视屏网络传输的程序写到ros框架中去了，如果只需要视屏网络传输的功能，可以忽略ros的存在。

//
//-------------------- ROS所需头文件-----------------------
#include<ros/ros.h>
//Use image_transport for publishing and subscribing to images in ROS
#include<image_transport/image_transport.h>
//Use cv_bridge to convert between ROS and OpenCV Image formats
#include<cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//Include headers for OpenCV Image processing
//-------------------- ROS所需头文件-----------------------

//----------------opencv的头文件 用于视屏编码和解码的--------
#include<opencv-3.3.1-dev/opencv2/imgproc/imgproc.hpp>
//Include headers for OpenCV GUI handling
#include<opencv-3.3.1-dev/opencv2/highgui/highgui.hpp>
#include<opencv-3.3.1-dev/opencv2/core.hpp>
#include<opencv-3.3.1-dev/opencv2/opencv.hpp>
//----------------opencv的头文件 用于视屏编码和解码的--------

//----------------socket网络编程所需头文件-------------------
#include<std_msgs/UInt8.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h> 
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h> 
#include<string.h> 
#include<sys/ioctl.h> 
#include<string>
#include<vector>
#include "std_msgs/String.h"
//----------------socket网络编程所需头文件-------------------

static int server_fd;
static int client_fd;

using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
    //---------socket框架----------
   struct sockaddr_in ser_addr;
   struct sockaddr_in cli_addr;
   //creat the socket;
   if((server_fd=socket(AF_INET,SOCK_DGRAM,0))==-1)

     {
       	perror("socket failed!\n");
        exit(1);
     }
    cout<<"creat socket successful! socket="<<server_fd<<endl;
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(10000);
    ser_addr.sin_addr.s_addr=inet_addr("192.168.0.101");
    if(bind(server_fd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr))==-1)
     {
      perror("bind error!\n");
      exit(1);
     }
    cout<<"bind successful!"<<endl;

    /*
    //set listen port
    if(listen(server_fd,5)==-1)
      {
       perror("bind error!\n");
       exit(1);
      }
     cout<<"set listen port sucessful!"<<endl;
     socklen_t sin_size =sizeof(struct sockaddr_in);
    if((client_fd=accept(server_fd,(struct sockaddr*)&cli_addr,&sin_size))==-1)
      {
	perror("accept error!");
	return 0;
       }
     cout<<"accept successful!"<<endl;
*/
//---------socket框架----------

    //---------ros的框架-----------
     ros::init(argc,argv,"video_ser_node");
     ros::NodeHandle nh;
     ros::Publisher pub = nh.advertise<std_msgs::String>("socket_string", 1000);
     cout<<"节点句柄成功！"<<endl;
     //---------ros的框架-----------
  
   Mat image;
   char buf[65536]={0};
  //ros::Rate loop_rate(200);
   while(true)
   {
/*
  //方法一：可行  （将收到的网络字节序解码）
  vector<uchar> decode;

	int n = read(client_fd, buf, sizeof(buf));
	int pos = 0;
	while (pos < n)
	{

	    decode.push_back(buf[pos++]);
	}
	//decode[n] = 0;
  memset(buf,0,sizeof(buf));
	image = imdecode(decode, CV_LOAD_IMAGE_COLOR);
 
  if(image.rows==0||image.cols==0)
  {
    memset(buf,0,sizeof(buf));
    continue;
  }
  */
  socklen_t client_len;
  client_len=sizeof(client_fd);
  int n=recvfrom(server_fd,buf,sizeof(buf),0,(struct sockaddr*)&client_fd,&client_len);
  //int n = read(client_fd, buf, sizeof(buf));
  cout<<"接收到的n：   "<<n<<endl;
  
  vector<uchar> decode(&buf[0],&buf[n]);//直接告诉buf的地址空间  减少一次内存拷贝	
  
	image = imdecode(decode, CV_LOAD_IMAGE_COLOR);//opencv的解码函数imdecode将每一帧图片的字节序decode解码为image图片
  if(image.rows==0||image.cols==0)
  {
    memset(buf,0,sizeof(buf));
    continue;
  }
  memset(buf,0,sizeof(buf));

 /*
  vector<uchar> decode;
  char buff[1];
  read(client_fd, buff, 1);
  int num=(int)buff[0];
  int n = read(client_fd, &decode[0],num);
  if(n<100)
  continue;
  image = imdecode(decode, CV_LOAD_IMAGE_COLOR);
  if(image.rows==0||image.cols==0)
  {
    memset(buf,0,sizeof(buf));
    continue;
  }
 */ 
	imshow("server_recv_video", image);//循环显示美贞图片成为视屏。
	waitKey(1);

   }
   close(client_fd);
   close(server_fd);
   return 0;
}
