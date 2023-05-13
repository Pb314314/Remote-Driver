#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
//定义IP
#define SERVER_IP   "127.0.0.1"
//定义端口
#define SERVER_PORT  8080
 
int main(void)
{
 
    int lfd,cfd;
    char str[INET_ADDRSTRLEN];
    //创建socket套接字
    lfd = socket(AF_INET,SOCK_STREAM,0);  //TCP  ipv4
    //绑定 IP(server) 和 端口号(监听)
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //或者 INADDR_ANY:提供任意一个本地有效IP
    bind(lfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    //监听 设置最大监听数目 128
    listen(lfd,128);
    //等待连接
    struct sockaddr_in clientAddr;
    socklen_t clientAddr_len = sizeof(clientAddr);
    cfd = accept(lfd,(struct sockaddr *)&clientAddr,&clientAddr_len); 
    //数据交换
    int n,i=0;
    char buf[1024] = {0};
    while(1)
    {     
        n = read(cfd,buf,sizeof(buf));
        if(n == 0)  //有客户端断开连接
        {
            printf("有客户端断开连接\n");
        }
        if(n < 0)
        {
            printf("aaaaaaaa\n");
        }
        // inet_ntop(AF_INET,&clientAddr.sin_addr,str,sizeof(str));
        // ntohs(clientAddr.sin_port);
        printf("已收到第%d次数据:%s\n",i++,buf); 
        //sleep(2);
        write(cfd,buf,n);
    }   
 
    close(cfd);
    close(lfd);
    return 0;
}

