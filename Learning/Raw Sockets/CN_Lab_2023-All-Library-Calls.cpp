HEADER FILES
--------------------------------------------------------------------------------------------------------------------------------

#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>

/**
 This function is used to send file descriptor over Unix domain socket
 You can use this function with file descriptor return
 by any one of below functions
 1 .  socketpair();
 2 .  socket(AF_UNIX,...);
 3 .  socket(AF_LOCAL,...);
 @param socket file_descriptor_of_sender
 @param fd_to_send
*/
 

				SHARE MEMORY
-----------------------------------------------------------------------------------------------------------------------------------

	int state=1;
	key_t h=ftok(".",state++);	// value of state should on every program where this share memory is used
 	int shmid=shmget(h,sizeof(int),IPC_CREAT|0666);
 	share_memory=shmat(shmid,(const void*)0,0);
 	
 	
 				SEMAPHORE
-----------------------------------------------------------------------------------------------------------------------------------
 	
	void sem_wait(int semid)
	{
		struct sembuf sb;
		sb.sem_num=0;
		sb.sem_op=-1;
		sb.sem_flg=0;
		if((semop(semid,&sb,1))==-1)
		{
			perror("\nFailed to acquire semaphore.");
			exit(0);
		}
	}

	void sem_try_wait(int semid)
	{
		struct sembuf sb;
		sb.sem_num=0;
		sb.sem_op=-1;
		sb.sem_flg=IPC_NOWAIT;;
		return semop(semid,&sb,1);
	}

	void sem_signal(int semid)
	{
		struct sembuf sb;
		sb.sem_num=0;
		sb.sem_op=1;
		sb.sem_flg=0;
		if((semop(semid,&sb,1))==-1)
		{
			perror("\nFailed to release semaphore.");
			exit(0);
		}
	}

	int state=1;
	key_t h=ftok(".",state++);	// value of state should on every program where this semaphore is used
	int sem_id;
	if((sem_id=semget(h,1,0666|IPC_CREAT))==-1)
 	{
		printf("error in creation semaphore\n");
		exit(0);
	}
	
	int semaphore_value=1;

	if((semctl(sem_id,0,SETVAL,semaphore_value))==-1)
	{
		printf("error to set value\n");
	}
	
	(OR)
	
#define sname "/mysem"

sem_t *sem = sem_open(sname, O_CREAT, 0644, 0);
sem_t *sem = sem_open(sname,1);

sem_wait(sem);
sem_post(sem);

----------------------------------------------------------------------------------------------------------------------------------
				MSG QUEUE
----------------------------------------------------------------------------------------------------------------------------------
	struct mymsg
	{
		long type;
		char msg[20];
	};

	struct mymsg msg1;
	key_t  key;
	int    mqpid;
	int    ret;
	int    len;
	system("touch f1.txt");
	if((key=ftok("f1.txt",'B')) == -1)
	{
		perror("key");
		exit(1);
	}
	if((mqpid=msgget(key,0644|IPC_CREAT))==-1)
	{
		perror("Key");
		exit(1);
	}
	
	if(msgsnd( mqpid ,&msg1 ,len+1 , 0) == -1)
	{
		perror("msgsnd");
		exit(1);
	}

	memset(msg1.msg,'\0',sizeof(msg1.msg));
	if(msgrcv( mqpid , &msg1 , sizeof(msg1.msg),1 ,0) == -1)
	{
		perror("msgrcv");
		exit(1);
	}	
----------------------------------------------------------------------------------------------------------------------------------
				FIFO
----------------------------------------------------------------------------------------------------------------------------------
	char name[50];
	if(mkfifo(name,0666)==-1)
	{
		perror("mkfifo()1");
		exit(1);
	}
	if((wfd=open("./wellknownfifo",O_WRONLY))==-1)
	{
		perror("open()");
		exit(1);
	}


	write(wfd,buffer,sizeof(buffer));

	
	char buffer[50];
	if(mkfifo("./wellknownfifo",0666)==-1)
	{
		perror("mkfifo()");
		exit(1);
	}
	if((rfd=open("./wellknownfifo",O_RDONLY))==-1)
	{
		perror("open()");
		exit(1);
	}

	read(rfd,buffer,50);

----------------------------------------------------------------------------------------------------------------------------------
 				SELECT
----------------------------------------------------------------------------------------------------------------------------------
	fd_set readset;
	FD_ZERO(&readset);
	
	int max=-1;
	
	for(i=0;i<no_of_file_descriptors;i++)
	{
		FD_SET(fd[i], &readset);
		if(fd[i]>max)
		max=fd[i];		
	}
	
	
	struct timeval t;
	t.tv_sec=3;
	t.tv_usec=100;
	int rv = select(max + 1, &readset, NULL, NULL, &t);

	if (rv == -1) 
	{
		perror("select");
	}
	
	else if (rv == 0) 
	{
    		printf("Timeout occurred!\n");
	} 
	
	else 
	{
		int i;
		// check for events 
		for(i=0;i<no_of_file_descriptors;i++)
    		if (FD_ISSET(fd[i], &readset)) 
		{

    		}
	}
 	
				pthread
----------------------------------------------------------------------------------------------------------------------------------
 	
 	void do_thread_service(void *arg)
	{
		int *args= (int*)arg ;
 	
	}
	
	pthread_t t_service;
 	if(pthread_create(&t_service,NULL,(void*)&do_thread_service ,(void*)args)!=0)
	perror("\npthread_create ");
 	



		           CONNECTION ORIENTED SERVER	( usage -:  "./a.out port_no")
