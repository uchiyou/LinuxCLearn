#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>

#define UNIX_DOMAIN "UNIX.domain"

int main(int argc,int argv[]){
	socklen_t addr_len;
//	int listen_fd;
	int com_fd;
	int ret;
	int i;
	static char recv_buf[1024];
	int len;

	struct sockaddr_un clt_addr;
	struct sockaddr_un srv_addr;


	// create the socket
	com_fd=socket(PF_UNIX,SOCK_DGRAM,0);
	if(com_fd<-1){
		perror("generate new socket failed 23 lines :");
		return -1;
	}


	// set the attribute of the server socket
	srv_addr.sun_family=AF_UNIX;
	strcpy(srv_addr.sun_path,UNIX_DOMAIN);
	unlink(UNIX_DOMAIN);

	// transform string into IP address
	ret = bind(com_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret==-1){
		perror("bind the socket failed");
		close(com_fd);
		unlink(UNIX_DOMAIN);
		return -1;
	}


	//read message from client
	for(i=0;i<4;i++){
		memset(recv_buf,0,1024);//init the buffer with char '0'
		int num=recvfrom(com_fd,recv_buf,1024,0,(struct sockaddr*)&clt_addr,&addr_len);//receive data from address clt_addr
		printf("receive message from client: %s\n",recv_buf);
	}
	
	
	close(com_fd);
	unlink(UNIX_DOMAIN);
	return 0;
}

