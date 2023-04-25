//Includes all the headers necessary to use the most common public pieces of the ROS system.
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

using namespace cv;
using namespace std;
 
//Store all constants for image encodings in the enc namespace to be used later.  
//namespace enc = sensor_msgs::image_encodings; 

void image_socket(Mat inImg);
static Mat image1;
static int imgWidth, imgHeight;

static int image_num = 1;
 
#define BUFFER_SIZE 50000
#define NAME_SIZE 20
static int sockfd;
 
//static ros::Publisher capture;  
//This function is called everytime a new image_info message is published
void camInfoCallback(const sensor_msgs::CameraInfo & camInfoMsg)
{
 //Store the image width for calculation of angle
 imgWidth = camInfoMsg.width;
//cout<<imgWidth<<endl;
 imgHeight = camInfoMsg.height;
//cout<<imgHeight<<endl;
}
 
//This function is called everytime a new image is published
void imageCallback(const sensor_msgs::ImageConstPtr& original_image)
{
   //Convert from the ROS image message to a CvImage suitable for working with OpenCV for processing
   cv_bridge::CvImagePtr cv_ptr;
    try
    {
      //Always copy, returning a mutable CvImage  
//OpenCV expects color images to use BGR channel order.  
cv_ptr = cv_bridge::toCvCopy(original_image, sensor_msgs::image_encodings::BGR8);
 }
catch (cv_bridge::Exception& e)
{
//if there is an error during conversion, display it  
ROS_ERROR("tutorialROSOpenCV::main.cpp::cv_bridge exception: %s", e.what());
return;
}
image_socket(cv_ptr->image);
 
}
 
void image_socket(Mat inImg)
{
cout<<"1    "<<inImg.rows<<"2    "<<inImg.cols<<endl;
imshow("inImg1", inImg);//显示图片
waitKey(20);
char **data = new char*[3];
for (int i = 0; i < 3; i++)
{
data[i] = new char[inImg.rows*inImg.cols];
memset(data[i],0, sizeof(char)*inImg.rows*inImg.cols);
}
for (int i = 0; i < inImg.rows; i++)
{
for (int j = 0; j < inImg.cols; j++)
{
//uchar *p = SrcImg.ptr<uchar>(i, j);
data[0][j + i*inImg.cols] = inImg.ptr<Vec3b>(i)[j][0];
data[1][j + i*inImg.cols] = inImg.ptr<Vec3b>(i)[j][1];
data[2][j + i*inImg.cols] = inImg.ptr<Vec3b>(i)[j][2];
}
}
for (int i = 0; i < 3; i++)
{
int n =write(sockfd, data[i], inImg.rows* inImg.cols);
if (n < 0)
 perror("ERROR writing to socket");
}
//释放
for (int i = 0; i < 3; i++)
{
delete []data[i];
}
delete[]data;


/**
waitKey(1);
 if( inImg.empty() )
{
ROS_INFO("Camera image empty");
return;//break;
}
 resize(inImg,image1,Size(imgWidth/10,imgHeight/10),0,0,CV_INTER_LINEAR);

imshow("image12", image1);
waitKey(1);
 char sendline[BUFFER_SIZE];
int i;
 int j;
 i=0;
 sendline[i]=image1.rows;
 i++;
 sendline[i]=image1.cols;
 i++;
 //将图像像素值存到sendline中
 for (int r=0;r<image1.rows;r++)
{
 for(int c=0;c<image1.cols;c++)
 {
 sendline[i]=image1.ptr<Vec3b>(r)[c][0];//green
 i++;
sendline[i]=image1.ptr<Vec3b>(r)[c][1];//blue
 i++;
 sendline[i]=image1.ptr<Vec3b>(r)[c][2];//red
 i++;
}
 }

//发送图像及名字到服务器
int n = write(sockfd, sendline, image1.total()*image1.channels()+2);
 if (n < 0)
 perror("ERROR writing to socket");
**/
/*
      if('w'==name[0])//如果第一个是w，接受服务器训练结果的名字
      {
          n = read(sockfd, recvline, NAME_SIZE,MSG_WAITALL); 
          cout<< recvline<<endl;
          cout<<n<<endl;
      }
       // n = read(sockfd, recvline, MAXLINE);  
       // write(STDOUT_FILENO, recvline, n); 
     // n = write(sockfd,image1.data,image1.total()*image1.channels());
 
      image_num++;
      break;
*/


//image1.release();
}
 

 //This is ROS node to track the destination image

int main(int argc, char **argv)
{
ros::init(argc, argv, "image_socket");
ROS_INFO("-----------------");

struct sockaddr_in serv_addr;
//创建socket
if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
{
perror("Socket failed!\n");
exit(1);
}
printf("Socket id = %d\n",sockfd);
//设置sockaddr_in 结构体中相关参数
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(10000);
serv_addr.sin_addr.s_addr = inet_addr("192.168.0.104");
//inet_pton(AF_INET, servInetAddr, &serv_addr.sin_addr);
bzero(&(serv_addr.sin_zero), 8);
//调用connect 函数主动发起对服务器端的连接
 if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))== -1)
 {
 perror("Connect failed!\n");
exit(1);
}
 printf("welcome\n");

ros::NodeHandle nh;
 image_transport::ImageTransport it(nh);
 
image_transport::Subscriber sub = it.subscribe("/usb_cam/image_raw", 1, imageCallback);
ros::Subscriber camInfo= nh.subscribe("/usb_cam/camera_info", 1, camInfoCallback);
ros::Rate loop_rate(50);
 
while (ros::ok())
{
 ros::spinOnce();
 loop_rate.sleep();
}
 
 close(sockfd);
//ROS_INFO is the replacement for printf/cout.
ROS_INFO("No error.");
return 0;
 
}
