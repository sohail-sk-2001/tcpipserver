#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdbool.h>

#define SERVERPORT 8080
#define BUFSIZE 100
#define SERVER_BACKLOG 10

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int check(int e, const char *msg){
	if(e==-1){
		perror(msg);
		exit(1);
	}
	return e;
}
int main(){
	int server_socket;
	SA_IN server_addr;
	check((server_socket = socket(AF_INET,SOCK_STREAM,0)),"Failed to create socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(SERVERPORT);

	check(bind(server_socket,(SA*)&server_addr,sizeof(server_addr)),"Bind failed");
	
	check(listen(server_socket,SERVER_BACKLOG),"Listen failed");
	printf("Server Listening....\n");
}
