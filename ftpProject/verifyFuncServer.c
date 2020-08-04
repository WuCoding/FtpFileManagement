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

			//查看token链表中有无该用户
			pToken_t pTokenNode;
			pTokenNode=findTokenNode(pTokenList,userName);
			if(!pTokenNode){//token链表中不存在该用户
				//创建token
				uuid_t uu;
				uuid_generate(uu);
				uuid_generate_random(uu);
				uuid_unparse_upper(uu,token);
				//插入token节点
				ret=insertTokenNode(pTokenList,userName,token);
				ERROR_CHECK(ret,-1,"insertTokenNode");
				//发送token
				ret=sendData(newFd,token,strlen(token));
				ERROR_CHECK(ret,-1,"sendData");
			}else{
				//发送token
				ret=sendData(newFd,pTokenNode->token,strlen(pTokenNode->token));
				ERROR_CHECK(ret,-1,"sendData");
			}

			return 1;
		}else{
			printf("password is error\n");
			return 0;
		}
	}else{//用户不存在，进行注册
		isSuccess=FALSE;
		ret=sendData(newFd,(char*)&isSuccess,sizeof(bool));
		ERROR_CHECK(ret,-1,"sendData");
		printf("该用户不存在\n");

		//接收加密密码
		ret=recvData(newFd,password);
		ERROR_CHECK(ret,-1,"recvData");
		if(ret==0){//对端关闭验证失败
			return 0;
		}
		//插入用户链表
		ret=insertUserNode(pUserList,userName,password);
		ERROR_CHECK(ret,-1,"insertUserNode");
		//创建token
		uuid_t uu;
		uuid_generate(uu);
		uuid_generate_random(uu);
		uuid_unparse_upper(uu,token);
		//插入token链表
		ret=insertTokenNode(pTokenList,userName,token);
		//发送token
		ret=sendData(newFd,token,strlen(token));
		ERROR_CHECK(ret,-1,"sendData");
		return 1;
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


