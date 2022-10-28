#include <stdio.h>  
#include <string.h>  
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "insuline.h"

#define key 65
#define size 2048

float pow(float height)
{
	return height*height;
}

void your_basalinsulin(float height, float weight, char **buffer)
{
	int shmid = shmget(key, size, 0666|IPC_CREAT);
	float *res = (float*) shmat(shmid, (void*)0, 0);
	int status;
	float height_in_meters = height / 100.0;

	if(fork() == 0)
	{
		float iwh;
		iwh = pow(height_in_meters) * 19.0 * 0.7;
		res[0] = iwh;
		exit(1);
		shmdt(res);
	}
	else
	{
		float iwh = pow(height_in_meters) * 19.0 * 0.7 * 0.4;
		wait(&status);
		int shmid = shmget(key, size, 0666|IPC_CREAT);
		float *res = (float*) shmat(shmid, (void*)0, 0);
		sprintf(*buffer, "\nDaily insulin dose: %f\nYour basal insulin: %f\n", res[0], iwh);
		shmdt(res);
		shmctl(shmid,IPC_RMID,NULL);
	}
}