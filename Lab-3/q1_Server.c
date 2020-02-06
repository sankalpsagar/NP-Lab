#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <signal.h>

void main()
{
	int s, r, recb, sntb, x, ns, a=0;
	printf("INPUT port number: ");
	scanf("%d", &x);
	socklen_t len;
	struct sockaddr_in server, client;

	char buff[200];
	char buff2[200];

	s = socket(AF_INET, SOCK_STREAM, 0);

	if(s==-1)
	{
		printf("\nSocket creation error!");
		exit(0);
	}

	printf("\nSocket successfully created.");

	server.sin_family = AF_INET;
	server.sin_port = htons(x);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	r = bind (s, (struct sockaddr*)&server, sizeof(server));
	if(r==-1)
	{
		printf("Binding error.");
		exit(0);
	}

	r = listen (s, 1);
	if(r==-1)
	{
		close(s);
		exit(0);
	}

	printf("\nListening...");

	len = sizeof(client);
	ns = accept(s, (struct sockaddr*)&client, &len);

	if(ns==-1)
	{
		close(s);
		printf("\nNo clients found!");
	}

	printf("\nConnection accepted.");
	int pid = fork();

	if (pid != 0) // parent process
	{
		printf("\nI'm the parent server process. My PID and PPID are: %d, %d.", getpid(), getppid());
		printf("\nMy child process has the pid: %d.", pid);
		while(1){
			recb = recv(ns, buff, sizeof(buff), 0);

			if (recb==-1)
			{
				printf("\nMessage receiving failed.");
				close(s);
				exit(0);
			}

			if (strcmp(buff, "Bye\n") == 0){
				printf("\nServer closing down. Goodbye!\n");
				close(ns);
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
		printf("\nI'm the child server process. My PID and PPID are: %d, %d.\n", getpid(), getppid());
		while(1){
			fflush(stdin);
			fflush(stdout);
			// printf("\n\nType your message: ");
			// scanf("%s", buff2);
			fgets(buff2, 200, stdin);

			sntb = send(ns, buff2, sizeof(buff2), 0);
			if (sntb==-1)
			{
				close(s);
				printf("\nMessage could not be sent.");
				exit(0);
			}

			if (strcmp(buff2, "Bye\n") == 0){
				printf("\nServer closing down. Goodbye!\n");
				kill(pid, SIGKILL);
				close(s);
				exit(0);
			}
		}
	}
}