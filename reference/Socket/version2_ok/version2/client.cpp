/* File Name: client.c */
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
#define MAXLINE 4096

int main(int argc, char** argv)
{
int    sockfd, n,rec_len;
char    recvline[4096], sendline[4096];
char    buf[MAXLINE];
struct sockaddr_in servaddr;
std::string str = "quit\n";
char quit[10];
strcpy(quit,str.c_str());
if( argc != 2){
    printf("usage: ./client <ipaddress>\n");
    exit(0);
}

if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    exit(0);
}

memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(8000);
if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
    printf("inet_pton error for %s\n",argv[1]);
    exit(0);
}

if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
}

printf("send msg to server: \n");
while(true){
    std::cout<< "send another message" <<std::endl;
    fgets(sendline, 4096, stdin);
    //发送信息
    std::cout<< sendline <<std::endl;
    if(send(sockfd, sendline, strlen(sendline), 0) < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    if(!strcmp(quit,sendline)){
        std::cout<< "End Sending and disconnecting" <<std::endl;
        close(sockfd);
        exit(0);
    }
    //接受信息
    /*
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1){
        perror("recv error");
        exit(1);
    }
    buf[rec_len]  = '\0';
    //std::cout<< sizeof(buf) <<std::endl;
    printf("Received : %s ",buf);*/
}
close(sockfd);
exit(0);
}

