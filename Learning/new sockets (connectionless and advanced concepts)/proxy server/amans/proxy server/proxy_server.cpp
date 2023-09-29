#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/poll.h>
#include <unistd.h>

using namespace std;

//----------------------------------------------------
//     All thread related data and structure here
//----------------------------------------------------

//This 2d matrix has all the fds
//The first row is all the fds that want to connect with special server 1
//The second row has fds that want special server 2 and so on
int arr[6][10];

//This has pointers of all the respective rows above
//will be used to poll
int pointers[6];

struct info{
    int ss;
};


//-----------------------------------------------------
//                  Thread Function
//-----------------------------------------------------

//Inside each thread we will pass the information as to which special server it should connect to

//Now every time a client specifies which server it wants, the main code will just add the fd of that
//particular client to the corrsponding row in the matrix and increase the pointer

//A new socket is made and connection is established between proxy server and special server

//Obviously we also need to map in the proxy server the socket that joins client to proxy with
//corresponsding socket from proxy server to special server

void * special_server(void * _arg){

    cout<<"Inside the thread now"<<endl;
    
    struct info* arg = (struct info*) _arg;
    int k = arg->ss;

    cout<<k<<endl;

    int curr = pointers[k];

    cout<<pointers[k]<<" "<<curr<<endl;


    //to map between client-server and server-server fds
    unordered_map<int,int> mp;  

    //poll
    struct pollfd pfd_c[10];    int cc=0;      //to poll on client side
    struct pollfd pfd_s[10];    int cs=0;     //to poll on server side

    while(1){

        // cout<<"Inside while loop"<<endl;

        //This implies someone new has come
        if(pointers[k]>curr){
            cout<<"Yahan bhi"<<endl;

            cout<<curr<<" "<<pointers[k]<<endl;
            

            //we poll the fd connecting client-server 
            struct pollfd temp;
            temp.fd = arr[k][curr];
            temp.events = POLLIN;
            pfd_c[cc]=temp;
            cc++;


            //--------------------------------------------------------------
            // We establish new connection between proxy and special server
            //--------------------------------------------------------------

            int sfd = socket(AF_INET , SOCK_STREAM , 0);
            if (sfd==-1) perror("Socket error");

            struct sockaddr_in server_address;
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(60001);
            server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

            //Connection
            int kk = connect (sfd , (sockaddr *)&server_address , sizeof(server_address));
            if (kk==-1) perror("Connection error");
            
            else{
                cout<<"Connected Successfully."<<endl;
                //We poll it
                struct pollfd temp2; 
                temp2.fd = sfd;
                temp2.events = POLLIN;
                pfd_s[cs] = temp2;
                cs++;

                //We add this into a map
                mp[arr[k][curr]] = sfd;
                mp[sfd] = arr[k][curr];
            }

            curr = pointers[k];

            cout<<curr<<" "<<pointers[k]<<endl;
        }

        int n = poll(pfd_c,cc,100);
        if(n>0){
            for(int i=0; i<cc; i++){
                if(pfd_c[i].revents && POLLIN){

                    //We simply recv the item and pass the buffer as it is to the appropriate client
                    char buffer[100];
                    int n = recv(pfd_c[i].fd,buffer,100,0);
                    buffer[n]='\0';

                    if(n>0){
                        int send_sfd = mp[pfd_c[i].fd];

                        send(send_sfd, buffer,strlen(buffer)+1,0);
                    }

                }
            }
        }

        n = poll(pfd_s,cs,100);
        if(n>0){
            for(int i=0; i<cs; i++){
                if(pfd_s[i].revents && POLLIN){

                    //We simply recv the item and pass the buffer as it is to the special server
                    char buffer[100];
                    int n = recv(pfd_s[i].fd,buffer,100,0);
                    buffer[n]='\0';

                    if(n>0){
                        int send_sfd = mp[pfd_s[i].fd];

                        send(send_sfd, buffer,strlen(buffer),0);
                    }

                }
            }
        }

    }
    
}

int main(){
    
    //Basic initialization
    for(int i=0; i<6; i++){
        pointers[i] = 0;
    }


    //--------------------------------------------------------
    //              First set up main socket
    //--------------------------------------------------------

    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1) perror("Socket error");


    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(50001);
    address.sin_addr.s_addr = INADDR_ANY;

    //Bind
    int a = bind (sfd , (struct sockaddr *)&address , sizeof(address));
    if (a == -1) perror("Bind error");

    //Listen
    int b = listen(sfd , 10);
    if (b == -1) perror("Listen error");

    if(a == -1 || b == -1) return 0;

    cout<<"Proxy Server up and running"<<endl;

    //-----------------------------------------------------------
    //-----------------------------------------------------------

    //vector to keep track of threads that have been spawned for the special servers
    // 0 means thread not there, 1 means there
    vector<int> v(6,0);


    //poll
    struct pollfd pfds[10];
    pfds[0].fd = sfd;
    pfds[0].events = POLLIN;
    int curr=1;

    while(1){
        int n = poll(pfds,curr,1000);
        // cout<<n<<endl;

        if(n==-1) perror("poll");
        
        else if(n==0) continue;

        else{

            for(int i=0; i<curr; i++){
                if(pfds[i].revents && POLLIN){
                    
                    if(i == 0){         //this means a new connection has come
            
                        struct sockaddr * client_address;
                        socklen_t len = sizeof(client_address);

                        int nsfd = accept(sfd, (struct sockaddr*)&client_address, &len);

                        cout<<"New client accepted"<<endl;
                        cout<<"NSFD is : "<<nsfd<<endl;

                        //now we are connected to it
                        //it will first send which server it wants to connect to
                        //so we need to listen to that here in the main area meawhile listening
                        //for other potential clients

                        //so we add it here in the poll list
                        struct pollfd temp;
                        temp.fd = nsfd;
                        temp.events = POLLIN;
                        pfds[curr] = temp;
                        
                        //we increase the curr :
                        curr++;

                        //When this client does finally tell us which server it wants
                        //then we will stop listening to it here and hand it over to
                        //the thread of the particular special server

                    }


                    //this is client telling us which special server it wants to connect to
                    else{

                        char buffer[10];
                        int n = recv(pfds[i].fd,buffer,10,0);

                        int service_required = buffer[0]-'0';
                        cout<<"service_required : "<<service_required<<endl;
                        

                        //Now regarding the service
                        // 1. If a thread is not available for it yet, we spawn one
                        // 2. Else we just go ahead and add this fd too there

                        if(!v[service_required]){
                            struct info *temp = new info();
                            temp->ss = service_required;

                            pthread_t t;
                            pthread_create(&t, NULL, special_server,(void*) temp);

                            v[service_required]=1;
                        }

                        sleep(2);
                        
                        //We also add this to our matrix
                        arr[service_required][pointers[service_required]] = pfds[i].fd;
                        pointers[service_required]++;

                        //we change the pfd array also
                        pfds[i].fd = 0;
                        //we change it to standard input as anyways that won't be used on the server

                    }
                }
            }
        }
    }
}