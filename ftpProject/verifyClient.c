#include "head.h"

int verifiedConnection(int newFd,pTokenList_t pTokenList,pUserList_t pUserList){
	int ret;
	enum clientType linkClientType;
	ret=recvData(newFd,(char*)&linkClientType);
	printf("clientType=%d\n",linkClientType);
	ERROR_CHECK(ret,-1,"recvData");
	if(ret==0){//对端断开，验证失败
		return 0;
	}

	if(linkClientType==NEWCLIENT){//新接入的客户端
		printf("新接入为客户端\n");
		return clientVerified(newFd,pUserList);
	}else{//新接入的是子线程
		;
	}
}

int clientVerified(int newFd,pUserList_t pUserList){
	if(pUserList->size==0){
		return 0;
	}
	int ret;
	pUser_t pUser;
	char userName[100]={0};

	ret=recvData(newFd,userName);
	ERROR_CHECK(ret,-1,"recvData");
	printf("username=%s\n",userName);
	if(ret==0){//对端断开，验证失败
		return 0;
	}
	pUser=findUserName(pUserList,userName);
	if(pUser==NULL){
		printf("user=NULL\n");
	}
	if(pUser){
		printf("该用户存在\n");
		printf("username=%s\npassword=%s\n",pUser->userName,pUser->password);
		return 1;
	}else{//不存在验证失败
		printf("该用户不存在\n");
		return 0;
	}
}

//int threadVerified(int newFd,pTokenList_t pTokenList){
//	;
//}
