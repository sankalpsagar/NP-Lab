#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include <signal.h>

void main()
{
	int s, r, recb, sntb, x;
	printf("INPUT port number: ");
	scanf("%d", &x);
	struct sockaddr_in server;
	char buff[200];
	char buff2[200];
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s==-1)
	{
		printf("\nSocket creation error.");
		exit(0);
	}

	printf("\nSocket successfully created.");

	server.sin_family = AF_INET;
	server.sin_port = htons(x);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	r = connect(s, (struct sockaddr*)&server, sizeof(server));
	if (r==-1)
	{
		printf("\nCannot connect.");
		exit(0);
	}

	printf("\nSuccesfully connected.");
	int pid = fork();

	if (pid != 0) // parent process
	{
		printf("\nI'm the parent client process. My PID and PPID are: %d, %d.", getpid(), getppid());
		printf("\nMy child process has the pid: %d.", pid);
		while(1){
			recb = recv(s, buff, sizeof(buff), 0);
			if (recb==-1)
			{
				printf("\nMessage receiving failed.");
				close(s);
				exit(0);
			}


			if (strcmp(buff, "Bye\n") == 0){
				printf("\nClient closing down. Goodbye!\n");
				close(s);
				exit(0);
			}

			fflush(stdin);
			fflush(stdout);
			printf("\nMessage received!");
			printf("\nMESSAGE: ");
			printf("%s\n", buff);
		}
	}

	else // child process
	{
		printf("\nI'm the child client process. My PID and PPID are: %d, %d.\n", getpid(), getppid());
		while(1){
			fflush(stdin);
			fflush(stdout);
			// printf("\n\nType your message: ");
			// scanf("%s", buff);
			fgets(buff2, 200, stdin);

			sntb = send(s, buff2, sizeof(buff), 0);
			if (sntb==-1)
			{
				close(s);
				printf("\nMessage could not be sent.");
				exit(0);
			}

			if (strcmp(buff2, "Bye\n") == 0){
				printf("\nClient closing down. Goodbye!\n");
				close(s);
				kill(pid, SIGKILL);
				exit(0);
			}
		}
	}
}