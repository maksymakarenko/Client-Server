#include "insuline.h"

#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <pthread.h>
#include <time.h>

#define SIZE 2048

int main()
{
	FILE *logfile;
	logfile = fopen("logfile.txt", "a");

	time_t nowtime;
	struct tm * timeinfo;

	time(&nowtime);
	timeinfo = localtime(&nowtime);

	system("clear");
	int sock_serv = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock_serv == -1)
	{
		printf("--> %s Cannot create socket\n", asctime(timeinfo)); 
		fprintf(logfile, "--> %s Cannot create socket\n", asctime(timeinfo));
		exit(0);
	}
	else
	{
		printf("--> %s Socket created\n", asctime(timeinfo));
		fprintf(logfile, "--> %s Socket created\n", asctime(timeinfo));
	}
	
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server)); 
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(2345);
	socklen_t lenght_serv = sizeof(server);
	
	if(bind(sock_serv, (const struct sockaddr *)&server, lenght_serv) != 0)
	{
		printf("--> %s Cannot bind socket\n", asctime(timeinfo)); 
		fprintf(logfile, "--> %s Cannot bind socket\n", asctime(timeinfo));
		close(sock_serv); 
		exit(0);
	}
	else
	{
		printf("--> %s Socket is binded\n", asctime(timeinfo));
		fprintf(logfile, "--> %s Socket is binded\n", asctime(timeinfo));
	}

	if(listen(sock_serv, 2) != 0)
	{
		printf("--> %s Cannot listen to socket\n", asctime(timeinfo)); 
		fprintf(logfile, "--> %s Cannot listen to socket\n", asctime(timeinfo));
		close(sock_serv); 
		exit(0);
	}
	else
	{
		printf("--> %s Listen to socket\n", asctime(timeinfo));
		fprintf(logfile, "--> %s Listen to socket\n", asctime(timeinfo));
	}
	
	char *buffer;
	buffer = (char*)malloc(SIZE*sizeof(char));
	
	int clifd;
	
	struct sockaddr_in client;
	socklen_t lenght_client = sizeof(client);
	
	float height, weight;

	while(1){
		clifd = accept(sock_serv, (struct sockaddr *)&client, &lenght_client);
		if(clifd == -1)
		{
			printf("--> %s id %d Accept failed\n", asctime(timeinfo), clifd); 
			fprintf(logfile, "--> %s id %d Accept failed\n", asctime(timeinfo), clifd);
			close(sock_serv); 
			break;
		}
		else
		{
			printf("--> %s id %d is connected\n", asctime(timeinfo), clifd); 
			fprintf(logfile, "--> %s id %d is connected\n", asctime(timeinfo), clifd);
		}

		if(fork() == 0){
			int read_acc, i = 1;
			close(sock_serv);
			
			while(1){
				time_t nowtime;
				struct tm * timeinfosocket;

				time(&nowtime);
				timeinfosocket = localtime(&nowtime);


				if((read_acc = recv(clifd, buffer, SIZE, 0)) > 0 && i == 1){
					if(strcmp(buffer, "exit") == 0)
					{
						printf("--> %s id %d: exit\n", asctime(timeinfosocket), clifd);
						fprintf(logfile, "--> %s id %d: exit\n", asctime(timeinfosocket), clifd); 
						free(buffer);
						close(clifd); 
						fclose(logfile);
						exit(1);
					}	
					height = atof(buffer);
					printf("--> %s id %d: height %s\n", asctime(timeinfosocket), clifd, buffer);
					fprintf(logfile, "--> %s id %d: height %s\n", asctime(timeinfosocket), clifd, buffer); 
					memset(buffer, 0, SIZE);
					i++;
				}
				else
					{
						printf("--> %s id %d: terminated\n", asctime(timeinfosocket), clifd); 
						fprintf(logfile, "--> %s id %d: terminated\n", asctime(timeinfosocket), clifd); 
						close(clifd); 
						exit(1);
					}

				if((read_acc = recv(clifd, buffer, SIZE, 0)) > 0 && i == 2){
					if(strcmp(buffer, "exit") == 0)
					{
						printf("--> %s id %d: exit\n", asctime(timeinfosocket), clifd); 
						fprintf(logfile, "--> %s id %d: exit\n", asctime(timeinfosocket), clifd); 
						free(buffer);
						close(clifd); 
						fclose(logfile);
						exit(1);
					}
					weight = atof(buffer);
					printf("--> %s id %d: weight %s\n", asctime(timeinfosocket), clifd, buffer);
					fprintf(logfile, "--> %s id %d: weight %s\n", asctime(timeinfosocket), clifd, buffer);
					your_basalinsulin(height, weight,  &buffer);
					printf("--> %s id %d: result %s", asctime(timeinfosocket), clifd, buffer);
					fprintf(logfile, "--> %s id %d: result %s", asctime(timeinfosocket), clifd, buffer);
					write(clifd, buffer, strlen(buffer));
					memset(buffer, 0, SIZE);
					i = 1;
				}
				else
					{
						printf("--> %s id %d: terminated\n", asctime(timeinfosocket), clifd); 
						fprintf(logfile, "--> %s id %d: terminated\n", asctime(timeinfosocket), clifd); 
						close(clifd); 
						exit(1);
					}

			}
		}
	}
	
	close(sock_serv);
	close(clifd);
	fclose(logfile);
	free(buffer);
	return 0;
}