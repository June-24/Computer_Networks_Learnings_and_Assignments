#include<bits/stdc++.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;
#define PORT 8080
int main()
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serv_addr;
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(PORT);
	
	inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);
	
	connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	
	char buf1[100]="Hello I am from client";
	send(sfd,buf1,strlen(buf1),0);
	return 0;
}

