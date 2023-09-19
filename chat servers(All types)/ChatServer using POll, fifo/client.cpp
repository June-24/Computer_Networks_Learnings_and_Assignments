// #include <bits/stdc++.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <fstream>
// #include <sys/wait.h>
// #include <poll.h>
// using namespace std;

// int main ()
// {
//     // getting pid
//     string pid = to_string(getpid());
//     cout<<"process with "<<pid<<" has joined the chat room..... "<<endl;

//     // to make new fifo, its name should be the pid_pipe number
//     char makepipe[pid.length()+1];
//     for (int i=0;i<pid.length();i++)
//         makepipe[i] = pid[i];
//     makepipe[pid.length()] = '\0';
//     // making that fifo
//     mkfifo(makepipe , 0666);

//     const char * pid_pipe = pid.c_str();
//     int wfd = open("NITWGDB" , O_WRONLY);
//     // first time just sending the pid_pipe number to put in map
//     write(wfd , pid_pipe , strlen(pid_pipe));   
//     int rfd = open(pid_pipe , O_RDONLY);
    
//     // creating the pollfds 
//     struct pollfd pfd[2];
//     int users = 0;

//     // initialising the pollfds
//     pfd[0].fd = 0; pfd[0].events = POLLIN; 
//     pfd[1].fd = rfd; pfd[1].events = POLLIN;

//     while (1)
//     {
//         // getting the bool value
//         int ret = poll(pfd , 2 , 100);
//         if (ret>0)
//         {
//             char buffer[1024];
//             // if it is ready to take input, we take it
//             if (pfd[0].revents & POLLIN)
//             {   
//                 string strl;
//                 getline(cin,strl);
//                 cout<<"sending the message : "<<strl<<endl;
//                 string newtext = pid + ":" + strl;
//                 const char * newtextt = newtext.c_str();
//                 write(wfd , newtextt , strlen(newtextt));
//             }
//             // if it is ready to print input from famous fifo, itll print
//             if (pfd[1].revents & POLLIN)
//             {
//                 char readbuffer[1024];
//                 read(rfd , readbuffer , sizeof(readbuffer));
//                 cout<<readbuffer<<endl;
//             }
//         }
//     }
// }

// using select
// #include <iostream>
// #include <string>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <signal.h>
// #include <cstring>
// #include <cerrno>

// using namespace std;

// int main() {
//     // Getting pid
//     string pid = to_string(getpid());
//     cout << "Process with " << pid << " has joined the chat room..... " << endl;

//     // To make a new FIFO, its name should be the pid_pipe number
//     char makepipe[pid.length() + 1];
//     for (int i = 0; i < pid.length(); i++)
//         makepipe[i] = pid[i];
//     makepipe[pid.length()] = '\0';

//     // Making that FIFO
//     mkfifo(makepipe, 0666);

//     const char *pid_pipe = pid.c_str();
//     int wfd = open("NITWGDB", O_WRONLY);

//     // First time just sending the pid_pipe number to put in map
//     write(wfd, pid_pipe, strlen(pid_pipe));
//     int rfd = open(pid_pipe, O_RDONLY);


//     fd_set read_fds;
//     int max_fd = max(0, rfd) + 1;
//     char readbuffer[1024];
//     char inputbuffer[1024];

//     while (true) {
//         FD_ZERO(&read_fds);
//         FD_SET(0, &read_fds); // Add stdin to the set
//         FD_SET(rfd, &read_fds); // Add FIFO to the set

//         // Use select to wait for events
//         int ret = select(max_fd, &read_fds, NULL, NULL, NULL);
//         if (ret < 0) {
//             perror("select");
//             break;
//         }

//         if (FD_ISSET(0, &read_fds)) {
//             // Stdin has input
//             int bytesRead = read(0, inputbuffer, sizeof(inputbuffer));
//             if (bytesRead > 0) {
//                 inputbuffer[bytesRead] = '\0';
//                 string inputStr(inputbuffer);
//                 cout << "Sending the message: " << inputStr << endl;
//                 string newtext = pid + ":" + inputStr;
//                 const char *newtextt = newtext.c_str();
//                 write(wfd, newtextt, strlen(newtextt));
//             }
//         }

//         if (FD_ISSET(rfd, &read_fds)) {
//             // FIFO has input
//             int fifoBytesRead = read(rfd, readbuffer, sizeof(readbuffer));
//             if (fifoBytesRead > 0) {
//                 readbuffer[fifoBytesRead] = '\0';
//                 cout << readbuffer << endl;
//             }
//         }
//     }

//     // Cleanup and close file descriptors
//     close(wfd);
//     close(rfd);
//     remove(makepipe);

//     return 0;
// }



