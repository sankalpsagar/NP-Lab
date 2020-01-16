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
#define MAXSIZE 50

int testint(char*);
void search(char []);
void sort(char*);
void ssplit(char []);

main()
{
	int s, r, recb, sntb, x, ns, a=0;
	printf("INPUT port number: ");
	scanf("%d", &x);
	socklen_t len;
	struct sockaddr_in server, client;

	char buff[MAXSIZE];
	char intbuff[MAXSIZE+1];
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

	printf("\nAccepting...");

	recb = recv(ns, buff, sizeof(buff), 0);
	if (recb==-1)
	{
		printf("\nMessage receiving failed!");
		close(s);
		close(ns);
		exit(0);
	}

	printf("\nMessage received!");
	printf("\nMESSAGE: ");
	printf("%s", buff);

	// check if message is made up purely of integers
	int flag = testint(buff);
	if (!flag)
	{
		char b[] = "Send integers only.";

		sntb = send(ns, b, sizeof(b), 0);
		if (sntb==-1)
		{
			printf("\nMessage sending failed.");
			close(s);
			close(ns);
			exit(0);
		}
		printf("\n");
		close(s);
		close(ns);
		exit(0);
	}
	else
	{
		char b[] = "Success!";

		sntb = send(ns, b, sizeof(b), 0);
		if (sntb==-1)
		{
			printf("\nMessage sending failed.");
			close(s);
			close(ns);
			exit(0);
		}
		printf("\n");
	}

	strncpy(intbuff, buff, sizeof(buff));
	int fexit=0;

	while (!fexit)
	{
		recb = recv(ns, buff, sizeof(buff), 0);
		if (recb==-1)
		{
			printf("\nMessage receiving failed!");
			close(s);
			close(ns);
			exit(0);
		}

		// printf("\nMessage received!");
		// printf("\nMessage: ");
		// puts(buff);

		len = strlen(buff);
		if (len==1)
		{
			int choice = buff[0] - '0';
			if (choice > 3){
				char b[] = "Invalid choices.";

				sntb = send(ns, b, sizeof(b), 0);
				if (sntb==-1)
				{
					printf("\nMessage sending failed.");
					close(s);
					close(ns);
					exit(0);
				}

			close(s);
			close(ns);
			exit(0);
			}
		}

		int pos = 0;
		int choice = buff[0] - '0';
		switch(choice)
		{
			case 1:
				search(intbuff);
				break;
			case 2:
				sort(intbuff);
				break;
			case 3:
				ssplit(intbuff);
				break;
			default:
				break;
		}

		if (strcmp(buff, "exit") == 0)
		{
			fexit = 1;
		}

	}

	printf("\nServer closing down.");
	printf("\n");
	close(ns);
	close(s);
}


int testint(char* buff)
{
	int j;
	j = strlen(buff);
	while (j--)
	{
		if (buff[j]>=48 && buff[j]<=57)
		{
			continue;
		}

		return 0;
	}

	return 1;
}

void search(char buff[])
{
	int j = 0;
	char a;
	int pos = -1;
	printf("\nEnter item to search for: ");
	scanf(" %c", &a);
	for(j=0; j<strlen(buff); j++)
	{
		// printf("ayyyy");
		if (buff[j]==a)
		{
			pos = j;
			break;
		}
	}
	int x=0;
	fflush(stdin);
	fflush(stdout);
	printf("\nThe position is: %d\n", pos);
	//printf("\ngangam");
	fflush(stdin);
	fflush(stdout);
	return;
}

void sort(char* buff)
{
	int i, j;
	int n = strlen(buff);
	for(i=0; i<n; i++)
	{
		for(j=i; j<n; j++){
			if (buff[i] > buff[j]){
				int temp = buff [j];
				buff[j] = buff[i];
				buff[i] = temp;
			}
		}
	}

	printf("The sorted array is: %s\n", buff);
}

void ssplit(char* buff)
{
	int i;
	int n = strlen(buff);
	char even[100];
	char odd[100];
	int k=0, l=0;
	for (i=0; i<n; i++)
	{
		if ((buff[i]-'0')%2 == 0)
		{
			even[k] = buff[i];
			k++;
		}
		else
		{
			odd[l] = buff[i];
			l++;
		}
	}
	odd[k] = '\0';
	even[l] = '\0';

	fflush(stdin);
	fflush(stdout);
	printf("\nThe Odd Array: %s", odd);
	fflush(stdin);
	fflush(stdout);
	printf("\nThe Even Array: %s", even);
	fflush(stdin);
	fflush(stdout);
	return;
}