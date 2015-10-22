/*  1
 * the function for get the net device and others information like IP , net mask and so on, 
 *  2  then open the net device and set the filt condition
 *  3   final deal with the packets
*/


#include<stdio.h>
#include<pcap.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h>



void deal_with_packet(u_char *user,const struct pcap_pkthdr *hdr,const u_char* packet){
	static int count=0;
	struct ether_header *eth_header;
	u_char *ptr;


	// printf the packet info
	printf("packet length: %d\n",hdr->len);
	printf("length of portion present: %d\n",hdr->caplen);


	eth_header=(struct ether_header*)packet;
	if(ntohs(eth_header->ether_type)!=ETHERTYPE_IP){
		perror("no ethernet packet");
		return;
	}


	// printf destination MAC
	ptr=eth_header->ether_dhost;
        int i;
	i=0;
	printf("destination address( MAC)");
	while(i<ETHER_ADDR_LEN){
		printf(" %x  ",*ptr++);
		i++;
	}


	// printf the source MAC
	ptr=eth_header->ether_shost;
	i=0;
	printf("\n source address (MAC )");
	while(i<ETHER_ADDR_LEN){
		printf(" %x ",*ptr++);
		i++;
	}

	printf("finish deal with %d packets \n",count);
	count++;




}





int main(int argc,char* argv[]){

	// the follow varies, for found net device and info
	char* net_dev;//   net device name
	char* ip_addr;//   the effective ip address
	char* net_mask;//  the net mask in the localhost
	char err_buffer[PCAP_ERRBUF_SIZE];// the buffer for error information

	bpf_u_int32 netp;// the point for IP address information
	bpf_u_int32 netmask;// the point for net mask information
        struct in_addr addr;// the struct for IP transform


	//  (3) the follow varies for open net device
	pcap_t *sniffer_des;
	
	// (4) for the filt confition
	struct bpf_program fp;
    
	// (5) for get packet
	int ret;
//	struct  bpf_program fp;

	// 1  get the device name in the system
	net_dev=pcap_lookupdev(err_buffer);
	if(net_dev==NULL){
		perror("detect the net device failed");
		return -1;
	}
	printf("net device in the machine : %s \n",net_dev);



	//  2 get the IP and net mask
	

	// look device  net_dev IP into netp, net mask into netmask,if error happen ,error info into err_buffer
	if(pcap_lookupnet(net_dev,&netp,&netmask,err_buffer)==-1){
		perror("try to get the IP and net mask failed\n");
		return -1;
	}
	// transform the format to string
	addr.s_addr=netp;
	ip_addr=inet_ntoa(addr);
	if(ip_addr==NULL){
		perror("transform IP format failed");
		return -1;
	}
	printf("current IP : %s\n",ip_addr);

	net_mask=inet_ntoa(addr);//net mask can be calculate by IP
	if(net_mask==NULL){
		perror("transform net mask failed");
		return -1;
	}
	printf("current net mask : %s\n",net_mask);



	// 3 open the net device
	
	// sniffer is the description of capture a device packet,
	//  open device  net_dev , allow accept max 65535 char, set the net device is  max mode, allow post off 1000 ms , 
	//  if error happen , let the error info save into err_buffer
	sniffer_des=pcap_open_live(net_dev,65535,1,1000,err_buffer);
	if(sniffer_des==NULL){
		perror("open net device failed");
		return -1;
	}
	printf("net device open successful\n");



	// 4  set filt condition
	char  filter_exp[]="port 23";// mean only capture data from 23 port
       // (1) compile the filt condition
	if(pcap_compile(sniffer_des,&fp,filter_exp,0,netmask)==-1){
		perror("compile failed 85 line");
		return -1;
	}

	// ( 2 ) apply the condition into the packet
	if(pcap_setfilter(sniffer_des,&fp)==-1){
		perror("apply the condition failed");
		return -1;
	}
   


	// 5 get the packet
	// capture 3 packet on sniffer_des resource description mark,and call back  through    deal_with_packet function
        ret=pcap_loop(sniffer_des,3,deal_with_packet,NULL);
        if(ret==-1 || ret == -2){
	perror("analyse the packet failed");
        return -1;
	}	


	// 6 close the sniffer description mark
        pcap_close(sniffer_des);
	return 0;
}

