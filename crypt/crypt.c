//编译时加 -lcrypt
#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(){
	char key[30];
	char salt[30];
	char* encrypted;
	while(1){
		memset(key,0,sizeof(key));
		memset(salt,0,sizeof(salt));
		printf("key=");
		scanf("%s",key);
		printf("salt=");
		scanf("%s",salt);
		fprintf(stdin,"key=%s\nsalt=%s\n",key,salt);
		
		encrypted=crypt(key,salt);
		puts(encrypted);
	}
}

