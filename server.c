#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<pthread.h>
#include"myqueue.h"

#define SERVERPORT 8080
#define BUFSIZE 100
#define SERVER_BACKLOG 10
#define THREAD_POOL_SIZE 30

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int check(int e, const char *msg);
void * thread_function(void *arg);
void* handle_connection(void* client_fd);

int check(int e, const char *msg){
	if(e==-1){
		perror(msg);
		exit(1);
	}
	return e;
}

void * thread_function(void *arg){
	while(true){
		int *pclient;
		pthread_mutex_lock(&mutex);
		pclient = dequeue();
		pthread_mutex_unlock(&mutex);
		if(pclient != NULL){
			handle_connection(pclient);
		}
	}
}

void* handle_connection(void* client_fd){
	int cli_fd = *((int*)client_fd);
	free(client_fd);
	char buff[BUFSIZE];
	int n;
	while(true){
		bzero(buff,BUFSIZE);
		read(cli_fd,buff,sizeof(buff));
		printf("From client: %s\n To Client: ", buff);
		bzero(buff,BUFSIZE);
		n=0;
		while((buff[n++]=getchar())!='\n');
		write(cli_fd,buff,sizeof(buff));

		if(strncmp("quit",buff,4)==0){
			printf("Server Exit...\n");
			close(cli_fd);
			break;
		}
	}
}


int main(){
	int server_socket,client_socket,len;
	SA_IN server_addr,client_addr;
	check((server_socket = socket(AF_INET,SOCK_STREAM,0)),"Failed to create socket");
	bzero(&server_addr,sizeof(server_addr));

	for (int i=0;i<THREAD_POOL_SIZE;i++){
		pthread_create(&thread_pool[i],NULL,thread_function,NULL);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(SERVERPORT);

	check(bind(server_socket,(SA*)&server_addr,sizeof(server_addr)),"Bind failed");
	
	check(listen(server_socket,SERVER_BACKLOG),"Listen failed");
	printf("Server Listening....\n");
	len = sizeof(client_addr);
	
	while(true){
		printf("Waiting for Connections...\n");
		check(client_socket=accept(server_socket,(SA*)&client_addr,&len),"Server Accept Failed");
	
	printf("Server accepted the client");

	//handle_connection(client_socket);
	//pthread_t t;
	
	int *p_client = malloc(sizeof(int));
	*p_client = client_socket;
	pthread_mutex_lock(&mutex);
	enqueue(p_client);
	pthread_mutex_unlock(&mutex);
	//pthread_create(&t,NULL,handle_connection,p_client);
	}

}

