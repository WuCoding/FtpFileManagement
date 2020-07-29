#include "head.h"

int main(int argc,char* argv[]){//server ip port
	ARGS_CHECK(argc,3);

	//绑定监听
	int socketFd=initTcp(argv[1],argv[2]);

	char data[1024];
	int ret;

	int newFd;
	while(1){
		newFd=connectClient(socketFd);
		printf("new socket:%d\n",newFd);

		memset(data,0,sizeof(data));
		ret=recvData(newFd,data);
		ERROR_CHECK(ret,-1,"recvData");
		if(ret==0){
			printf("client is unlink\n");
			return 0;
		}
		printf("%s\n",data);
	}
}