---------------------------------------------------------------------------------------------------------------------------------
	if(argc!=2)
	printf("\n usage ./a.out port_no");

	int sfd;
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_len;
	int port_no=atoi(argv[1]);

	if((sfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully");

	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	int opt=1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");

	listen(sfd,10);

	cli_len=sizeof(cli_addr);
	int nsfd;
	if((nsfd = accept(sfd , (struct sockaddr *)&cli_addr , &cli_len))==-1)
	perror("\n accept ");
	else printf("\n accept successful");
	//break after exec in child


		          CONNECTION ORIENTED CLIENT	( usage -:  "./a.out port_no")
---------------------------------------------------------------------------------------------------------------------------------

	if(argc!=2)
	printf("\n usage ./a.out port_no");

	int sfd;
	struct sockaddr_in serv_addr;
	int port_no=atoi(argv[1]);

	bzero(&serv_addr,sizeof(serv_addr));

	if((sfd = socket(AF_INET , SOCK_STREAM , 0))==-1)
	perror("\n socket");
	else printf("\n socket created successfully\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	//serv_addr.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr);

	if(connect(sfd , (struct sockaddr *)&serv_addr , sizeof(serv_addr))==-1)
	perror("\n connect : ");
	else printf("\nconnect succesful");



			           CONNECTION LESS SERVER	( usage -:  "./a.out port_no")
---------------------------------------------------------------------------------------------------------------------------------
	if(argc!=2)
	printf("\n usage ./a.out port_no");

	int sfd;
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_len;
	int port_no=atoi(argv[1]);

	if((sfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully");

	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");

	cli_len = sizeof(cli_addr);

	fgets( buffer , 256 , stdin );
	sendto(sfd , buffer , 256 , 0 , ( struct sockaddr * ) &cli_addr ,  cli_len);
	recvfrom(sfd , buffer , 256 , 0 , ( struct sockaddr * ) &cli_addr , & cli_len );

		          CONNECTION LESS CLIENT	( usage -:  "./a.out port_no")
---------------------------------------------------------------------------------------------------------------------------------

	if(argc!=2)
	printf("\n usage ./a.out port_no");

	int sfd;
	struct sockaddr_in serv_addr;
	int port_no=atoi(argv[1]);
	char buffer[256];

	bzero(&serv_addr,sizeof(serv_addr));

	if((sfd = socket(AF_INET , SOCK_DGRAM , 0))==-1)
	perror("\n socket");
	else printf("\n socket created successfully\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	socklen_t serv_len = sizeof(serv_addr);
	
	fgets( buffer , 256 , stdin );
	sendto(sfd , buffer , 256 , 0 , ( struct sockaddr * ) &serv_addr ,  serv_len);
	recvfrom(sfd , buffer , 256 , 0 , ( struct sockaddr * ) &serv_addr , & serv_len );

		           UNIX SOCKET CONNECTION ORIENTED SERVER	( usage -:  "./a.out")
---------------------------------------------------------------------------------------------------------------------------------
	#define ADDRESS  "mysocket"

	int  usfd;
	struct sockaddr_un userv_addr,ucli_addr;
  	int userv_len,ucli_len;

	usfd = socket(AF_UNIX , SOCK_STREAM , 0);
	perror("socket");

  	bzero(&userv_addr,sizeof(userv_addr));

  	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	userv_len = sizeof(userv_addr);

	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
	perror("server: bind");

	listen(usfd, 5);

	ucli_len=sizeof(ucli_addr);

	int nusfd;
	nusfd=accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);


		          UNIX SOCKET CONNECTION ORIENTED CLIENT	( usage -:  "./a.out")
---------------------------------------------------------------------------------------------------------------------------------

	#define ADDRESS     "mysocket"

	int usfd;
	struct sockaddr_un userv_addr;
  	int userv_len,ucli_len;

  	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

  	if(usfd==-1)
  	perror("\nsocket  ");

  	bzero(&userv_addr,sizeof(userv_addr));
  	userv_addr.sun_family = AF_UNIX;
   	strcpy(userv_addr.sun_path, ADDRESS);

	userv_len = sizeof(userv_addr);

	if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1)
	perror("\n connect ");

	else printf("\nconnect succesful");



				SEND_FD AND RECV_FD
---------------------------------------------------------------------------------------------------------------------

int send_fd(int socket, int fd_to_send)
 {
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);
 }
 
 
 
 
 
 int recv_fd(int socket)
 {
  int sent_fd, available_ancillary_element_buffer_space;
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&socket_message, 0, sizeof(struct msghdr));
  memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
   return -1;

  if(message_buffer[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(control_message = CMSG_FIRSTHDR(&socket_message);
       control_message != NULL;
       control_message = CMSG_NXTHDR(&socket_message, control_message))
  {
   if( (control_message->cmsg_level == SOL_SOCKET) &&
       (control_message->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(control_message));
    return sent_fd;
   }
  }

  return -1;
 }




		           UNIX SOCKET CONNECTION LESS SERVER	( usage -:  "./a.out")
---------------------------------------------------------------------------------------------------------------------------------
	#define ADDRESS  "mysocket"

	int  usfd;
	struct sockaddr_un userv_addr,ucli_addr;
  	int userv_len,ucli_len;

	usfd = socket(AF_UNIX , SOCK_DGRAM , 0);
	perror("socket");

  	bzero(&userv_addr,sizeof(userv_addr));

  	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	userv_len = sizeof(userv_addr);

	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
	perror("server: bind");

	fgets( buffer , 256 , stdin );
	sendto(usfd , buffer , 256 , 0 , ( struct sockaddr * ) &ucli_addr ,  ucli_len);
	recvfrom(sfd , buffer , 256 , 0 , ( struct sockaddr * ) &ucli_addr , & uscli_len );


		          UNIX SOCKET CONNECTION LESS CLIENT	( usage -:  "./a.out")
---------------------------------------------------------------------------------------------------------------------------------

	#define ADDRESS     "mysocket"

	int usfd;
	struct sockaddr_un userv_addr;
  	int userv_len,ucli_len;

  	usfd = socket(AF_UNIX, SOCK_DGRAM, 0);

  	if(usfd==-1)
  	perror("\nsocket  ");

  	bzero(&userv_addr,sizeof(userv_addr));
  	userv_addr.sun_family = AF_UNIX;
   	strcpy(userv_addr.sun_path, ADDRESS);

	userv_len = sizeof(userv_addr);
	
	fgets( buffer , 256 , stdin );
	sendto(sfd , buffer , 256 , 0 , ( struct sockaddr * ) &userv_addr ,  userv_len);
	recvfrom(sfd , buffer , 256 , 0 , ( struct sockaddr * ) &userv_addr , & userv_len );


		         SOCKET PAIR	( usage -:  "./a.out")
---------------------------------------------------------------------------------------------------------------------------------

	int usfd[2];
	if(socketpair(AF_UNIX,SOCK_STREAM,0,usfd)==-1)
	perror("socketpair ");
			
	int c=fork();
			
	if(c==-1)
	perror("\nfork ");
			
	else if(c>0)
	{
		close(usfd[1]);
	}
			
	else if(c==0)
	{
		close(usfd[0]);	
		dup2(usfd[1],0);
		execvp(file_name,args);
	}
	
	
	RAW SOCKETS
---------------------------------------------------------------------------------------------------------------------------------

void print_ipheader(struct iphdr* ip)
{
	cout<<"------------------------\n";
	cout<<"Printing IP header....\n";
	cout<<"IP version:"<<(unsigned int)ip->version<<endl;
	cout<<"IP header length:"<<(unsigned int)ip->ihl<<endl;
	
	cout<<"Type of service:"<<(unsigned int)ip->tos<<endl;
	cout<<"Total ip packet length:"<<ntohs(ip->tot_len)<<endl;
	cout<<"Packet id:"<<ntohs(ip->id)<<endl;
	cout<<"Time to leave :"<<(unsigned int)ip->ttl<<endl;
	cout<<"Protocol:"<<(unsigned int)ip->protocol<<endl;
	cout<<"Check:"<<ip->check<<endl;
	cout<<"Source ip:"<<inet_ntoa(*(in_addr*)&ip->saddr)<<endl;
	//printf("%pI4\n",&ip->saddr );
	cout<<"Destination ip:"<<inet_ntoa(*(in_addr*)&ip->daddr)<<endl;
	cout<<"End of IP header\n";
	cout<<"------------------------\n";
}

----------------------------------------------------------------------------------------------------------------------------------
void print_tcpheader(struct tcphdr* tcph)
{
        cout<<"source port number = "<<tcph->source<<endl;
	cout<<"destination port number = "<<tcph->dest<<endl;
	cout<<"acknowledgement number = "<<tcph->ack_seq<<endl;
	cout<<"size of TCP Header in full word ="<<tcph->doff<<endl;
	cout<<"No more data from sender "<<tcph->fin<<endl;
	cout<<"Synchronize sequence numbers "<<tcph->syn<<endl;
	cout<<"Reset the connection "<<tcph->rst<<endl;
	cout<<"Push Function"<<tcph->psh<<endl;
	cout<<"Acknowledgment field significant "<<tcph->ack<<endl;
	cout<<"Urgent Pointer field significant "<<tcph->urg<<endl;
	cout<<"window size "<<tcph->window<<endl;
	cout<<"checksum "<<tcph->check<<endl;
	cout<<"urgent pointer "<<tcph->urg_ptr<<endl;
}
-----------------------------------------------------------------------------------------------------------------------------------
void print_ethheader(struct ethhdr* eth)
{
    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
    eth->h_dest[0], eth->h_dest[1],
    eth->h_dest[2], eth->h_dest[3],
    eth->h_dest[4], eth->h_dest[5]);
    
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->h_source[0], eth->h_source[1],
           eth->h_source[2], eth->h_source[3],
           eth->h_source[4], eth->h_source[5]);
    printf("protocol : 0x%04x\n", ntohs(eth->h_proto));
}
-----------------------------------------------------------------------------------------------------------------------------------
 RAW SOCKET SERVER
-----------------------------------------------------------------------------------------------------


	if(argc<2)cout<<"Enter protocal in arguments";
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	perror("socket");
	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	cout<<"opt"<<endl;
		struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

		char buff[]="hello";	
	client.sin_addr.s_addr=INADDR_ANY;

	unsigned int client_len=sizeof(client);
	cout<<"sending"<<endl;
  sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
  perror("send");
  
  
  RAW SOCKET CLIENT
---------------------------------------------------------------------------------------------------------------------------------

	if(argc<2)cout<<"Enter protocol in arguments\n";
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd==-1)custom_perror("socket")
		char buf[BUF_LEN];
	struct sockaddr_in client;
	socklen_t clilen=sizeof(client);
	cout<<"receive"<<endl;
	recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)clilen);
	perror("recv");
	 struct iphdr *ip;
  ip=(struct iphdr*)buf;
	cout<<(buf+(ip->ihl)*4)<<endl;
	

				GETPEERNAME (usage: only after accept; only on nsfd)
