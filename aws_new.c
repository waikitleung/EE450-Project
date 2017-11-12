#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define TCPPORT "25620"
#define UDPPORT "24620"
#define HOST "127.0.0.1"
#define PORTA "21620"
#define PORTB "22620"
#define PORTC "23620"
#define BACKLOG 10



void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family==AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);

}

int port_return(int UDPsocket,struct sockaddr_storage addr,socklen_t fromlen){
	int port;
	getpeername(UDPsocket,(struct sockaddr*)&addr,&fromlen);
	struct sockaddr_in *UDP=(struct sockaddr_in*)&addr;
	port=ntohs(UDP->sin_port);
	return port;
}





int main(){
	// set up the TCP--from Beej
	int sockfd,new_fd;  // sockfd is TCP socket, new_fd is used to oepn the new socket to communciate
	int UDPsocket;
	struct addrinfo hints,*servinfo,*p;
	struct addrinfo *servinfoA,*servinfoB,*servinfoC;
	struct sockaddr_storage their_addr;//  connector's address information
	struct sockaddr_storage addr;
	//struct sockaddr_in addr1;
	socklen_t sin_size;
	socklen_t fromlen;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	float unkonwn_result;
	
	

	
	memset(&hints,0,sizeof hints);
	hints.ai_family= AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;       //use my IP

	if((rv=getaddrinfo(HOST,TCPPORT,&hints,&servinfo))!=0){
		fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(rv));
		return 1;

	}

	for(p=servinfo;p!=NULL;p=p->ai_next){   
		if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
			perror("server:socket");
			continue;
		}

	

		if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd,p->ai_addr,p->ai_addrlen)==-1){
			close(sockfd);
			perror("server:bind");
			continue;
		}
		break;
	} 
	if(p==NULL){
		fprintf(stderr,"Server failed to bind \n");
		exit(1);
	}
	freeaddrinfo(servinfo); // all done with this structure
	
	

	// now generating the UDP socket
	hints.ai_socktype=SOCK_DGRAM;
	if((rv=getaddrinfo(HOST,UDPPORT,&hints,&servinfo))!=0){
		fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(rv));
		return 1;

	}
	for(p=servinfo;p!=NULL;p=p->ai_next){   
		if((UDPsocket=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
			perror("server:socket");
			continue;
		}

		if (setsockopt(UDPsocket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
			perror("setsockopt");
			exit(1);
		}
		if (bind(UDPsocket,p->ai_addr,p->ai_addrlen)==-1){
			close(UDPsocket);
			perror("server:bind");
			continue;
		}
		break;
	}
	if(p==NULL){
		fprintf(stderr,"Server failed to bind \n");
		exit(1);
	}
	freeaddrinfo(servinfo); // all done with this structure
	
	int severA_address,severB_address,severC_address;
	struct addrinfo *p1,*p2,*p3;
	severA_address=getaddrinfo(HOST,PORTA,&hints,&servinfoA);    // The type of P1?
	p1=servinfoA;

	severB_address=getaddrinfo(HOST,PORTB,&hints,&servinfoB);
	p2=servinfoB;
	
	severC_address=getaddrinfo(HOST,PORTC,&hints,&servinfoC);
	p3=servinfoC;
	
	
	



	//listen
	if (listen(sockfd,BACKLOG)==-1){
		perror("listen");
		exit(1);
	}
	printf("The AWS is up and running 1 \n");

	while(1){
		sin_size=sizeof their_addr;
		new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);
		if(new_fd==-1){
			perror("accept");
			continue;
		}
		

		// get the  client port number
		int client_port;
		getpeername(new_fd,(struct sockaddr*)&their_addr,&sin_size);
		struct sockaddr_in *fd=(struct sockaddr_in*)&their_addr;
		client_port=ntohs(fd->sin_port);

		//receive the information from client    HOW TO GET PORT NUMBER!!
		char function[4];     // receive the function name
		function[3]=0;
		float data;
		recv(new_fd,function,3 ,0);
		int b1=recv(new_fd,&data,sizeof data,0);
		printf("The AWS received %f and function %s from the client using TCP from port.\n",data,function);

		sendto(UDPsocket,&data,sizeof data,0,p1->ai_addr,p1->ai_addrlen);
		sendto(UDPsocket,&data,sizeof data,0,p2->ai_addr,p2->ai_addrlen);
		sendto(UDPsocket,&data,sizeof data,0,p3->ai_addr,p3->ai_addrlen);


		printf("The AWS sent %f to BackendServer A \n", data);
		printf("The AWS sent %f to BackendServer B \n", data);
		printf("The AWS sent %f to BackendServer C \n", data);
		printf("qeqwwdasds\n", data);
		//   fromlen= sizeof addr1;    test later!!
		recvfrom(UDPsocket,&unkonwn_result,sizeof unkonwn_result,0,&addr,&fromlen);
		printf("124124123214214\n", data);
		//  port=ntohs(addr1.sin_port);   test later!!
		//  printf("receive %d" port);    test later!!
		//  determine where the result comes from
		int port;
		port=port_return(UDPsocket,addr,fromlen);     //  return the port number
		printf("%d, %f, %d \n", b1,unkonwn_result,port);
		float resultA1;  // the square result
		float resultB1;	 // the cube result
		float resultC;   // the 5th power result
		float resultA2;	 // the 4th power result
		float resultB2;  // the 6th power result

		if (port==21620)
			resultA1=unkonwn_result;
		else if(port==22620)
			resultB1=unkonwn_result;
		else
			resultC=unkonwn_result;
		
		b1=recvfrom(UDPsocket,&unkonwn_result,sizeof unkonwn_result,0,&addr,&fromlen);
		port=port_return(UDPsocket,addr,fromlen);
		printf("%d, %f, %d \n", b1,unkonwn_result,port);

		if (port==21620)
			resultA1=unkonwn_result;      // square result
		else if(port==22620)
			resultB1=unkonwn_result;	  // cube result
		else
			resultC=unkonwn_result;		  // 5-times result

		recvfrom(UDPsocket,&unkonwn_result,sizeof unkonwn_result,0,&addr,&fromlen);
		port=port_return(UDPsocket,addr,fromlen);
		printf("%d, %f, %d \n", b1,unkonwn_result,port);
		if (port==21620)
			resultA1=unkonwn_result;
		else if(port==22620)
			resultB1=unkonwn_result;
		else
			resultC=unkonwn_result;


		printf("The AWS reveived square result %f from BackendServer A using UDP over port %c \n", resultA1,PORTA);
		printf("The AWS reveived cube result %f from BackendServer B using UDP over port %c \n", resultB1,PORTB);
		printf("The AWS reveived quintic result %f from BackendServer C using UDP over port %c \n", resultC,PORTC);

		sendto(UDPsocket,&resultA1,sizeof resultA1,0,p1->ai_addr,p1->ai_addrlen);  //send to A get The 4 power
		sendto(UDPsocket,&resultA1,sizeof resultA1,0,p2->ai_addr,p2->ai_addrlen);  //send to B get The 6 power
		printf("The AWS sent %f to BackendServer A \n", resultA1);
		printf("The AWS sent %f to BackendServer B \n", resultA1);

		recvfrom(UDPsocket,&unkonwn_result,sizeof unkonwn_result,0,&addr,&fromlen);
		port=port_return(UDPsocket,addr,fromlen);
		if (port==21620)
			resultA2=unkonwn_result;  // the 4th power
		else if(port==22620)
			resultB2=unkonwn_result;  // the 6 power

		recvfrom(UDPsocket,&unkonwn_result,sizeof unkonwn_result,0,&addr,&fromlen);
		port=port_return(UDPsocket,addr,fromlen);
		if (port==21620)
			resultA2=unkonwn_result;  // the 4th power
		else if(port==22620)
			resultB2=unkonwn_result;  // the 6 power
		
		//calculate the final result   ATTENTION! Here needs modify!!!!!!
		float final_result=0.0;
		if(strcmp(function,"DIV")==0){
			final_result=1+resultA1+resultB1+resultA2+resultC+resultB2;

		}
		else if (strcmp(function,"LOG")==0){
			final_result=-data-(resultA1)/2-resultB1/3-resultA2/4-resultC/5-resultB2/6;
		}

		printf("The AWS calculate %s on %f %f \n", function,data,final_result);
		send(new_fd,&final_result,sizeof(final_result),0);
		close(new_fd);

	} //end of while

}// end of main function
		
