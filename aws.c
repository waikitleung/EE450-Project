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

// get sockaddr,IPV4 OR IPV6
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family==AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);

}

// the backsever calculate the  result (Using UDP)
float calcul(float data[],char function[],sever_name){
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
// use the UDP to send the data to backsever
