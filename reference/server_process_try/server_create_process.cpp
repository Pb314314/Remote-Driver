#include <iostream>
#include<unistd.h>
#include <string>
using namespace std;
int main(int argc, char** argv){
    cout<<"让我试一试"<<endl;
    if(argc!= 3){
        cout<<"The input should be: Sender 8080"<<endl;
        exit(1);
    }
    string process_name = argv[1];
    string arg = argv[2];
    cout<<"程序开始执行啦，成为进程"<<endl;
	//需要执行某程序模块  a.out     a.out  a b c  
	//char* argv[5]={"ls","-l","/",NULL};
    char* process[3] = {argv[1],argv[2]};
    //char* process[5]={"ls","-l"};
	execv("/bin/ls",process);//argv[0]  argv[1] argv[2]	
	int i=0;
	for(i=0;i<100;i++)
	{
		cout<<i<<endl;
	}
	cout<<"进程结束执行"<<endl;

}