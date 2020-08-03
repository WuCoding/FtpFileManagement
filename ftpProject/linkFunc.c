#include "head.h"

int initUserList(pUserList_t pUserList){
	pUserList->size=0;
	pUserList->head=NULL;
	pUserList->tail=NULL;

	return 0;
}

int insertUserNode(pUserList_t pUserList,char userName[],char password[]){
	pUser_t pNewNode=(pUser_t)malloc(sizeof(User_t));
	ERROR_CHECK(pNewNode,NULL,"malloc");
	memset(pNewNode,0,sizeof(User_t));

	strcpy(pNewNode->userName,userName);
	strcpy(pNewNode->password,password);

	if(pUserList->size==0){
		pUserList->head=pNewNode;
		pUserList->tail=pNewNode;
		pUserList->size=1;
	}else{
		pUserList->tail->pNext=pNewNode;
		pUserList->tail=pNewNode;
		(pUserList->size)++;
	}

	return 0;
}

int initTokenList(pTokenList_t pTokenList){
	pTokenList->size=0;
	pTokenList->head=NULL;
	pTokenList->tail=NULL;

	return 0;
}

int insertTokenNode(pTokenList_t pTokenList,char userName[],char token[]){
	pToken_t pNewNode=(pToken_t)malloc(sizeof(Token_t));
	ERROR_CHECK(pNewNode,NULL,"malloc");
	memset(pNewNode,0,sizeof(Token_t));

	strcpy(pNewNode->userName,userName);
	strcpy(pNewNode->token,token);

	if(pTokenList->size==0){
		pTokenList->head=pNewNode;
		pTokenList->tail=pNewNode;
		pTokenList->size=1;
	}else{
		pTokenList->tail->pNext=pNewNode;
		pTokenList->tail=pNewNode;
		(pTokenList->size)++;
	}
	return 0;
}

pToken_t findTokenNode(pTokenList_t pTokenList,char* userName){
	if(pTokenList->size==0){
		return NULL;
	}
	pToken_t pCurNode=pTokenList->head;
	while(pCurNode){
		if(!(strcmp(pCurNode->userName,userName))){
			return pCurNode;
		}
		pCurNode=pCurNode->pNext;
	}
	return NULL;
}

#if 0
int printUserList(pUserList_t pUserList){
	pUser_t curNode=pUserList->head;
	while(curNode){
		printf("%s\n",curNode->userName);
		printf("%s\n",curNode->password);

		curNode=curNode->pNext;
	}
}
#endif

pUser_t findUserName(pUserList_t pUserList,char* userName){
	
	if(pUserList->size==0){
		return NULL;
	}
	pUser_t pCurNode=pUserList->head;
	while(pCurNode){
		if(!(strcmp(pCurNode->userName,userName))){//匹配成功
			return pCurNode;
		}
		pCurNode=pCurNode->pNext;
	}
	return NULL;
}

#if 0
int test(){
	UserList_t userList;
	initUserList(&userList);
	pUser_t pTest;
	char userName[100]={0};
	char password[200]={0};
	for(int i=0;i<5;i++){
		memset(userName,0,sizeof(userName));
		memset(password,0,sizeof(password));
		printf("username:\n");
		scanf("%s",userName);
		printf("password\n");
		scanf("%s",password);

		insertUserNode(&userList,userName,password);
	}
	if(findUserName(&userList,"123")){
		printf("查找成功\n");
	}

	pTest=findUserName(&userList,"123");


	printf("%s\n%s\n",pTest->userName,pTest->password);

	//printUserList(&userList);
}


int main(){
	test();
}
#endif
