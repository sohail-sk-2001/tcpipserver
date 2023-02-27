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

void handle_connection(int cli_fd){
	char buff[BUFSIZE];
	int n;
	while(true){
		bzero(buff,BUFSIZE);
		printf("Enter the message: ");
		n=0;
		while((buff[n++]=getchar())!='\n');
		write(cli_fd,buff,BUFSIZE);
		bzero(buff,BUFSIZE);
		read(cli_fd,buff,BUFSIZE);
		printf("From Server: %s",buff);
		if((strncmp(buff,"quit",4))==0){
			printf("\nClient Exit....\n");
			break;
		}
	}
}


int main(){
	int client_socket,server_socket;
	SA_IN server_addr,client_addr;
	check((client_socket = socket(AF_INET,SOCK_STREAM,0)),"Failed to create socket");
	bzero(&server_addr,sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(SERVERPORT);
	check(connect(client_socket,(SA*)&server_addr,sizeof(server_addr)),"Failed to connect");
	printf("Connected to server....\nType quit to close the connection\n");

	handle_connection(client_socket);

	close(client_socket);
}

