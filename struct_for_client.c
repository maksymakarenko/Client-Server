#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#define SIZE 2048

int main()
{
	int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock_cli == -1){printf("cannot create socket"); exit(0);}

	struct sockaddr_in client;
	memset(&client, 0, sizeof(client)); 
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	client.sin_port = htons(2345);
	socklen_t lenght_client = sizeof(client);
	
	if(connect(sock_cli, (const struct sockaddr*)&client, lenght_client) != 0) {printf("cannot connect"); close(sock_cli); exit(0);} 
	
	char *buffer;
	buffer = (char*)malloc(SIZE * sizeof(char));
	
	puts("\nBasal Insuline Calculator");
	
	while(1)
	{
		puts("\nType your height: ");
		scanf("%s", buffer);
		send(sock_cli, buffer, strlen(buffer), 0);
		if(strcmp(buffer, "exit") == 0){close(sock_cli); break;}
		memset(buffer, 0, SIZE);

		
		puts("\nType your weight: ");
		scanf("%s", buffer);
		send(sock_cli, buffer, strlen(buffer), 0);
		if(strcmp(buffer, "exit") == 0){close(sock_cli); break;}
		memset(buffer, 0, SIZE);
		
		if(recv(sock_cli, buffer, SIZE, 0) < 0){printf("receive failed"); break;}
		puts(buffer);
	}
	
	close(sock_cli);
	return 0;
}
