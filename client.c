#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define AWSPORT "25620"   //AWS tcp port number
#define HOST "127.0.0.1"
 


void *get_in_addr(struct sockaddr *sa){      //get sockaddr IPV4 or IPV6
	if (sa->sa_family==AF_INET)
		{
			return &(((struct sockaddr_in*) sa)->sin_addr);
		}
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
}




// setup the TCP connection
int main(int argc,char *argv[]){
	float data=atof(argv[2]);
	
   	int sockfd;
	struct addrinfo hints,*servinfo,*p;
	int rv;

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;

	if((rv=getaddrinfo(HOST,AWSPORT,&hints,&servinfo))!=0){
		fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(rv));
		return 1;
	}

	for (p=servinfo;p!=NULL;p=p->ai_next){
		if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==1){   //load up address
			perror("client:socket");
			continue;
		}
	
		if(connect(sockfd,p->ai_addr,p->ai_addrlen)==-1){   //ai_addr is the server address
			close(sockfd);
			perror("client:connect");
			continue;
		}
		break;   //  if get here means we have connected successfully
	}

	if (p==NULL){
		printf(stderr, "client:failed to connect.\n");
		exit(0);
		return 2;
	}
	freeaddrinfo(servinfo);

	printf("The client is up and running\n");  
	send(sockfd,argv[1],sizeof argv[1],0);
	send(sockfd,data,sizeof data,0);   //  Be aware of the data!!
	

	printf("The client has sent the %s %f to AWS.\n", argv[1],data);

	float result=0;;
	recvfrom(sockfd,result,sizeof result,0);   // receive from which socket??
	printf("According to AWS %s  on %f , %f",argv[1],data,result);
}