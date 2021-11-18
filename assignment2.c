#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Reading and writing end of the pipe
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv)
{

	/*************************************** command line arguments ***************************************
		To compile assignment2.c: gcc assignment2.c -o assignment2
		To run your program: ./assignment2 "CS" "3305" " is fun!"
		argv[0] = "./assignment2"
		argv[1] = "CS"
		argv[2] = "3305"
		argv[3] = " is fun!"

		Number of arguments (argc) = 4

		In this assignment, the parent process reads argv[1] and the child process reads argv[2] and argv[3]
		e.g. In the parent process: char *x = argv[1];
	*******************************************************************************************************/

	// If the user does not pass X, Y and Z, the program will terminate
	if (argc != 4)
	{
		printf("Invalid arguments\n");
		exit(0);
	}
	/*
	// You must insert the following into your code (Replace zeros with the appropriate values/variables)
	printf("A pipe is created for communication between parent (PID %d) and child\n", 0);
	printf("parent (PID %d) created a child (PID %d)\n", 0, 0);
	printf("parent (PID %d) receives X = \"%s\" from the user\n", 0, 0);
	printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n", 0, 0, 0);
	printf("child (PID %d) concatenates Y and Z to generate Y'= %s\n", 0, 0);
	printf("child (PID %d) writes Y' into the pipe\n", 0);
	printf("parent (PID %d) reads Y' from the pipe (Y' = \"%s\")\n", 0, 0);
	printf("parent (PID %d) concatenates X and Y' to generate the string: %s\n", 0, 0);

	// Note: You do not need to implement a function to concatenate two strings. "string.h" header file has been included

	*/

	/***************************************************************************************************
	 * 										   YOUR CODE GOES HERE										
	 ***************************************************************************************************/
	int n;
  	int status;
  	int port[2];
  	pid_t pid;

	// Create pipe, if error occurs, print that an error has occured and exit
	if (pipe(port) < 0){
    	perror("pipe error");
    	exit(0);
  	}
	printf("A pipe is created for communication between parent (PID %d) and child\n", getpid());


	// Execute the fork
	pid = fork();

	// If we are inside the parent
	if (pid > 0){
		printf("parent (PID %d) created a child (PID %d)\n", getpid(), pid);
		char* xStr = argv[1];	
		printf("parent (PID %d) receives X = \"%s\" from the user\n", getpid(), xStr);

		// Wait for child to finish
		wait(NULL);

		// Read the length of the string in
		int len_yzStr;
		read(port[0], &len_yzStr, sizeof(len_yzStr));

		// Read in the string 
		char yzStr[len_yzStr];
		read(port[0], yzStr, len_yzStr);
		printf("parent (PID %d) reads Y' from the pipe (Y' = \"%s\")\n", getpid(), yzStr);

		// Concatenate the strings
		int len_xyzStr = (strlen(xStr) + strlen(yzStr) + 1);  // increased length for null pointer at the end of a string
		char xyzStr[len_xyzStr];
		strcpy(xyzStr, xStr);
		strcat(xyzStr, yzStr);
		printf("parent (PID %d) concatenates X and Y' to generate the string: %s\n", getpid(), xyzStr);
	}

	//If we are inside the child
	if (pid == 0){
		// Read the string from the command line prompt
		char* yStr = argv[2];
		char* zStr = argv[3];
		printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n", getpid(), yStr, zStr);

		// Concatenate the strings 
		int len_yzStr = (strlen(yStr) + strlen(zStr));
		char yzStr[len_yzStr + 1]; // increased length for null pointer at the end of a string
		strcpy(yzStr, yStr);
		strcat(yzStr, zStr);
		printf("child (PID %d) concatenates Y and Z to generate Y'= %s\n", getpid(), yzStr);

		// Write the string and the lenth of the string into the pipe
		int len_yzStr2 = len_yzStr + 1;  // increased length for null pointer at the end of a string
		write(port[1], &len_yzStr2, sizeof(len_yzStr2));
		write(port[1], yzStr, len_yzStr2);
		printf("child (PID %d) writes Y' into the pipe\n", getpid());
	}







	return 0;
}
