#include "head.h"
//recv()
int recvCycle(int fd,char* buf,int dataLen){
	int curLen=0;
	int ret=0;
	while(curLen<dataLen){
		ret=recv(fd,buf+curLen,dataLen-curLen,0);
		ERROR_CHECK(ret,-1,"recv");
		if(ret==0){//此时客户端已经关闭
			return 0;
		}
		curLen+=ret;
	}
	return curLen;
}

//recvCycle()
int recvData(int fd,char* buf){
	int dataLen;
	int ret;
	//接收数据长度
	ret=recvCycle(fd,(char*)&dataLen,sizeof(int));
	ERROR_CHECK(ret,-1,"recvCycle");
	if(ret==0){//对端关闭
		return 0;
	}
	//接收数据
	ret=recvCycle(fd,buf,dataLen);
	ERROR_CHECK(ret,-1,"recvCycle");

	return ret;
}
