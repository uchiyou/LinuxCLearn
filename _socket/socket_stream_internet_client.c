#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<netdb.h>
#include<unistd.h>

int main(int argc,char* argv[]){
	int connect_fd;// the socket resource mark
	int ret;
	char snd_buf[1024];// the buffer for send information
        int i;
	int port;
	int len;

	struct sockaddr_in srv_addr;// for the client infomation

	if(argc!=2){
		perror("argc!=2, no give port");
		return -1;
	}


	// turn the input string into type int
	port=atoi(argv[1]);


	//create a new socket
	connect_fd=socket(AF_INET,SOCK_STREAM,0);
        if(connect_fd==-1){
		perror("generate a socket failed");
		return -1;
	}


	// set the socket struct apparameters
	memset(&srv_addr,0,sizeof(srv_addr));
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	srv_addr.sin_port=htons(port);


        //
	ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret==-1){
		perror("connect to the service failed");
		close(connect_fd);
		return -1;
	}


        memset(snd_buf,0,1024);

	//get the information from stdinput ( keyboard),
	//then send to server,
	while(1){
		// write message to stardard output ( screen )
		write(STDOUT_FILENO,"input message:",14);
		// read message from stardard input ( keyboard )
		len=read(STDIN_FILENO,snd_buf,1024);

		//write message to server
		if(len>0)
			write(connect_fd,snd_buf,1024);

		// receive the message from service
		len=read(connect_fd,snd_buf,1024);
		if(len>0)
			printf("from service message : %s\n",snd_buf);

		// if there are end mark '@' ,then end
		if(snd_buf[len-1]=='@')
			break;
	}

	close(connect_fd);
	return 0;
}
