#include "head.h"

int main(int argc, char* argv[]) {//client ip port
	ARGS_CHECK(argc,3);
	int socketFd;
	socketFd = connectServer(argv[1], argv[2]);
	ERROR_CHECK(socketFd, -1, "connectServer");
	
	int ret;

	//进行链接验证
	
	char token[200];
	char userName[100];
	memset(token,0,sizeof(token));
	memset(userName,0,sizeof(userName));

	printf("username=\n");
	scanf("%s",userName);
	printf("token=\n");
	scanf("%s",token);
	enum clientType linkThread=DOWNTHREAD;
	ret=verifiedServeThread(socketFd,userName,token,linkThread);
	if(ret==0){
		printf("token verified failed\n");
		return 0;
	}else{
		printf("token verified success\n");
		while(1);
	}
}
