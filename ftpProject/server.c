#include "head.h"

int main(int argc,char* argv[]){//server ip port
	ARGS_CHECK(argc,3);

	//绑定监听
	int socketFd=initTcp(argv[1],argv[2]);

	char buf[1024];
	int ret;

	//创建epoll
	int eplFd=epoll_create(MAX_EVENTS);
	struct epoll_event* events;
	events=(struct epoll_event*)calloc(MAX_EVENTS,sizeof(struct epoll_event));

	//添加监听socket到epoll
	ret=epollAdd(eplFd,socketFd);
	ERROR_CHECK(ret,-1,"epollAdd");

	//初始化用户链表
	UserList_t userList;
	initUserList(&userList);
	insertUserNode(&userList,"wcy","$6$HKIZ4emv$WjMkFoao4PZ9N7CsrY.fF9z8SBY/yQScok2uwulloz0i4lyI1nqzC8VfdsXQNRzp7ahkBQoZCx0Dg6NYlcHAi1");
	//初始化token链表
	TokenList_t tokenList;
	initTokenList(&tokenList);

	int readFdSize;
	while(1){
		readFdSize=epoll_wait(eplFd,events,MAX_EVENTS,-1);
		ERROR_CHECK(readFdSize,-1,"epoll_wait");
		for(int i=0;i<readFdSize;i++){
			if(events[i].data.fd==socketFd){//有新的链接接入
				int newFd=connectClient(socketFd);
				ERROR_CHECK(newFd,-1,"connectClient");
				
				//进行链接验证-------------------------------------
				//-1 recv发生错误，0 验证失败，
				//1 客户端验证成功 2 下载子线程
				//3 上传子线程
				ret=verifiedClient(newFd,&tokenList,&userList);
				ERROR_CHECK(ret,-1,"verifiedConnection");
				if(ret==0){//验证失败，断开链接
					close(newFd);
				}else if(ret==1){//客户端验证成功加入epoll模型
					ret=epollAdd(eplFd,newFd);
					ERROR_CHECK(ret,-1,"epollAdd");
				}else{//子线程验证成功
					if(ret==2){
						printf("downThread connect\n");
					}else if(ret==3){
						printf("upThread connect\n");
					}
				}
			}else{//有客户发来信息
				int clientFd=events[i].data.fd;
				memset(buf,0,sizeof(buf));
				ret=recvData(clientFd,buf);
				ERROR_CHECK(ret,-1,"recvData");
				if(ret==0){//客户端断开
					printf("client is unlink\n");
					epollDel(eplFd,clientFd);
					close(clientFd);
				}else{
					printf("%s\n",buf);
				}
			}
		}
	}
}
