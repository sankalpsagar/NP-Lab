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

main()
{
	int s, r, recb, sntb, x;
	printf("INPUT port number: ");
	scanf("%d", &x);
	struct sockaddr_in server;
	char buff[50];
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
	printf("\n\nType your array: ");
	scanf("%s", buff);

	sntb = send(s, buff, sizeof(buff), 0);
	if (sntb==-1)
	{
		close(s);
		printf("\nMessage could not be sent.");
		exit(0);
	}

	recb = recv(s, buff, sizeof(buff), 0);
	if (recb==-1)
	{
		printf("\nMessage receiving failed.");
		close(s);
		exit(0);
	}

	printf("\nMessage received!");
	printf("\nMESSAGE: ");
	printf("%s", buff);

	while(1){
		int a;
		printf("\nEnter your choice: \n1.Search \n2.Sort \n3.Split \n4.EXIT \n");
		scanf ("%d", &a);
		char c = a + '0';
		buff[0] = c;
		buff[1] = '\0';

		if (a == 4){
			buff[0]='e';
			buff[1]='x';
			buff[2]='i';
			buff[3]='t';
			buff[4]='\0';

			break;
		}

		sntb = send(s, buff, sizeof(buff), 0);
		if (sntb==-1)
		{
			close(s);
			printf("\nMessage could not be sent.");
			exit(0);
		}
	}

	close(s);
}