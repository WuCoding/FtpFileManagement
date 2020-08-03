#ifndef __HEAD_H__
#define __HEAD_H__
#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/select.h>
#include<sys/time.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<errno.h>
#include<signal.h>
#include<string.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/epoll.h>
#include<sys/uio.h>
#include<uuid/uuid.h>
//#include<mysql/mysql.h>

#define ARGS_CHECK(argc,num) {if(num!=argc){printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName) {if(retVal==ret){printf("error=%d\n",errno);perror(funcName);return -1;}}
#define MAX_EVENTS 1024
typedef struct{
	int dataLen;
	char data[1024];
}Train_t,*pTrain_t;

//客户端类型
enum clientType{
	NEWCLIENT,DOWNTHREAD,UPTHREAD
};

//用户名和加密密码
typedef struct user_node{
	char userName[100];
	char password[200];
	struct user_node *pNext;
}User_t,*pUser_t;
typedef struct user_list{
	pUser_t head,tail;
	size_t size;
}UserList_t,*pUserList_t;

//用户名和token
typedef struct token_node{
	char token[200];
	char userName[100];
	struct token_node *pNext;
}Token_t,*pToken_t;
typedef struct token_list{
	pToken_t head,tail;
	size_t size;
}TokenList_t,*pTokenList_t;

//bool类型
typedef enum BOOL{
	FALSE,TRUE
}bool;

//socket() bind() listen()
int initTcp(char* ip,char* port);
//accept()
int connectClient(int socketFd);
//socket() connect()
int connectServer(char* ip,char* port);

//recv()
int recvCycle(int fd,char* buf,int dataLen);
//recvCycle()
int recvData(int fd,char* buf);
//send()
int sendData(int fd,char* buf,int len);

//epoll_ctl()
int epollAdd(int eplFd,int fd);
//epoll_ctl()
int epollDel(int eplFd,int fd);

//random string
void getRandStr(char result[],int len);

//userlist
int initUserList(pUserList_t pUserList);
int insertUserNode(pUserList_t pUserList,char userName[],char password[]);
pUser_t findUserName(pUserList_t pUserList,char* userName);
//tokenlist
int initTokenList(pTokenList_t pTokenList);
int insertTokenNode(pTokenList_t pTokenList,char userName[],char token[]);
pToken_t findTokenNode(pTokenList_t pTokenList,char* userName);

//verified
int verifiedClient(int newFd,pTokenList_t pTokenList,pUserList_t pUserList);
int clientLinkVerified(int newFd,pTokenList_t pTokenList,pUserList_t pUserList);
int threadLinkVerified(int newFd,pTokenList_t pTokenList);
int verifiedServeClient(int socketFd,char token[],char userName[]);
int verifiedServeThread(int socketFd,char userName[],char token[],enum clientType linkThread);

//salt.c
int getRandSalt(char salt[],size_t saltLen,size_t len);
int getSaltFromPassword(char salt[],size_t saltLen,char password[],size_t passwordLen);
#endif