---------------------------------------------------------------------------------------------------------------------------------

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <stdio.h>


{
   int s;
   struct sockaddr_in peer;
   int peer_len;
   
   peer_len = sizeof(peer);
     
   if (getpeername(s, &peer, &peer_len) == -1) {
      perror("getpeername() failed");
      return -1;
   }

      /* Print it.    */
   printf("Peer's IP address is: %s\n", inet_ntoa(peer.sin_addr));
   printf("Peer's port is: %d\n", (int) ntohs(peer.sin_port));
   
}

PASSING ARGUMENTS THROUGH EXEC
-------------------------------------------------------------------------------------------------------------------------------
string msg;
char **arg=new char*[2];
arg[0]=strdup(msg.c_str());
arg[1]=NULL;
int c=fork();
if(c>0);
else if(c==0)
{
	if(execvp("./s",arg)==-1)
	cout<<"eroor"<<endl;
	exit(1);
}


//retrieving in child

int main(int argc, char const *argv[])
{
	string info=argv[argc];
}


 MKFIFO
 ------------------------------------------------------------------------------------------------------------------------------
 
 #include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

  int fd;
  mkfifo("fifo1.fifo",0666);
  fd=open("./fifo1.fifo",O_RDONLY);
             
             POLL
--------------------------------------------------------------------------------------------------------------------------------
	int size;
	struct pollfd fds[size];
	fds[i]=open(" ", 0666);
	fds[i].events=POLLIN;
	
	int ret=poll(fds, size, timeout);
	if(fds[i].revents & POLLIN) 
	{
	
	}
	
				LIBPCAP
