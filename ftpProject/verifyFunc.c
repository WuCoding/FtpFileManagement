#include "head.h"

//验证失败返回 0 新客户端验证成功 1 下载子线程验证成功 2 上传子线程验证成功 3
//server <------------client
int verifiedClient(int newFd,pTokenList_t pTokenList,pUserList_t pUserList){
	int ret;
	
	//接受类型
	enum clientType linkClientType;
	ret=recvData(newFd,(char*)&linkClientType);
	ERROR_CHECK(ret,-1,"recvData");
	if(ret==0){//对端断开，验证失败
		return 0;
	}

	//进行验证
	if(linkClientType==NEWCLIENT){//新接入的客户端
		return clientLinkVerified(newFd,pTokenList,pUserList);
	}else if(linkClientType==DOWNTHREAD){//新接入的是下载子线程
		ret=threadLinkVerified(newFd,pTokenList);
		if(ret==0){
			return 0;
		}else{
			return 2;
		}
	}else{//新接入的是上传子线程
		ret=threadLinkVerified(newFd,pTokenList);
		if(ret==0){
			return 0;
		}else{
			return 3;
		}
	}
}

//验证失败 0 验证成功 1
//server <-----------------------client
int clientLinkVerified(int newFd,pTokenList_t pTokenList,pUserList_t pUserList){
	if(pUserList->size==0){
		return 0;
	}
	int ret;
	pUser_t pUser;
	char userName[100];
	char password[200];
	char salt[100];
	char token[200];

	memset(userName,0,sizeof(userName));
	memset(password,0,sizeof(password));
	memset(salt,0,sizeof(salt));
	memset(token,0,sizeof(token));

	//接收用户名
	ret=recvData(newFd,userName);
	ERROR_CHECK(ret,-1,"recvData");
	if(ret==0){//对端断开，验证失败
		printf("client is unlink\n");
		return 0;
	}

	//验证用户名
	bool isSuccess;
	pUser=findUserName(pUserList,userName);
	if(pUser){
		printf("该用户存在\n");
		printf("username=%s\npassword=%s\n",pUser->userName,pUser->password);

		
		
		//发送验证成功
		isSuccess=TRUE;
		ret=sendData(newFd,(char*)&isSuccess,sizeof(bool));
		ERROR_CHECK(ret,-1,"sendData");

		//发送salt
		ret=getSaltFromPassword(salt,sizeof(salt),pUser->password,strlen(pUser->password));
		if(ret==-1){
			printf("get salt is error\n");
			return 0;
		}

		ret=sendData(newFd,salt,strlen(salt));
		ERROR_CHECK(ret,-1,"sendData");

		//接收密码
		ret=recvData(newFd,password);
		ERROR_CHECK(ret,-1,"recvData");
		if(ret==0){//客户端断开
			printf("client is unlink\n");
			return 0;
		}

		//进行密码验证
		if(!(strcmp(pUser->password,password))){//验证成功
			printf("password verified success\n");
			
			//创建token
			uuid_t uu;
			uuid_generate(uu);
			uuid_generate_random(uu);
			uuid_unparse_upper(uu,token);
			//发送token
			ret=sendData(newFd,token,strlen(token));
			ERROR_CHECK(ret,-1,"sendData");

			//插入token节点
			ret=insertTokenNode(pTokenList,userName,token);
			ERROR_CHECK(ret,-1,"insertTokenNode");

			return 1;
		}else{
			printf("password is error\n");
			return 0;
		}
	}else{//不存在验证失败
		isSuccess=FALSE;
		ret=sendData(newFd,(char*)&isSuccess,sizeof(bool));
		ERROR_CHECK(ret,-1,"sendData");
		printf("该用户不存在\n");
		
		return 0;
	}
}

//验证失败 0 验证成功 1
//server <-------------- thread
int threadLinkVerified(int newFd,pTokenList_t pTokenList){
	int ret;
	
	char userName[100];
	char token[200];
	memset(userName,0,sizeof(userName));
	memset(token,0,sizeof(token));

	//接收用户名
	ret=recvData(newFd,userName);
	ERROR_CHECK(ret,-1,"recvData");
	if(ret==0){//对端断开链接
		printf("client thread is unlink\n");
		return 0;
	}
	//接收token
	ret=recvData(newFd,token);
	ERROR_CHECK(ret,-1,"recvData");
	if(ret==0){//对端断开链接
		printf("client thread is unlink\n");
		return 0;
	}

	bool isSuccess;
	pToken_t pTokenNode;
	pTokenNode=findTokenNode(pTokenList,userName);
	if(pTokenNode){//存在该token节点
		if(!(strcmp(pTokenNode->token,token))){//token验证成功
			printf("verified token\n");

			isSuccess=TRUE;
			ret=sendData(newFd,(char*)&isSuccess,sizeof(bool));
			ERROR_CHECK(ret,-1,"sendData");

			return 1;
		}else{//token验证失败
			printf("not verified token\n");
			return 0;
		}
	}else{//不存在该token节点
		printf("cant find this token node\n");
		return 0;
	}
}

//新客户端与服务器链接 成功 1 失败 0
//client--------> server
int verifiedServeClient(int socketFd,char token[],char userName[]){
	int ret;

	char password[200];
	char salt[100];
	memset(password,0,sizeof(password));
	memset(salt,0,sizeof(salt));
	
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
		printf("username is inexistence\n");
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
