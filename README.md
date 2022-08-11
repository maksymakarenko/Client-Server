# Client-Server

PROJECT THEME: basal insuline dose calculator

LIBRARIES: <stdio.h> <string.h> <stdlib.h> <netinet/in.h> <sys/socket.h> <arpa/inet.h> <unistd.h> <sys/wait.h> <sys/types.h> <sys/ipc.h> <sys/shm.h>

TECNOLOGIES: sockets, processes, shared memory

DESCRIPTION: my program calculates basal insuline dose for people with "Type 1 diabetes". I use server for computing part of my program, which is provided by sockets. Calculatig of basal dose goes in two processes. First process calculate insuline dose for ideal weight. Second process calculate basal dose for your weight. Processes using shared memory.

COMPILING: using Makefile for faster and correct program compile
