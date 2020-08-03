#include "head.h"

int getRandSalt(char salt[],size_t saltLen,size_t len){
	if((len+5)>saltLen){
		return -1;
	}
	char randCh[100];
	memset(randCh,0,sizeof(randCh));
	memcpy(randCh,"1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",62);
	memcpy(salt,"$6$",3);
	int randLen=strlen(randCh);
	srand(time(NULL));
	int chIndex;
	int i;
	for(i=3;i<len+3;i++){
		chIndex=rand()%randLen;
		salt[i]=randCh[chIndex];
	}
	salt[i]='$';
	return 0;
}

int getSaltFromPassword(char salt[],size_t saltLen,char password[],size_t passwordLen){
	int amount=3;
	int index=0;
	while(amount){
		if(((index+2)>saltLen)||((index+1)>passwordLen)){//访问越界
			return -1;
		}
		salt[index]=password[index];
		if(password[index]=='$'){
			amount--;
		}
		index++;
	}
	return 0;
}

