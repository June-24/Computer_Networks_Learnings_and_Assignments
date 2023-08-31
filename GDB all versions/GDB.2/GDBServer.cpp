#include <bits/stdc++.h>
#include <sys/poll.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include<sys/wait.h>
using namespace std;

int main()
{
    // opening fifos and making them ready
    mkfifo("IN1", 0666);int ffd1 = open("IN1", O_RDONLY | O_NONBLOCK);
    mkfifo("IN2", 0666);int ffd2 = open("IN2", O_RDONLY | O_NONBLOCK);
    mkfifo("IN3", 0666);int ffd3 = open("IN3", O_RDONLY | O_NONBLOCK);

    // making fifos for returning answers
    mkfifo("OUT1", 0666);
    mkfifo("OUT2", 0666);
    mkfifo("OUT3", 0666);

    // creating poll for each
    struct pollfd pfds[3];
    pfds[0].fd = ffd1;pfds[0].events = POLLIN;
    pfds[1].fd = ffd2;pfds[1].events = POLLIN;
    pfds[2].fd = ffd3;pfds[2].events = POLLIN;
    
    while (1)
    {
        int ret = poll(pfds, 3, 10);
        if (ret > 0)
        {
            for (int i = 0; i < 3; ++i)
            {
                
                if (pfds[i].revents & POLLIN)
                {
                    int tfd = open("T.cpp", O_WRONLY);
                    cout<<"Going to execute program "<<i+1<<endl;

                    // clearing T.cpp if anything there
                    ofstream file1("T.cpp", ofstream::out | ofstream::trunc);
                    file1.close();
                    
                    // writing to T.cpp
                    char buff[20];
                    int bytesRead;
                    int flag=0;
                    while ((bytesRead = read(pfds[i].fd, buff, sizeof(buff) - 1)) > 0)
                    {
                        flag=1;
                        buff[bytesRead] = '\0';
                        write(tfd, buff, bytesRead);
                    }

                    // compile T.cpp
                    system("g++ T.cpp -o T");

                    int c=fork();
                    if(c>0){
                        // parent
                        wait(NULL);
                        // clearing T.cpp
                        ofstream file("T.cpp", ofstream::out | ofstream::trunc);
                        file.close();

                        // verifying
                        string t="out"+to_string(i+1)+".txt";
                        ifstream outfd(t);
                        ifstream poutfd("Pout.txt");
                        int flag1 = 0;
                        string s1, s2;
                        while (getline(outfd, s1) && getline(poutfd, s2))
                        {
                            if (s1 != s2)
                            {
                                // not matching
                                flag1 = 1;
                            }
                        }
                        char t2[]={'.','/', 'O', 'U', 'T', char('1'+ i ), '\0'};
                        int ofd=open(t2,O_WRONLY);
                        if (flag1 == 1)
                        {
                            // Wrong
                            write(ofd, "Not Passed", 11);
                            // cout<<"Test case for program "<<i+1<<" failed"<<endl;
                        }
                        else
                        {
                            // Correct
                            write(ofd, "Been Passed", 12);
                            // cout<<"Test case for program "<<i+1<<" passed"<<endl;
                        }
                        cout<<"Executed program "<<i+1<<endl<<endl;

                    }
                    else{
                        // child
                        char if1[]={'.','/', 'i', 'n', char('1'+ i ), '.', 't', 'x', 't', '\0'};
                        int itfd = open(if1, O_RDONLY);
                        int poutfd = open("Pout.txt", O_WRONLY);
                        // changing cin, cout for read from file and write to file
                        dup2(poutfd, 1);
                        dup2(itfd, 0);

                        // Calling exec to exec the temp file
                        char *path[] = {NULL};
                        execvp("./T", path);
                    }
                    fcntl(pfds[i].fd, F_SETFL, fcntl(pfds[i].fd, F_GETFL) & ~O_NONBLOCK);

                    
                }
            }
        }
    }

    return 0;
}
