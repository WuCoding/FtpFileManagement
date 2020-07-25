//编译时加 -lcrypto
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

int main(int argc,char* argv[]){
	char* buf="helloworld";
	char* md;
	int i;
	md=MD5(buf,strlen(buf),NULL);
	printf("%ld==%s\n",strlen(md),md);
	for(i=0;i<strlen(md);i++){
		printf("%02hhx",md[i]);
	}
	printf("\n");
	return 0;
}
