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


void split(char* str, char delimiter, char** substrings, int& num_substrings);
void get_path(char* base_path, char** args_vec, const char* result);

int main(int argc, char** argv)
{
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

    if(!fork()){ //fork=0,如果是子进程，进入
        //对于fork可以再多理解一下。 当fork运行就会出现一个新的自进程。对于原来的父进程fork return子进程的id；对于子进程fork会return0；
        //我倒是可以让所有的自进程进入这里的一个函数进行首发操作
        std::cout<< "Another CLients!" <<std::endl;
        while(true){
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
            char buff_try[] = "ls -a -l -a";
            printf("recv msg from client: %s\n", buff);
            char* args_vec[10];
            int length;
            split(buff, ' ', args_vec, length);
            args_vec[length] = NULL;
            std::cout<< length <<std::endl;
            for(int i=0;i<length;i++){
                std::cout<<"!!:" << args_vec[i]<<std::endl;
            }
            const char* command_path = NULL;
            char base_path[] = "/bin";
            get_path(base_path,args_vec,command_path);
	    char command_path2[] = "/bin/ls";
            std::cout<< "432432" <<"!!" <<std::endl;
            
            execv(command_path2,args_vec);
            
        }
        
        //if(send(connect_fd, "Receive Your Love!\n", 26,0) == -1) perror("send error");//给自进程发送系信息
        //close(connect_fd);
        //exit(0);
    }
    //所有的自进程都不会出来，父进程才能到这里。父进程不做信息处理工作，只处理accept连接
}
    close(socket_fd);
}

//args: "ls -l"; args_vec:{"ls","-l","null"}
//str: "ls -l" delimiter: " "
void split(char* str, char delimiter, char** substrings, int& num_substrings) {
	num_substrings = 0;
	char* token = strtok(str, &delimiter);
	while (token != NULL && num_substrings < 10) {
		substrings[num_substrings++] = token;
		token = strtok(NULL, &delimiter);
	}
}

//
//
void get_path(char* base_path, char** args_vec, const char* result){
	//result = NULL;
	std::string file_name = std::string(args_vec[0]);
	std::string temp = std::string(base_path ) + "/"+std::string(file_name);
	result = temp.c_str();
    std::cout<<"666:" << result<<std::endl;
    return;
}