-------------------------------------------------------------------------------------------------------------------------------

#include<pcap.h>
#include<stdio.h>
#include<stdlib.h> // for exit()
#include<string.h> //for memset
 
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
 
void process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
void process_ip_packet(const u_char * , int);
void print_ip_packet(const u_char * , int);
void print_tcp_packet(const u_char *  , int );
void print_udp_packet(const u_char * , int);
void print_icmp_packet(const u_char * , int );
void PrintData (const u_char * , int);
 
FILE *logfile; //to store the output
struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j; 
 
int main()
{
    pcap_if_t *alldevsp , *device;
    pcap_t *handle; //Handle of the device that shall be sniffed
 
    char errbuf[100] , *devname , devs[100][100];
    int count = 1 , n;
     
    // get the list of available devices
    printf("Finding available devices ... ");
    if( pcap_findalldevs( &alldevsp , errbuf) )
    {
        printf("Error finding devices : %s" , errbuf);
        exit(1);
    }
    printf("Done");
     
    //Print the available devices
    printf("\nAvailable Devices are :\n");
    for(device = alldevsp ; device != NULL ; device = device->next)
    {
        printf("%d. %s - %s\n" , count , device->name , device->description);
        if(device->name != NULL)
        {
            strcpy(devs[count] , device->name);
        }
        count++;
    }
     
    //Ask user which device to sniff
    printf("Enter the number of the device you want to sniff : ");
    scanf("%d" , &n);
    devname = devs[n];
     
    //Open the device for sniffing
    printf("Opening device %s for sniffing ... " , devname);
    handle = pcap_open_live(devname , 65535 , 1 , 0 , errbuf);
     
    if (handle == NULL) 
    {
        fprintf(stderr, "Couldn't open device %s : %s\n" , devname , errbuf);
        exit(1);
    }
    printf("Done\n");
     
    logfile=fopen("log.txt","w");
    if(logfile==NULL) 
    {
        printf("Unable to create file.");
    }
     
    //Put the device in sniff loop
    pcap_loop(handle , 0 , process_packet , NULL); 
    return 0;   
}
 
void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
    int size = header->len;
     
    //Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ++total;
    switch (iph->protocol) //Check the Protocol and do accordingly...
    {
        case 1:  //ICMP Protocol
            ++icmp;
            print_icmp_packet( buffer , size);
            break;
         
        case 2:  //IGMP Protocol
            ++igmp;
            break;
         
        case 6:  //TCP Protocol
            ++tcp;
            print_tcp_packet(buffer , size);
            break;
         
        case 17: //UDP Protocol
            ++udp;
            print_udp_packet(buffer , size);
            break;
         
        default: //Some Other Protocol like ARP etc.
            ++others;
            break;
    }
    printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d\r", tcp , udp , icmp , igmp , others , total);
}
 
