#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>


int main(int argc,int argv[]){
	int client_sock;
//	int server_sock;
	int length;
	int n;

	struct sockaddr_in srv_addr;
	struct sockaddr_in clt_addr;
	struct hostent *hp;

	char buffer[1024];

	if(argc!=2){
		perror("argc!=2, no port be input");
		return -1;
	}

	
	// 1 generate new socket
	//   AF_INET meanning TCP/IP protocal family,  SOCK_DGRAM meanning the socket type : data gram
	client_sock=socket(AF_INET,SOCK_DGRAM,0);
	if(client_sock<0){
		perror("generate new socket failed");
		return -1;
	}



	// 2 set the struct address data
	srv_addr.sin_family=AF_INET;// set TCP/IP protocal
	// get the host name then transform it into binary
        hp = gethostbyname("127.0.0.1");
	// copy the IP from hp to srv_addr.sin_addr
	bcopy((char*)hp->h_addr,(char*)&srv_addr.sin_addr,hp->h_length);
        // get the port from input and set little-endian
	srv_addr.sin_port=htons(atoi(argv[1]));



	// 3 receive data from keyboard
	printf("input the data:\n");
	memset(buffer,0,1024);
	fgets(buffer,1023,stdin);// receive 1024 bytes from standard input (keyboard


	// 4 send data to server
	n=sendno(client_sock,buffer,strlen(buffer),0,&srv_addr,sizeof(srv_addr));
	if(n<0){
		perror("send information failed");
		close(client_sock);
		return -1;
	}


	// 5 receive data from server
	memset(buffer,0,1024);//clear buffer with 0
	n=recvfrom(client_sock,buffer,strlen(buffer),0,&srv_addr,sizeof(srv_addr));
	if(n<0){
		perror("receive information failed");
		close(client_sock);
		return -1;
	}
	printf("receive data from server: %s\n",buffer);


	// 6  close the socket
	close(client_sock);
	return 0;
}
