#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
 
//定义IP
#define SERVER_IP   "127.0.0.1"
// //定义端口

#define SERVER_PORT  8080
 
int main(void)
{
 
    int sockfd;
    //创建套接字  TCP ipv4
    sockfd = socket(AF_INET,SOCK_STREAM,0); 
    //连接
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET,SERVER_IP,&serverAddr.sin_addr);
    connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    //数据交换
    char buf[1024] = {0};
    int i=0,n=0;
    while(1)
    {
        //memset(buf,0,sizeof(buf));
        fgets(buf,sizeof(buf),stdin);
        //scanf("%s",buf);
        write(sockfd,buf,sizeof(buf));
        memset(buf,0,sizeof(buf));
        n = read(sockfd,buf,sizeof(buf));
        //printf("------a-------\n");
        write(STDOUT_FILENO,buf,n);
    }
 
    close(sockfd);
 
    return 0;
}

