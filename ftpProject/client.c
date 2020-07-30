#include "head.h"

int main(int argc, char* argv[]) {//client ip port
	ARGS_CHECK(argc,3);
	int socketFd;
	socketFd = connectServer(argv[1], argv[2]);
	ERROR_CHECK(socketFd, -1, "connectServer");
	
	//用户验证
	int ret;
	enum clientType linkClientType=NEWCLIENT;
	char userName[100];
	memset(userName,0,sizeof(userName));
	printf("please input your username:");
	scanf("%s",userName);
	//发送客户类型
	ret=sendData(socketFd,(char*)&linkClientType,sizeof(enum clientType));
	ERROR_CHECK(ret,-1,"sendData");
	ret=sendData(socketFd,userName,strlen(userName));
	ERROR_CHECK(ret,-1,"sendData");


	char buf[1024];
	
	int eplFd=epoll_create(MAX_EVENTS);
	struct epoll_event *events;
	events=(struct epoll_event*)calloc(MAX_EVENTS,sizeof(struct epoll_event));

	ret=epollAdd(eplFd,STDIN_FILENO);
	ERROR_CHECK(ret,-1,"epollAdd");
	ret=epollAdd(eplFd,socketFd);
	ERROR_CHECK(ret,-1,"epollAdd");
	int readFdSize;

	while(1){
		readFdSize=epoll_wait(eplFd,events,MAX_EVENTS,-1);
		for(int i=0;i<readFdSize;i++){
			if(events[i].data.fd==socketFd){//服务器有消息
				memset(buf,0,sizeof(buf));
				ret=recvData(socketFd,buf);
				ERROR_CHECK(ret,-1,"recvData");
				if(ret==0){//服务器断开链接
					printf("server is unlink\n");
					return 0;
				}
				printf("%s\n",buf);
			}else if(events[i].data.fd==STDIN_FILENO){//标准输入有消息
				memset(buf,0,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf)-1);
				ERROR_CHECK(ret,-1,"read");
				ret=sendData(socketFd,buf,strlen(buf)-1);
				ERROR_CHECK(ret,-1,"sendData");
			}
		}
	}
}
