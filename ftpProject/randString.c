#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void getRandStr(char result[],int len){
	char randCh[100];
	memset(randCh,0,sizeof(randCh));
	memcpy(randCh,"1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",62);
	int randLen=strlen(randCh);
	srand(time(NULL));
	int chIndex;
	for(int i=0;i<len;i++){
		chIndex=rand()%randLen;
		result[i]=randCh[chIndex];
	}
}

