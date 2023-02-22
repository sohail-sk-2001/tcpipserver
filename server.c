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
		read(cli_fd,buff,sizeof(buff));
		printf("From client: 5s\t To Clienmt: ", buff);
		bzero(buff,BUFSIZE);
		n=0;
		while((buff[n++]=getchar())!='\n');
		write(cli_fd,buff,sizeof(buff));

		if(strncmp("quit",buff,4)==0){
			printf("Server Exit...\n");
			break;
		}
	}
}


int main(){
	int server_socket,client_socket,len;
	SA_IN server_addr,client_addr;
	check((server_socket = socket(AF_INET,SOCK_STREAM,0)),"Failed to create socket");
	bzero(&server_addr,sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(SERVERPORT);

	check(bind(server_socket,(SA*)&server_addr,sizeof(server_addr)),"Bind failed");
	
	check(listen(server_socket,SERVER_BACKLOG),"Listen failed");
	printf("Server Listening....\n");
	len = sizeof(client_addr);

	check(client_socket=accept(server_socket,(SA*)&client_addr,&len),"Server Accept Failed");

	printf("Server accepted the client");

	handle_connection(client_socket);

	close(server_socket);
}

