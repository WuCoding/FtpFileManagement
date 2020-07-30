#include "head.h"

//epoll_ctl()
int epollAdd(int eplFd,int fd){
	int ret;
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	ret=epoll_ctl(eplFd,EPOLL_CTL_ADD,fd,&event);
	ERROR_CHECK(ret,-1,"epoll_ctl");
	printf("%d add in epoll\n",fd);
	return 0;
}

//epoll_ctl()
int epollDel(int eplFd,int fd){
	int ret;
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	ret=epoll_ctl(eplFd,EPOLL_CTL_DEL,fd,&event);
	ERROR_CHECK(ret,-1,"epoll_ctl");
	printf("%d delete from epoll\n",fd);
	return 0;
}

