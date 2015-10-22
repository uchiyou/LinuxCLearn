#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/db.h>
#include<netdb.h>
#include<unistd.h>


int main(int argc,char* argv[]){
	int sock;// the server socket mark
	int length;// the server socket address struct data  length
	int fromlen;// the length of client socket address struct data
	int n;// for some count

	struct sockaddr_in server;//struct data of server address information
	struct sockaddr_in from;//struct data of client address information
	 
	char buf[1024];

	if(argc!=2){
		perror("argc!=2, no port information gave");
		return -1;
	}


	// create new socket 
	sock =socket(AF_INET,SOCK_DGRAM,0);// SOCK_DGRAM mean data gram format
	if(sock<0){
		perror("create new socket failed");
		return -1;
	}


	length=sizeof(server);
	// clear a content to 0 at a address
	bzero(&server,length);





	server.sin_family=AF_INET;// socket address struct : internet domain
	server.sin_addr.s_addr=INADDR_ANY;// any IP in the machine
	server.sin_port=htons(atoi(argv[1]));// transform the string into type int for port


	// bind the socket 
	if(bind(sock,(struct sockaddr*)&server,length)<0)
	{
		perror("bind the socket failed 40 lines");
		close(sock);
		return -1;
	}


	fromlen=sizeof(struct sockaddr);
	


	// receive information from client, and sent back
	while(1){
		n=recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&from,&fromlen);
		if(n<0){
			perror("receive data from client failed");
			break;
		}

		//show the received data to stardard ( screen )
		write(STDOUT_FILENO,buf,sizeof(buf));


		//send message back to client
		n=sendno(sock,"server received",15,0,(struct sockaddr*)&from,fromlen);
		if(n<0){
			perror("server send data back failed");
			return -1;
		}
	}


	close(sock);
	return 0;
}



