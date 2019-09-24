/*
 * Shell.c
 *
 *  Created on: Feb 19, 2018
 *      Author: Ritika Singhal
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>

//Maximum length of the user input
#define MAXLINE 80

//Function Prototype
int parse_string(char str[], char *arg[]);

int main()
{
	char *args[MAXLINE/2 + 1];		//Args array which will hold the parsed command line input
	int should_run = 1;				//The loop counter which terminates the loop
	char str[MAXLINE/2 + 1];		//Temp array for the user input.

	//If if_wait is 1, then parent waits for the child to terminate,
	//and if 0, then parent continues without waiting
	int if_wait;

	printf("CS 149 Shell from Ritika Singhal\n");

	while(should_run)
	{
		printf("Ritika-049> ");		//Output prompt in the shell
		fflush(stdout);
		fgets(str, (MAXLINE/2 + 1), stdin);	//getting the user input i.e. command
		fflush(stdin);

		//If user enters exit as the command, then loop terminates and program ends.
		if(strcmp(str, "exit\n") == 0)
		{
			should_run = 0;
			break;
		}

		//The parse_string function is called to parse the user command and
		//this function returns if the user command wants the parent to wait or not.
		if_wait = parse_string(str, args);

		pid_t pid;		//the process id

		//this creates a child process and both the parent and child process return from fork().
		pid = fork();

		if(pid<0)				//error if fork fails
		{
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		else if(pid == 0)		//child process
		{
			//exec for child process

			execvp(args[0], args);
			printf("Exec error\n");	//error if exec fails.
			exit(1);
		}
		else					//parent process
		{
			if(if_wait)
			{
				wait(NULL);		//parent process waits for the child to terminate.
			}
		}

	}

	return 0;
}

//This functions parses the user input/command in the 'str' string and
//stores the tokens in the 'arg' array. End of the arg array contains 'NULL'
//It also checks if there is an '&' at the end of the command and
//returns the value of if_wait accordingly.
int parse_string(char str[], char *arg[])
{
	int if_wait =1;

	char *token;
	int i=0;

	int length = strlen(str);
	if(str[length-1]== '\n')
		str[length-1] = '\0';

	token = strtok(str, " ");

	while(token != NULL)
	{
		if(strcmp(token, "&") == 0)
		{
			if_wait = 0;
		}
		else
		{
			arg[i] = token;
			i++;
		}
		token = strtok(NULL, " ");
	}

	arg[i] = NULL;
	return if_wait;
}