void print_ethernet_header(const u_char *Buffer, int Size)
{
    struct ethhdr *eth = (struct ethhdr *)Buffer;
     
    fprintf(logfile , "\n");
    fprintf(logfile , "Ethernet Header\n");
    fprintf(logfile , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    fprintf(logfile , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    fprintf(logfile , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}
 
void print_ip_header(const u_char * Buffer, int Size)
{
    print_ethernet_header(Buffer , Size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    fprintf(logfile , "   |-IP Version        : %d\n",(unsigned int)iph->version);
    fprintf(logfile , "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    fprintf(logfile , "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    fprintf(logfile , "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    fprintf(logfile , "   |-Identification    : %d\n",ntohs(iph->id));
    fprintf(logfile , "   |-TTL      : %d\n",(unsigned int)iph->ttl);
    fprintf(logfile , "   |-Protocol : %d\n",(unsigned int)iph->protocol);
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(iph->check));
    fprintf(logfile , "   |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
    fprintf(logfile , "   |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
}
 
void print_tcp_packet(const u_char * Buffer, int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
             
    int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
     
    fprintf(logfile , "\n\n***********************TCP Packet*************************\n");  
         
    print_ip_header(Buffer,Size);
         
    fprintf(logfile , "\n");
    fprintf(logfile , "TCP Header\n");
    fprintf(logfile , "   |-Source Port      : %u\n",ntohs(tcph->source));
    fprintf(logfile , "   |-Destination Port : %u\n",ntohs(tcph->dest));
    fprintf(logfile , "   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    fprintf(logfile , "   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    fprintf(logfile , "   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    fprintf(logfile , "   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    fprintf(logfile , "   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    fprintf(logfile , "   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    fprintf(logfile , "   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    fprintf(logfile , "   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    fprintf(logfile , "   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    fprintf(logfile , "   |-Window         : %d\n",ntohs(tcph->window));
    fprintf(logfile , "   |-Checksum       : %d\n",ntohs(tcph->check));
    fprintf(logfile , "   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    fprintf(logfile , "\n");
    fprintf(logfile , "                        DATA Dump                         ");
    fprintf(logfile , "\n");
         
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile , "TCP Header\n");
    PrintData(Buffer+iphdrlen,tcph->doff*4);
         
    fprintf(logfile , "Data Payload\n");    
    PrintData(Buffer + header_size , Size - header_size );
                         
    fprintf(logfile , "\n###########################################################");
}
 
void print_udp_packet(const u_char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
     
    fprintf(logfile , "\n\n***********************UDP Packet*************************\n");
     
    print_ip_header(Buffer,Size);           
     
    fprintf(logfile , "\nUDP Header\n");
    fprintf(logfile , "   |-Source Port      : %d\n" , ntohs(udph->source));
    fprintf(logfile , "   |-Destination Port : %d\n" , ntohs(udph->dest));
    fprintf(logfile , "   |-UDP Length       : %d\n" , ntohs(udph->len));
    fprintf(logfile , "   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer , iphdrlen);
         
    fprintf(logfile , "UDP Header\n");
    PrintData(Buffer+iphdrlen , sizeof udph);
         
    fprintf(logfile , "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , Size - header_size);
     
    fprintf(logfile , "\n###########################################################");
}
 
void print_icmp_packet(const u_char * Buffer , int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
     
    fprintf(logfile , "\n\n***********************ICMP Packet*************************\n"); 
     
    print_ip_header(Buffer , Size);
             
    fprintf(logfile , "\n");
         
    fprintf(logfile , "ICMP Header\n");
    fprintf(logfile , "   |-Type : %d",(unsigned int)(icmph->type));
             
    if((unsigned int)(icmph->type) == 11)
    {
        fprintf(logfile , "  (TTL Expired)\n");
    }
    else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
    {
        fprintf(logfile , "  (ICMP Echo Reply)\n");
    }
     
    fprintf(logfile , "   |-Code : %d\n",(unsigned int)(icmph->code));
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(icmph->checksum));
    //fprintf(logfile , "   |-ID       : %d\n",ntohs(icmph->id));
    //fprintf(logfile , "   |-Sequence : %d\n",ntohs(icmph->sequence));
    fprintf(logfile , "\n");
 
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile , "UDP Header\n");
    PrintData(Buffer + iphdrlen , sizeof icmph);
         
    fprintf(logfile , "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , (Size - header_size) );
     
    fprintf(logfile , "\n###########################################################");
}
 
void PrintData (const u_char * data , int Size)
{
   //u_char *ptr=(u_char *)data;
 //  const char* S1 = reinterpret_cast<const char*>(data);
  // fprintf(logfile,"%s\n",S1);
    int i , j;
    for(i=0 ; i < Size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            fprintf(logfile , "         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    fprintf(logfile , "%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else fprintf(logfile , "."); //otherwise print a dot
            }
            fprintf(logfile , "\n");
        } 
         
        if(i%16==0) fprintf(logfile , "   ");
            fprintf(logfile , " %02X",(unsigned int)data[i]);
                 
        if( i==Size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++) 
            {
              fprintf(logfile , "   "); //extra spaces
            }
             
            fprintf(logfile , "         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) 
                {
                  fprintf(logfile , "%c",(unsigned char)data[j]);
                }
                else
                {
                  fprintf(logfile , ".");
                }
            }
             
            fprintf(logfile ,  "\n" );
        }
    }
}


//usage:
g++ filename.cpp -o file -lpcap
sudo ./file
-------------------------------------------------------------------------------------------------------------------------------	
			RPC
-------------------------------------------------------------------------------------------------------------------------------
  .x file
  
  struct arg{
	
};
program ADD_PROG{
	version ADD_VERS{
		return_type fun(arg)=1;
	}=1;
}=0x23451111;

usage:
rpcgen -a -C file.x
make -f Makefile.file
./file_server
./file_client localhost args

------------------------------------------------------------------------------------------------------------------------------------
			ARP REPLY
------------------------------------------------------------------------------------------------------------------------------------
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>       // IPPROTO_RAW, INET_ADDRSTRLEN
	#include <netinet/ip.h>       // IP_MAXPACKET (which is 65535)
	#include <net/if.h>
	#include <linux/if_packet.h>
	#include <net/ethernet.h>

struct arphdr
{
        uint16_t htype;		//Hardware type
        uint16_t ptype;		//Protocol type
        uint8_t hlen;		//Hardware address length
        uint8_t plen;		//Protocol address length
  	uint16_t opcode;	//Operation code
        uint8_t sender_mac[6];  
        uint8_t sender_ip[4];
        uint8_t target_mac[6];
	uint8_t target_ip[4]; 

/*
    #define    ARPOP_REQUEST	1	// request to resolve address 
    #define    ARPOP_REPLY	2	// response to previous request 
    #define    ARPOP_REVREQUEST 3	// request protocol address given hardware
    #define    ARPOP_REVREPLY	4	// response giving protocol address 
    #define    ARPOP_INVREQUEST 8 	// request to identify peer 
    #define    ARPOP_INVREPLY	9	// response identifying peer 
*/

};

int sfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	char  interface[40];
	strcpy (interface, argv[5]);
	if(sfd==-1)
	{
		perror("socket");
	}
	char* buf = (char*)malloc(1500);
	uint8_t src[6],dst[6];

	//usr mac address
	src[0] = 0x8C;
	src[1] = 0x16;
	src[2] = 0x45;
	src[3] = 0xCE;
	src[4] = 0x8B;
	src[5] = 0xE4;

	//local gateway mac address
	dst[0] = 0x00;
	dst[1] = 0x25;
	dst[2] = 0x83;
	dst[3] = 0x70;
	dst[4] = 0x10;
	dst[5] = 0x00;
	
	memcpy(buf,dst,6*(sizeof (uint8_t)));
	memcpy(buf+6*(sizeof (uint8_t)),src,6*(sizeof (uint8_t)));
	
	buf[12] = ETH_P_ARP / 256;
	buf[13] = ETH_P_ARP % 256;
	
	struct arphdr* arp = (struct arphdr*)(buf+14);
	arp->htype = htons(1);		//because we use ethernet
	arp->ptype = 8;	// ETH_P_IP = 0x0800 
	arp->hlen = 6;
	arp->plen = 4;
	arp->opcode = htons(2);		// ARP reply
	
	memcpy(arp->sender_mac ,src,6*(sizeof(uint8_t)));
	memcpy(arp->target_mac ,dst,6*(sizeof(uint8_t)));
	
	// Friend's IP
	arp->sender_ip[0] = atoi(argv[1]);
	arp->sender_ip[1] = atoi(argv[2]);
	arp->sender_ip[2] = atoi(argv[3]);
	arp->sender_ip[3] = atoi(argv[4]);
	
	//Gateway IP
	arp->target_ip[0] = 172;
	arp->target_ip[1] = 30;
	arp->target_ip[2] = 100;
	arp->target_ip[3] = 1;
	
	memcpy(buf+14,arp,28);
	
	int bytes;
		
        struct sockaddr_ll device;
        memset (&device, 0, sizeof (device));
	if ((device.sll_ifindex = if_nametoindex (interface)) == 0)
	{
		perror ("if_nametoindex() failed to obtain interface index ");
		exit (EXIT_FAILURE);
	}
	printf ("Index for interface %s is %i\n", interface, device.sll_ifindex);
        device.sll_family = AF_PACKET;
 	memcpy (device.sll_addr, dst, 6 * sizeof (uint8_t));
	device.sll_halen = 6;
				    
	while(1)	
        {
  	      if ((bytes = sendto (sfd, buf,42, 0, (struct sockaddr *) &device, sizeof (device))) <= 0) 
	      {
			perror ("sendto() failed");
			exit (EXIT_FAILURE);
	      }
        }
-----------------------------------------------------------------------------------------------------------------------------------------
				Own Ethernet Header( 14 bytes [6+6+2] )
-----------------------------------------------------------------------------------------------------------------------------------------
struct ether_header
{
  uint8_t  ether_dhost[ETH_ALEN];	/* destination eth addr	*/
  uint8_t  ether_shost[ETH_ALEN];	/* source ether addr	*/
  uint16_t ether_type;		        /* packet type ID field	*/
};
	char* buf = (char*)malloc(1500);
	uint8_t src[6],dst[6];

	//usr mac address(6  bytes)
	src[0] = 0x8C;
	src[1] = 0x16;
	src[2] = 0x45;
	src[3] = 0xCE;
	src[4] = 0x8B;
	src[5] = 0xE4;

	//destination mac address( 6 bytes )
	dst[0] = 0x00;
	dst[1] = 0x25;
	dst[2] = 0x83;
	dst[3] = 0x70;
	dst[4] = 0x10;
	dst[5] = 0x00;
	
	memcpy(buf,dst,6*(sizeof (uint8_t)));
	memcpy(buf+6*(sizeof (uint8_t)),src,6*(sizeof (uint8_t)));
	
	//packet_type (2 bytes)
	buf[12] = ETH_P_ARP / 256;
	buf[13] = ETH_P_ARP % 256;
-----------------------------------------------------------------------------------------------------------------------------------------
			  IP Header
-----------------------------------------------------------------------------------------------------------------------------------------
struct myipheader
 {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error	"Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */
 };

	struct myipheader *ip;
        char   buffer[1000];

        memset(buffer,'\0',sizeof(buffer));
        ip=(struct myipheader*)buffer;
        ip->version=4;
        ip->ttl=64;
        ip->id=0;
        ip->ihl=5;
        ip->protocol=100;	// value same as last argument(integer) of raw socket creation sys_call
        ip->saddr=inet_addr("1.2.3.4");
        ip->daddr=inet_addr("127.0.0.2");
        ip->tot_len=1000;
        
        printf("Msg:");
        scanf("%s",buffer+60);
        if(sendto(rsfd,buffer,1000,0,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
        {
            perror("sendto");
            exit(1);
        }	
------------------------------------------------------------------------------------------------------------------------------------------
				TCP Header
------------------------------------------------------------------------------------------------------------------------------------------
	


		struct tcphdr
		  {
		    __extension__ union
		    {
		      struct
		      {
			uint16_t th_sport;	/* source port */
			uint16_t th_dport;	/* destination port */
			tcp_seq th_seq;		/* sequence number */
			tcp_seq th_ack;		/* acknowledgement number */
		# if __BYTE_ORDER == __LITTLE_ENDIAN
			uint8_t th_x2:4;	/* (unused) */
			uint8_t th_off:4;	/* data offset */
		# endif
		# if __BYTE_ORDER == __BIG_ENDIAN
			uint8_t th_off:4;	/* data offset */
			uint8_t th_x2:4;	/* (unused) */
		# endif
			uint8_t th_flags;
		# define TH_FIN	0x01
		# define TH_SYN	0x02
		# define TH_RST	0x04
		# define TH_PUSH	0x08
		# define TH_ACK	0x10
		# define TH_URG	0x20
			uint16_t th_win;	/* window */
			uint16_t th_sum;	/* checksum */
			uint16_t th_urp;	/* urgent pointer */
		      };
		      struct
		      {
			uint16_t source;
			uint16_t dest;
			uint32_t seq;
			uint32_t ack_seq;
		# if __BYTE_ORDER == __LITTLE_ENDIAN
			uint16_t res1:4;
			uint16_t doff:4;
			uint16_t fin:1;
			uint16_t syn:1;
			uint16_t rst:1;
			uint16_t psh:1;
			uint16_t ack:1;
			uint16_t urg:1;
			uint16_t res2:2;
		# elif __BYTE_ORDER == __BIG_ENDIAN
			uint16_t doff:4;
			uint16_t res1:4;
			uint16_t res2:2;
			uint16_t urg:1;
			uint16_t ack:1;
			uint16_t psh:1;
			uint16_t rst:1;
			uint16_t syn:1;
			uint16_t fin:1;
		# else
		#  error "Adjust your <bits/endian.h> defines"
		# endif
			uint16_t window;
			uint16_t check;
			uint16_t urg_ptr;
		      };
		    };
		};
		
		
	// sample initialization
	
	u_char buffer[1500];
	struct ether_header* eth;
	struct iphdr* ip;
	struct tcphdr* tcp;
	
	tcp = (struct tcphdr*)(buffer+14+(ip->ihl*4));
	
	tcp->dest = //destination port;
	tcp->source = //source port;
	tcp->doff = // length of tcp header
	tcp->seq = // sequence number;
	tcp->ack_seq = // acknowledgement number
	tcp->syn = // SYN bit
	tcp->urg = // urgent bit
	tcp->window = // window size
-------------------------------------------------------------------------------------------------------------------------------------------
			UDP Header
------------------------------------------------------------------------


struct udphdr
{
  __extension__ union
  {
    struct
    {
      uint16_t uh_sport;	/* source port */
      uint16_t uh_dport;	/* destination port */
      uint16_t uh_ulen;		/* udp length */
      uint16_t uh_sum;		/* udp checksum */
    };
    struct
    {
      uint16_t source;
      uint16_t dest;
      uint16_t len;
      uint16_t check;
    };
  };
};


#include<stdio.h>;	//for printf
#include<string.h>; //memset
#include<sys/socket.h>;	//for socket ofcourse
#include<stdlib.h>; //for exit(0);
#include<errno.h>; //For errno - the error number
#include<netinet/udp.h>;	//Provides declarations for udp header
#include<netinet/ip.h>;	//Provides declarations for ip header

/* 
	96 bit (12 bytes) pseudo header needed for udp header checksum calculation 
*/
struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

/*
	Generic checksum calculation function
*/
unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>;1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

	/Create a raw socket of type IPPROTO
	int s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
	
	if(s == -1)
	{
		//socket creation failed, may be because of non-root privileges
		perror("Failed to create raw socket");
		exit(1);
	}
	
	//Datagram to represent the packet
	char datagram[4096] , source_ip[32] , *data , *pseudogram;
	
	//zero out the packet buffer
	memset (datagram, 0, 4096);

	//Datagram to represent the packet
	char datagram[4096] , source_ip[32] , *data , *pseudogram;
	
	//zero out the packet buffer
	memset (datagram, 0, 4096);
	
	//IP header
	struct iphdr *iph = (struct iphdr *) datagram;
	
	//UDP header
	struct udphdr *udph = (struct udphdr *) (datagram + sizeof (struct ip));
	
	struct sockaddr_in sin;
	struct pseudo_header psh;
	
	//Data part
	data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
	strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	//some address resolution
	strcpy(source_ip , "192.168.1.2");
	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	sin.sin_addr.s_addr = inet_addr ("192.168.1.1");
	
	//Fill in the IP Header
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
	iph->id = htonl (54321);	//Id of this packet
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_UDP;
	iph->check = 0;		//Set to 0 before calculating checksum
	iph->saddr = inet_addr ( source_ip );	//Spoof the source ip address
	iph->daddr = sin.sin_addr.s_addr;
	
	//Ip checksum
	iph->check = csum ((unsigned short *) datagram, iph->tot_len);
	
	//UDP header
	udph->source = htons (6666);
	udph->dest = htons (8622);
	udph->len = htons(8 + strlen(data));	//udp header size
	udph->check = 0;	//leave checksum 0 now, filled later by pseudo header
	
	//Now the UDP checksum using the pseudo header
	psh.source_address = inet_addr( source_ip );
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );
	
	int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
	pseudogram = malloc(psize);
	
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
	
	udph->check = csum( (unsigned short*) pseudogram , psize);
	
	//Send the packet
	if (sendto (s, datagram, iph->tot_len ,	0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
	{
		perror("sendto failed");
	}
	//Data send successfully
	else
	{
		printf ("Packet Send. Length : %d \n" , iph->tot_len);
	}
	

------------------------------------------------------------------------------------------------------------------------------------------
				Pcap filter Code
------------------------------------------------------------------------------------------------------------------------------------------

    s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);perror("socket"); 
    char *device = "lo";
    struct bpf_program filter;
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    char filter_exp[] = "src host 127.0.0.1 || src host 127.0.0.2";
    bpf_u_int32 subnet_mask, ip;

    /* Snapshot length is how many bytes to capture from each packet. This includes*/
    int snapshot_length = 1024;

    /* End the loop after this many packets are captured */
    int total_packet_count = 10;
    u_char *my_arguments = NULL;
    pcap_lookupnet(device, &ip, &subnet_mask, error_buffer) ;
    handle = pcap_open_live(device, snapshot_length, 0, 10000, error_buffer);
    pcap_compile(handle, &filter, filter_exp, 0, ip) ;
    pcap_setfilter(handle, &filter);
    pcap_loop(handle, total_packet_count, my_packet_handler, my_arguments);

-------------------------------------------------------------------------------------------------------------------------------------------
			Generic checksum calculation function
------------------------------------------------------------------------------------------------------------------------------------------
unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>;1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

	sample call

	//Ip checksum
	iph->check = csum ((unsigned short *) data, iph->tot_len);
------------------------------------------------------------------------------------------------------------------------------------
				PCAP Monitor Mode
------------------------------------------------------------------------------------------------------------------------------------								
	char error_buffer[PCAP_ERRBUF_SIZE];
	pcap_t *handle = pcap_create("wlan0", error_buffer);
	pcap_set_rfmon(handler, 1);
	pcap_set_promisc(handler, 1); /* Capture packets that are not yours */
	pcap_set_snaplen(handler, 2048); /* Snapshot length */
	pcap_set_timeout(handler, 1000); /* Timeout in milliseconds */
	pcap_activate(handle);
------------------------------------------------------------------------------------------------------------------------------------
				PCAP My Handler
------------------------------------------------------------------------------------------------------------------------------------
void my_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
)

{
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("Not an IP packet. Skipping...\n\n");
        return;
    }
    const u_char *ip_header;
    const u_char *tcp_header;
    const u_char *payload;
    int ethernet_header_length = 14; /* Doesn't change */
    int ip_header_length;
    int tcp_header_length;
    int payload_length;
    ip_header = packet + ethernet_header_length;
    ip_header_length = ((*ip_header) & 0x0F);
    ip_header_length = ip_header_length * 4;
    printf("IP header length (IHL) in bytes: %d\n", ip_header_length);
    u_char protocol = *(ip_header + 9);
    if (protocol != IPPROTO_TCP) {
        printf("Not a TCP packet. Skipping...\n\n");
        return;
    }
    tcp_header = packet + ethernet_header_length + ip_header_length;
    tcp_header_length = ((*(tcp_header + 12)) & 0xF0) >> 4;
    tcp_header_length = tcp_header_length * 4;
    printf("TCP header length in bytes: %d\n", tcp_header_length);
    int total_headers_size = ethernet_header_length+ip_header_length+tcp_header_length;
    printf("Size of all headers combined: %d bytes\n", total_headers_size);
    payload_length = header->caplen -
        (ethernet_header_length + ip_header_length + tcp_header_length);
    printf("Payload size: %d bytes\n", payload_length);
    payload = packet + total_headers_size;
    printf("Memory address where payload begins: %p\n\n", payload);

    /*  
    if (payload_length > 0) {
        const u_char *temp_pointer = payload;
        int byte_count = 0;
        while (byte_count++ < payload_length) {
            printf("%c", *temp_pointer);
            temp_pointer++;
        }
        printf("\n");
    }
    */

    return;
}
------------------------------------------------------------------------------------------------------------------------------------





