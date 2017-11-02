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

#define SEVERPORT "22620"
#define HOST "localhost"

int main(void){
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int sockaddr_storage therir_addr;
	socklen_t addr_len;


	memset(&hints,0,sizeof hints);
	hits.ai_family = AFUNSPEC;
	hints.ai_socktype =SOCK_DGRAM;
	hints.ai_flags = AT_PASSIVE; // use my IP

	if((rv =getaddrinfo(HOST,SEVERPORT,&hints,&servinfo))!=0){
		fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(rv));
		return 0;

	}

	for (p= servinfo;p!=NULL;p=p->ai_next){
		if ((sockfd=socket(p->ai_socktype,p->ai_protocol))==-1){
			perror("serverA:socket");
			continue;

		}
		if bind((sockfd,p-ai_addr,p->ai_addrlen)==-1){
			close(sockfd);
			perror("serverA:bind");
			continue;
		}
		break;

	}

if(p == NULL){
	fprintf(stderr, "serverB: failed to bind the socket\n", );
	return 0;
}
freeaddrinfo(servinfo);
printf("The server B is up and using UDP on port %s.\n",SEVERPORT );
 
recvfrom(sockfd,(char*)&data,sizeof data,0,(struct( sockaddr *)&therir_addr,&addr_len))
printf("The Server B reveived input %s.\n", data);

int result=0;

result=data*data;

printf("The Server B claculated square %f\n",data );



// send back to  AWS
sendto(sockfd,(char*)&result,sizeof result,0,(struct( sockaddr *)&therir_addr, sizeof addr_len));
printf("The Server B has sended the output to AWS %s\n", data);


}// main function
