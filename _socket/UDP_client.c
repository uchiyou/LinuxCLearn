#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>

#define UNIX_DOMAIN "UNIX.domain"

int main(int argc,int argv[]){
	int com_fd;
	int ret;
	int snd_buf[1024];
	int i;

	struct sockaddr_un srv_addr;
	struct sockaddr_un clt_addr;
	char clt_file[]="uuu";

	//set server address data struct
	srv_addr.sun_family=AF_UNIX;
	strcpy(srv_addr.sun_path,UNIX_DOMAIN);


	// create new socket 
	com_fd=socket(PF_UNIX,SOCK_DGRAM,0);
        if(com_fd<0){
		perror("create new socket failed");
		return -1;
	}


	// make the file be different from each other
	mkstemp(clt_file);


	//set the client address data struct
	clt_addr.sun_family=AF_UNIX;
	strcpy(srv_addr.sun_path,clt_file);
	unlink(UNIX_DOMAIN);


        // bind the address data to the socket
	ret=bind(com_fd,(struct sockaddr*)&clt_addr,sizeof(clt_addr)+strlen(clt_addr.sun_path));
	if(ret==-1){
		perror("bind the client failed");
		unlink(clt_file);
		close(com_fd);
	}


	// init the snd_buf by 0
	memset(snd_buf,0,1024);
        //set the data into snd_buf for send
	//sprintf(snd_buf,"%ld : message from client ",(long)getpid());
        
        printf("input the information will be sent:\n");
	read(STDIN_FILENO,snd_buf,sizeof(snd_buf));

	// send the ready info to server
	sendto(com_fd,snd_buf,sizeof(snd_buf),0,(struct sockaddr*)&srv_addr,sizeof(srv_addr));


 
	close(com_fd);
	unlink(clt_file);
	return 0;
}
