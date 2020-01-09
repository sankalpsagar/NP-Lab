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
	int s, r, recb, sntb, x, ns, a=0;
	printf("INPUT port number: ");
	scanf("%d", &x);
	socklen_t len;
	struct sockaddr_in server, client;

	char buff[MAXSIZE];
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

	printf("\n");

	printf("\nType your message: ");
	scanf("%s", buff);

	sntb = send(ns, buff, sizeof(buff), 0);
	if (sntb==-1)
	{
		printf("\nMessage sending failed.");
		close(s);
		close(ns);
		exit(0);
	}

	printf("\nMessage sent!");

	printf("\nServer closing down.");
	printf("\n");
	close(ns);
	close(s);
}


