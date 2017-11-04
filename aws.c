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
#define UDP "24620"
#define HOST "localhost"
#define PORTA "21620"
#define PORTB "22620"
#define PORTC "23620"

#ifndef DATA_H_
#define DATA_H_
 float data=resultA;
#endif
#include <data.h>




// get sockaddr,IPV4 OR IPV6
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family==AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);

}

// the backsever calculate the  result (Using UDP)
float calcul(float data,char function[],char sever_name){
	int mysock;
	struct addrinfo hints,*servinfo, *p;
	int rv;
	char* backsever_port

if (sever_name=='A')
	backsever_port=PORTA;
else if (sever_name='B')
	backsever_port=PORTB;
else if (sever_name='C')
	backsever_port=PORTC;




// set up UDP
memset(&hins,0,sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ao_socketype = SOCK_DGRAM;

if ((rv=getaddrinfo(HOST,backsever_port,&hints,&servinfo))!=0){
	fprintf(stderr,"getaddrinfo:" "%s\n",gai_strerror(rv) );
	return 1;
}
//loop throuth all the results 
	for (p=servinfo;p!=NULL;p=p->ai_next){
		if((mysock=socket(p->ai_family,p->ai_socket,p->ai_protocol))==1){
			perror("listener:"socket);
			continue;

		}

		break;

	}
if(p==NULL){
	fprintf(stderr,"listener:failed to bind the socket\n");
	return 2;
}


// use UDP send data
sendto(mysock,function,sizeof function,0,p->ai_addr,p->ai_addrlen);
sendto(mysock,(char*)&data,sizeof data,0,p->ai_addr,p->ai_addrlen);
printf("The AWS sent %f to the BackendServer %c,\n", data,sever_name);

float result =0;
recvfrom(mysock,(char*)&result,sizeof result,0,NULL,NULL);
return result;

} //  calcul end


int main(){
	// set up the TCP--from Beej
	int sockfd,new_fd;  // new_fd is used to oepn the new socket to communciate
	struct addrinfo hints,*servinfo,*p;
	struct sockaddr_storage their_addr;//  connector's address information
	socklen_t sin_size;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	char function=function;
	float data=data;
	
	memset(&hints,0,sizeof hints);
	hints.ai_family= AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;

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
	return 2;
	}
	freeaddrinfo(servinfo);

	//listen
	if (listen(sockfd,BACKLOG)==-1){
		perror("listen");
		exit(1);
	}
	printf("The AWS is up and running \n");

	while(1){
		sin_size=sizeof their_addr;
		new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);
		if(new_fd==-1){
			perror("accept");
			exit(1);
		}

		// get the port of the client     Go BACK TO CHECK THESE LINES!!
		inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr*)&their_addr),s,sizeof s);
		struct sockaddr_in addrTheir;
		memset(&addrTheir,0,sizeof(addrTheir));
		int len =sizeof(addrTheir);
		getpeername(new_fd,(struct sockaddr*)&addrTheir,(socklen_t*)&len);
		int client_port=addrTheir.sin_port;

		//reveive the information from client
		recv(new_fd,function,sizeof function,0);
		printf("The AWS received %f and function %s from the client using TCP from port %d \n",data,function, client_port);


		float resultA=calcul(function,data,'A');
		float resultB=calcul(function,data,'B');
		float resultC=calcul(function,data,'C');

		printf("The AWS reveived square result %f from BackendServer A using UDP over port %d \n", resultA,PORTA);
		printf("The AWS reveived cube result %f from BackendServer B using UDP over port %d \n", resultB,PORTB);
		printf("The AWS reveived quintic result %f from BackendServer C using UDP over port %d \n", resultC,PORTC);

		float origin_data=data;
		float data=resultA;
		float reusltA1=calcul(function,data,'A');
		float data=resultB;
		float reusltB1=calcul(function,data,'A');
		printf("The AWS reveived biquadratic result %f from BackendServer A using UDP over port %d \n", resultA1,PORTA);
		printf("The AWS reveived six power result %f from BackendServer B using UDP over port %d \n", resultB1,PORTB);



		//calculate the final reuslt   ATTENTION! Here needs modify!!!!!!
		if(strcmp(function,DIV)==0){
			reuslt=1+origin_data+resultA+resultB+reusltA1+resultC+reusltB1;

		}
		else if (strcmp(function,LOG)==0){
			reuslt=-origin_data-(resultA)/2-resultB/3-reusltA1/4-resultC/5-reusltB1/6;
		}

		printf("The AWS calculate %s on %f %f \n", function,origin_data,reuslt);
		send(new_fd,(const char*)&reuslt,sizeof(reuslt),0);
		close(new_fd);

	} //end of while

	}// end of main function
		
