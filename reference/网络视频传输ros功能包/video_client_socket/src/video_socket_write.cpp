
//原理：客户端：利用OpenCV调取摄像头 --> 创建socket框架 --> 利用OpenCV将视频每一帧编码 --> 发送出去。
//      服务器端：创建socket框架 --> 接收到图片的字节序 --> 利用OpenCV的解码函数将每一帧图片解码 --> 显示图片或者说视屏


//！！！！因为ros里面用OpenCV 所以我就把视屏网络传输的程序写到ros框架中去了，如果只需要视屏网络传输的功能，可以忽略ros的存在。


//-------------------- ROS所需头文件-----------------------
//Includes all the headers necessary to use the most common public pieces of the ROS system.
#include<ros/ros.h>
//Use image_transport for publishing and subscribing to images in ROS
#include<image_transport/image_transport.h>
//Use cv_bridge to convert between ROS and OpenCV Image formats
#include<cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//-------------------- ROS所需头文件-----------------------
//

//----------------opencv的头文件 用于视屏编码和解码的--------
//Include headers for OpenCV Image processing
//Include headers for OpenCV GUI handling
#include<opencv-3.3.1-dev/opencv2/imgproc/imgproc.hpp>
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
#include<vector>//用于储存编码后的视屏数据
#include<sys/ioctl.h>
//----------------socket网络编程所需头文件-------------------


using namespace cv;
using namespace std;

static int sockfd;

int main(int argc, char **argv)
{
    //---------ros的框架-----------
    ros::init(argc, argv, "image_socket");
    ROS_INFO("-----------------");
    //---------ros的框架-----------

    //---------socket框架----------
    struct sockaddr_in serv_addr;
    //创建socket
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("Socket failed!\n");
        exit(1);
    }
    printf("Socket id = %d\n",sockfd);
    //设置sockaddr_in 结构体中相关参数
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(10000);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.101");
    //inet_pton(AF_INET, servInetAddr, &serv_addr.sin_addr);
    bzero(&(serv_addr.sin_zero), 8);
    //---------socket框架----------
    
    
    //调用connect 函数主动发起对服务器端的连接
    /*
    if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))== -1)
    {
        perror("Connect failed!\n");
        exit(1);
    }
    printf("welcome\n");
*/
    ros::NodeHandle nh;
    //image_transport::ImageTransport it(nh);

    //image_transport::Subscriber sub = it.subscribe("/usb_cam/image_raw", 1, imageCallback);
    //ros::Subscriber camInfo= nh.subscribe("/usb_cam/camera_info", 1, camInfoCallback);
    //ros::Rate loop_rate(100);


    VideoCapture capture(0);//利用OpenCV的函数调用摄像头 0号usb摄像头
    Mat image;

    while (1)
    {
        //下面几行是把摄像头捕捉的视屏显示出来
        if (!capture.isOpened())
            return 0;

        capture >> image;
        imshow("client_send_video", image);
        waitKey(3);
        if (image.empty())
            continue;
/*
方法零：该方法可不可行我忘记了。
        vector<uchar> data_encode;
        vector<int>param =vector<int>(2);
        param[0]=CV_IMWRITE_JPEG_QUALITY;
        param[1]=90;//视屏按照%90压缩
        imencode(".jpg", image, data_encode,param);
        int nSize=data_encode.size();
        char buff[1];
        buff[0]=nSize;
        write(sockfd, buff, 1);//试图想先发送图像大小过去，该方法行不行，我忘记了。
        if (write(sockfd, &data_encode[0], nSize) < 0)
        {
            printf("send image1 error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
*/
/*
        //方法一：可行
        vector<uchar> data_encode;
        vector<int>quality =vector<int>(2);
        quality[0]=CV_IMWRITE_JPEG_QUALITY;
        quality[1]=90;
        //vector<int>quality;
        //quality.push_back(CV_IMWRITE_JPEG_QUALITY);
        //quality.push_back(50);//进行50%的压缩
        imencode(".jpg", image, data_encode,quality);//将图像编码
        char encodeImg[655350];
        int nSize=data_encode.size();
        for (int i = 0; i < nSize; i++)
        {
            encodeImg[i] = data_encode[i];
        }

        if (write(sockfd, encodeImg, nSize) < 0)
        {//完美，点个赞
            printf("send image1 error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }

        memset(encodeImg, 0, sizeof(encodeImg));
*/
        //方法二：可行
        //下面几行是利用opencv的编码函数imencode将每一帧图片编码存放到data_encode这个vector里面
        vector<uchar> data_encode;
        vector<int>quality =vector<int>(2);
        quality[0]=CV_IMWRITE_JPEG_QUALITY;//编码格式
        quality[1]=50;//图片压缩比例：50为压缩为50%
        imencode(".jpg", image, data_encode,quality);//将图像编码
        //char encodeImg[655350];
        int nSize=data_encode.size();
        cout<<"图像的数据大小： "<<nSize<<endl;
       // write(sockfd, reinterpret_cast<uchar*>(&nSize), sizeof(int));
       sendto(sockfd,&data_encode[0],nSize,0,(struct sockaddr*)&serv_addr, sizeof(serv_addr));//这种方法比方法一可以省去图像内存的拷贝，给自己点个赞。
       /*
        if (write(sockfd, &data_encode[0], nSize) < 0)
        {
            printf("send image1 error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
        */
        //memset(encodeImg, 0, sizeof(encodeImg));
    }
    close(sockfd);//结束之后关闭这个网络套接字
    ROS_INFO("No error.");//ros的打印函数
    return 0;

}
