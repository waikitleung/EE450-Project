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

#define SEVERPORT "21620"
#define HOST "localhost"

int main(void){
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int sockaddr_storage,their_addr;
	socklen_t addr_len;


	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype =SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if((rv =getaddrinfo(HOST,SEVERPORT,&hints,&servinfo))!=0){
		fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(rv));
		return 0;

	}

	for (p= servinfo;p!=NULL;p=p->ai_next){
		if ((sockfd= socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
			perror("serverA:socket");
			continue;

		}
		if (bind(sockfd,p->ai_addr,p->ai_addrlen)==-1){
			close(sockfd);
			perror("serverA:bind");
			continue;
		}
		break;

	}

	if(p == NULL){
		fprintf(stderr, "serverA: failed to bind the socket\n");
		return 0;
	}
	freeaddrinfo(servinfo);
	printf("The server A is up and using UDP on port %s.\n",SEVERPORT );
 	
 	float data;
	recvfrom(sockfd,(char*)&data,sizeof data,0,(struct sockaddr*)&their_addr,&addr_len);
	printf("The Server A reveived input %f.\n", data);

	int result=0;

	result=data*data;

	printf("The Server A claculated square %f\n",data );



	// send back to  AWS
	sendto(sockfd,(char*)&result,sizeof result,0,(struct sockaddr *)&their_addr, sizeof addr_len);
	printf("The Server A has sended the output to AWS %f\n", data);


	}// main function
