#include <bits/stdc++.h>
#include <unistd.h>
#include<signal.h>

#include <sys/wait.h>
using namespace std;

int main()
{
    int p1 = fileno(popen("./p1", "w"));
    int p2 = fileno(popen("./p2", "r"));
    int p3 = fileno(popen("./p3", "r"));
    int c1 = fork();
    int c2 = fork();

    cout<<"hello"<<endl;
    if (c1 > 0 && c2 > 0)
    {
        // main parent
    }
    else if (c1 == 0 && c2 > 0)
    {
        char buffer[1000]="ThiS IS P1";
        write(p1, buffer, 1000);
    }
    else if (c1 > 0 && c2 == 0)
    {
        char buffer[1000];
        read(p2, buffer, 1000);
         cout<<"P2 "<<buffer<<endl;
    }
    else
    {
        char buffer[1000];
        read(p3, buffer, 1000);
         cout<<"P3 "<<buffer<<endl;
    }
}