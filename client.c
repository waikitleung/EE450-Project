#include <stdio.h>
#include <stdlib.h>
#include <unisted.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define AWSPORT "25620"   //AWS tcp port number
#define HOST "localhost"
 
int data[10];

void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family==AF_INET)
		{
			return &(((struct sockaddr_in*) sa)->sin_addr);
		}
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void data_input(){

	FILE *stream=NULL;
	stream=fopen("data.csv","r");
	if(stream=NULL){
		printf("can not open the file!\n");
		exit(0);
	}
	//int ch;
	while(!feof(stream)){
		fscanf(stream,"%f",&data[0])
	}
	fclose(stream);
}


int main(int argc, char* argv[]){
		char function[2]
		strcpy(function,argv[1])
        int t=0;
        for(t=0;t<2;t++){
        	function[t]=toupper(function[t])
        }
}


// setup the TCP connection
int main(int argc,char *argv[]){
int sockfd;
struct addrinfo hints,*servinfo,*p;
int rv;

memset(&hints,0,sizeof hints);
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_STREAM;

if((rv=getaddrinfo(HOST,AWSPORT,&hints,&servinfo))!=0){
	fprintf(stderr, "getaddrinfo:"%s\n",gai_strerror(rv));
	return 1;
}

for (p=servinfo;p!=NULL;p=p->ai.next){
	if((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==1){
		perror("client:socket");
		continue;
	}
	
	if(connect(sockfd,p->ai_addr,p->ai_addrlen)==-1){
		close(socket);
		perror("client:connect")
		continue;
	}
	break;
}

if (p==NULL){
	fprintf(stderr, "client:failed to connect.\n", );
	exit(0);
	return 2;
}
freeaddrinfo(servinfo);
printf("The client is up and running\n");  
send(sockfd,function,sizeof function,0)
send(sockfd,(char*)&data,sizeof data,0)


printf("The client has sent the "%s %f" to AWS.\n", function,&data);

int result=0;;
recv(sockfd,(char*)&result,sizeof result,0);
printf("According to AWS "%s" on "%f","%f",function,&data,&result);