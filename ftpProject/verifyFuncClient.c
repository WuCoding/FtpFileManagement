#include "head.h"
//新客户端与服务器链接 成功 1 失败 0
//client--------> server
int verifiedServeClient(int socketFd,char token[],char userName[]){
	int ret;

	char password[200];
	char salt[100];
	char YorN[100];
	char password2[200];
	memset(password,0,sizeof(password));
	memset(salt,0,sizeof(salt));
	memset(YorN,0,sizeof(YorN));
	memset(password2,0,sizeof(password2));

	//初始化客户端类型
	enum clientType linkClientType=NEWCLIENT;

	//输入用户名
	printf("please input your username:\n");
	scanf("%s",userName);

	//发送客户类型
	ret=sendData(socketFd,(char*)&linkClientType,sizeof(enum clientType));
	ERROR_CHECK(ret,-1,"sendData");
	//发送用户名
	ret=sendData(socketFd,userName,strlen(userName));
	ERROR_CHECK(ret,-1,"sendData");

	//验证是否成功
	bool isSuccess;
	ret=recvData(socketFd,(char*)&isSuccess);
	ERROR_CHECK(ret,-1,"recvData");
	if(ret==0){
		printf("server is unlink\n");
		return 0;
	}

	if(isSuccess){//用户名存在
		//接收salt
		ret=recvData(socketFd,salt);
		ERROR_CHECK(ret,-1,"recvData");
		if(ret==0){
			printf("server is unlink\n");
			return 0;
		}

		//输入密码
		printf("please input your password:\n");
		scanf("%s",password);

		//对密码进行加密
		char* encrypted;
		encrypted=crypt(password,salt);	
		//发送加密密文
		ret=sendData(socketFd,encrypted,strlen(encrypted));
		ERROR_CHECK(ret,-1,"sendData");
		//接收token
		ret=recvData(socketFd,token);
		ERROR_CHECK(ret,-1,"recvData");
		if(ret==0){
			printf("password is error\n");
			return 0;
		}
		return 1;
	}else{//用户名不存在
		printf("username is inexistence.if you want to sign in [Y/N]:");
		scanf("%s",YorN);
		while((!strcmp(YorN,"Y"))||(!strcmp(YorN,"y"))){//进行注册
			printf("input your password:\n");
			scanf("%s",password);
			printf("input again:\n");
			scanf("%s",password2);
			if(strcmp(password,password2)){//两次输入的密码不匹配
				printf("the twice input password is not same!\n");
				printf("if you want to sign in again [Y/N]:");
				memset(YorN,0,sizeof(YorN));
				scanf("%s",YorN);
				continue;
			}else{//两次输入密码相同
				//获取随机salt
				ret=getRandSalt(salt,sizeof(salt),8);
				if(ret==-1){
					printf("getRandSalt is error\n");
					return 0;
				}
				//对密码进行加密
				char* encrypted;
				encrypted=crypt(password,salt);
				//发送加密密码
				ret=sendData(socketFd,encrypted,strlen(encrypted));
				ERROR_CHECK(ret,-1,"sendData");

				//接收token
				ret=recvData(socketFd,token);
				ERROR_CHECK(ret,-1,"recvData");
				if(ret==0){//对端关闭 验证失败
					return 0;
				}
				return 1;
			}
		}
		return 0;
	}
}

//验证失败 0 验证成功 1
//thread---------------->server
int verifiedServeThread(int socketFd,char userName[],char token[],enum clientType linkThread){
	int ret;

	//发送类型
	ret=sendData(socketFd,(char*)&linkThread,sizeof(enum clientType));
	ERROR_CHECK(ret,-1,"sendData");

	//发送用户名
	ret=sendData(socketFd,userName,strlen(userName));
	ERROR_CHECK(ret,-1,"sendData");

	//发送token
	ret=sendData(socketFd,token,strlen(token));
	ERROR_CHECK(ret,-1,"sendData");

	//接收验证结果
	bool isSuccess;
	ret=recvData(socketFd,(char*)&isSuccess);
	ERROR_CHECK(ret,-1,"recvData");
	if(ret==0){//服务端断开
		printf("server is unlink\n");
		return 0;
	}else{
		return 1;
	}
}
