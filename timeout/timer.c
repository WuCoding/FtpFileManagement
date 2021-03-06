#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

//itimerval结构体的定义
int handle_count=0;
void set_time(void){
	struct itimerval itv;
	itv.it_interval.tv_sec=1;//自动装载，之后每10秒响应一次
	itv.it_interval.tv_usec=0;
	itv.it_value.tv_sec=1;//第一次定时的时间
	itv.it_value.tv_usec=0;
	setitimer(ITIMER_REAL,&itv,NULL);
}   
void alarm_handle(int sig){    
	handle_count++;
	if(!(handle_count%30)){
		printf("have handle count is %d\n",handle_count/30);
	}
}   
void main(void)
{
	struct itimerval itv;
	signal(SIGALRM,alarm_handle);
	set_time();
	while(1);
}
