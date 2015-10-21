#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<errno.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<netdb.h>

void sig_handler(int signo){
	pid_t pid;
	int stat;
	

	//wait for child process exit , and tip when child process eixt
	pid=waitpid(-1,&stat,WNOHANG);
	if(pid>0){
		printf("process %ld, exit\n",(long)getpid());
		waitpid(-1,&stat,WNOHANG);
	}
	return;
}

int main(int argc,char* argv[]){

	socklen_t clt_addr_len;
	int listen_fd;
	int com_fd;
	int ret;
	int i;
	static char recv_buf[1024];
	int len;
	int port;
	int pid;


	struct sockaddr_in clt_addr;//the struct for client connect 

	struct sockaddr_in srv_addr;//the struct for service socket

	//when service start, should point a port to listen
	if(argc!=2){
		printf("argc!=2, not got a port to listen\n");
		return -1;
	}

	//get the input port 
	port=atoi(argv[1]);


	//set the signal deal function
	if(signal(SIGCHLD,sig_handler)<0){
		perror("set the sig_handler failed 50:");
		return -1;
	}


	//
	listen_fd=socket(PF_INET,SOCK_STREAM,0);
	if(listen_fd<0){
		perror("generate socket failed 59");
		return -1;
	}


	//
	memset(&srv_addr,0,sizeof(srv_addr));
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	srv_addr.sin_port=htons(port);


	//
	ret =bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret==-1){
		perror(" bind struct srv_add failed");
		close(listen_fd);
		return -1;
	}


        //
	ret=listen(listen_fd,6);
	if(ret==-1){
		perror("the service socket listen failed");
		close(listen_fd);
		return -1;
	}

	/*create a new process for every connected client,
	 *
	 * use function read accept the data,then transform 
	 * to up letter
	 * if met the character @ ,then end the connection*/
	while(1){
//		memset(&clt_addr,0,sizeof(srv_addr));
		len=sizeof(clt_addr);
		com_fd=accept(listen_fd,(struct sockaddr*)&clt_addr,&len);
		if(com_fd<0){
			if(errno==EINTR)
			continue;
		}
			else{
				perror("accept the client failed");
				close(listen_fd);
				return -1;
			}
		}


		//generate a new process for every connect
		pid=fork();
		if(pid<0){
			perror("create new process failed");
			close(listen_fd);
			return -1;
		}else if(pid==0){
			printf("accepte the data from client in process: %ld -------\n",(long)getpid());

			while((len=read(com_fd,recv_buf,1024))>0){
					printf("service receive data: %s\n",recv_buf);
					if(recv_buf[len-1]=='@')
					break;
					
					for(i=0;i<len;i++)
					recv_buf[i]=toupper(recv_buf[i]);
					write(com_fd,recv_buf,len);
					}
					close(com_fd);
					return 0;
					}else{
					close(com_fd);

		}
	
return 0;
}
