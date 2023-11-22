#include "header.h"
//! MUST DO HTONS IF WE WANT SAME PORT NO TO BE SHOWN IN SNIFFER AS THAT ONLY WOULD BE CORRECT PORT
//! ALWAYS TO HTONS
using namespace std;
void error(string s)
{
	perror(s.c_str());
	exit(1);
}
int port;
int main(int argc, char *argv[])
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		error("socket");

	struct sockaddr_in serveraddr, clientaddr;

	//!!! DONT BIND IN CLIENT IF THIS SHOULD TO WORK
	// clientaddr.sin_family = AF_INET;
	// clientaddr.sin_port = htons(9999); //! MUST DO THIS IF WE WANT SAME PORT NO TO BE SHOWN IN PACKET SNIFFER
	// clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// if (bind(sfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
	// 	error("bind");

	cout << "enter port of which service needed\n";
	cin >> port;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(port); //! MUST DO THIS IF WE WANT SAME PORT NO TO BE SHOWN IN PACKET SNIFFER
	serveraddr.sin_family = AF_INET;

	if (connect(sfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		error("connect");

	char buf[1024] = "This client wants ";
	strcat(buf, to_string(port).c_str());

	int n = send(sfd, buf, sizeof(buf), 0);
	if (n < 0)
		error("send");
}