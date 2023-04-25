#include<ros/ros.h>
//Use image_transport for publishing and subscribing to images in ROS
#include<image_transport/image_transport.h>
//Use cv_bridge to convert between ROS and OpenCV Image formats
#include<cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//Include headers for OpenCV Image processing
#include<opencv-3.3.1-dev/opencv2/imgproc/imgproc.hpp>
//Include headers for OpenCV GUI handling
#include<opencv-3.3.1-dev/opencv2/highgui/highgui.hpp>
#include<opencv-3.3.1-dev/opencv2/core.hpp>
#include<opencv-3.3.1-dev/opencv2/opencv.hpp>
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
#include "std_msgs/String.h"

#define BUFFER_SIZE 50000

using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
   
   
   int server_fd;
   int client_fd;
   struct sockaddr_in ser_addr;
   struct sockaddr_in cli_addr;
   //creat the socket;
   if((server_fd=socket(AF_INET,SOCK_STREAM,0))==-1)

     {
       	perror("socket failed!\n");
        exit(1);
     }
    cout<<"creat socket successful! socket="<<server_fd<<endl;
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(10000);
    ser_addr.sin_addr.s_addr=inet_addr("192.168.0.104");
    if(bind(server_fd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr))==-1)
     {
      perror("bind error!\n");
      exit(1);
     }
    cout<<"bind successful!"<<endl;
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
     

     uchar readbuff[BUFFER_SIZE];

     Mat image(480, 640, CV_8UC3, cv::Scalar(0));

     ros::init(argc,argv,"video_ser_node");
     ros::NodeHandle nh;
     ros::Publisher pub = nh.advertise<std_msgs::String>("socket_string", 1000);
     cout<<"节点句柄成功！"<<endl;
     ros::Rate loop_rate(50);
cout<<"1"<<endl;




char **data = new char*[3];
for (int i = 0; i < 3; i++)
{
data[i] = new char[image.rows* image.cols];
memset(data[i],0,sizeof(char)*image.rows* image.cols);
}

while(ros::ok())
{

for (int i = 0; i < 3; i++)
{
  //memset(data[i],0,sizeof(char)*image.rows* image.cols);
  int rval=read(client_fd, data[i], image.rows* image.cols);
}
if (strlen(data[0]) == 0 || strlen(data[1]) == 0 || strlen(data[2]) == 0 )
  {
  //释放
  for (int i = 0; i < 3; i++)
  {
   delete[]data[i];
  }
  return -1;
}


for (int i = 0; i <image.rows; i++)
{
    for (int j = 0; j <image.cols; j++)
       {
         //uchar *p = SrcImg.ptr<uchar>(i, j);
        image.ptr<Vec3b>(i)[j][0] = data[0][j + i*image.cols];
	image.ptr<Vec3b>(i)[j][1] = data[1][j + i*image.cols];
	image.ptr<Vec3b>(i)[j][2] = data[2][j + i*image.cols];
	}
}




/*
cout<<"2"<<endl;
        int rval;
        if ((rval=read(client_fd, readbuff,27655)) < 0) 
          {
	    perror("reading stream error!");
	    continue;
          }
cout<<"3"<<endl;
         int rows,cols;
         int i=0;
cout<<"4"<<endl;
         rows=readbuff[i];
cout<<"5"<<rows<<endl;
         i++;
         cols=readbuff[i];
cout<<"6"<<cols<<endl;
         i++;
         for(int r=0;r<rows;r++)
           {
              for(int c=0;c<cols;c++)
                {
cout<<"7    "<<readbuff[i]<<endl;
                  image.ptr<Vec3b>(r)[c][0]=readbuff[i];//green
cout<<"8     "<<image.ptr<Vec3b>(r)[c][0]<<endl;
                   i++;
                  image.ptr<Vec3b>(r)[c][1]=readbuff[i];//green
cout<<"9"<<endl;
                   i++;
                  image.ptr<Vec3b>(r)[c][2]=readbuff[i];//green
cout<<"10"<<endl;
                   i++;
                }
           } 
cout<<"11"<<endl;
*/

         imshow("server_video", image);
         waitKey(20);
        std_msgs::String msg2;
        msg2.data="oh no!";
        pub.publish(msg2);
        loop_rate.sleep();
      }
cout<<"5"<<endl;
     //close(client_fd);
     close(server_fd);

     return 0;
}
