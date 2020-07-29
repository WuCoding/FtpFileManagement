#include "head.h"

int main(int argc,char* argv[]){//client ip port
	ARGS_CHECK(argc,3);
	int socketFd;
	socketFd=connectServer(argv[1],argv[2]);
	ERROR_CHECK(socketFd,-1,"connectServer");
	printf("socket:%d\n",socketFd);
	
	char buf[1024];
	int retRead,retSend;
	while(1){
	//	memset(buf,0,sizeof(buf));
	//	retRead=read(STDIN_FILENO,buf,sizeof(buf)-1);
	//	printf("bufsize=%d\n",retRead);
	//	write(STDOUT_FILENO,buf,ret+1);


		//retSend=send(socketFd,buf,retRead-1,0);
		memset(buf,0,sizeof(buf));
		retRead=recvData(socketFd,buf);
		ERROR_CHECK(retRead,-1,"recvData");
		if(retRead==0){
			printf("server is unlink\n");
			return 0;
		}
		printf("%d %s\n",retRead,buf);
	
	}
}

