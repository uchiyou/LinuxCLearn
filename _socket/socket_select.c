#include<stdio.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc,char* argv[]){
	fd_set rfds;
	struct timeval tv;
	int retval;


	// wait for input from keyboard
	FD_ZERO(&rfds);// clear the file description
	FD_SET(0,&rfds);// add stardard input device into file description set


	// wait for 5 second
	tv.tv_sec=5;
	tv.tv_usec=0;// 1 second = 1000 usecond


	// only listen on stardard input device ( keyboard ) 
	retval=select(1,&rfds,NULL,NULL,&tv);
	if(retval==-1)
	{
		perror("select called failed");
		return -1;
	}else if(retval){
		printf("data is avaible now \n");
	}else
		printf("no data in the five second\n");

	return 0;
}
