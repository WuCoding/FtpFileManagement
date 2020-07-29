#include "head.h"
//socket()  bind()  listern()
int initTcp(char* ip,char* port){
	int socketFd;
	socketFd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serAddr;
	bzero(&serAddr,sizeof(serAddr));
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(atoi(port));
	serAddr.sin_addr.s_addr=inet_addr(ip);
	bind(socketFd,(struct sockaddr*)&serAddr,sizeof(struct sockaddr));
	listen(socketFd,10);

	printf("listening...\n");

	return socketFd;
}
//accept()
int connectClient(int sfd){
	int newFd;
	struct sockaddr_in clientAddr;
	int addrLen;
	memset(&clientAddr,0,sizeof(struct sockaddr_in));
	addrLen=sizeof(struct sockaddr_in);
	newFd=accept(sfd,(struct sockaddr*)&clientAddr,&addrLen);
	ERROR_CHECK(newFd,-1,"accept");
	printf("%s %d success connect\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
	return newFd;
}

//socket()  connect()
int connectServer(char* ip,char* port){
	int socketFd;
	int ret;
	socketFd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serAddr;
	memset(&serAddr,0,sizeof(struct sockaddr_in));
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=htons(atoi(port));
	serAddr.sin_addr.s_addr=inet_addr(ip);
	ret=connect(socketFd,(struct sockaddr*)&serAddr,sizeof(struct sockaddr));
	ERROR_CHECK(ret,-1,"connect");
	printf("connecting...\n");
	return socketFd;
}

