#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    int itfd = open("IT.txt", O_RDONLY);
    int poutfd = open("Pout.txt", O_WRONLY);

    // one for getting file and one for sending file
    int f=mkfifo("NITWGDB", 0666);
    mkfifo("RETURN1", 0666);
    cout<<f<<endl;

    // Receiving the file data
    int rfd = open("NITWGDB", O_RDONLY);
    // storing P.cpp in T.cpp
    int tfd = open("T.cpp", O_WRONLY);
    char buff[20];
    ssize_t bytesRead;
    while ((bytesRead = read(rfd, buff, sizeof(buff) - 1)) > 0)
    {
        buff[bytesRead] = '\0';
        write(tfd, buff, bytesRead);
    }

    close(rfd);
    close(tfd);

    // Executing the file
    const char arr[18] = {'g', '+', '+', ' ', 'T', '.', 'c', 'p', 'p', ' ', '-', 'o', ' ', 'T'};
    system(arr);

    int c = fork();
    if (c > 0)
    {
        // Parent
        cout << "Inside parent now" << endl;
        cout << "Parent going into Wait state" << endl
             << endl;
        wait(NULL);
        cout << "Parent out of wait state" << endl;

        // After the file has been executed
        cout << "Program has been executed" << endl
             << endl;

        // Need to check if Pout and Out are same or not
        ifstream outfd("OT.txt");
        ifstream poutfd("Pout.txt");
        int flag = 0;
        string s1, s2;
        while (getline(outfd, s1) && getline(poutfd, s2))
        {
            if (s1 != s2)
            {
                // not matching
                flag = 1;
            }
        }

        int wfd = open("RETURN1", O_WRONLY);

        if (flag == 1)
        {
            // Wrong
            write(wfd, "Wrong", 5);
        }
        else
        {
            // Correct
            write(wfd, "Correct", 7);
        }
        close(wfd);
    }
    else
    {
        // Child process
        cout << "Inside child Process now" << endl<<endl;

        // changing cin, cout for read from file and write to file
        dup2(poutfd, 1);
        dup2(itfd, 0);

        // Calling exec to exec the temp file
        char *path[] = {NULL};
        execvp("./T", path);
    }

    close(itfd);
    close(poutfd);
}
