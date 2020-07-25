#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void getRandStr(char result[],int len){
	char randCh[]="1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int randLen=strlen(randCh);
	srand(time(NULL));
	int chIndex;
	for(int i=0;i<len;i++){
		chIndex=rand()%randLen;
		result[i]=randCh[chIndex];
	}
	//int size=sizeof(randCh);
	//printf("%d %d",randLen,size);
}


int main(){
	char text[20]="";
	getRandStr(text,10);
	printf("%s\n",text);
}


