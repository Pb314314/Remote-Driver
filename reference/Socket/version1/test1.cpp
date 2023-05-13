#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
int main(){
    std::string str = "quit\n";
    char quit[10];
    strcpy(quit,str.c_str());
    char buf[1024];
    while(true){
        fgets(buf,sizeof(buf),stdin);
        /*
        if(!strcmp(quit,buf)){
        std::cout<< "End Sending and disconnecting" <<std::endl;
        //close(sockfd);
        return 1;
        }*/
        cout<< buf[4]<<buf[5] <<endl;
        //scanf("%s",buf);
        //write(sockfd,buf,sizeof(buf));
        //memset(buf,0,sizeof(buf));
    }
    
    //n = read(sockfd,buf,sizeof(buf));
    //printf("------a-------\n");
    //write(STDOUT_FILENO,buf,n);
}        