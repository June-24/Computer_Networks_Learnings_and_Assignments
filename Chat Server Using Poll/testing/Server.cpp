#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

int main ()
{
    mkfifo("chattalks", 0666);
    map<string, int> mp;

    int rdf = open("chattalks", O_RDONLY);

    while (1)
    {
        char buffer[1024] = {'\0'};

        read(rdf, buffer, sizeof(buffer));

        bool joinnow = true;
        string pid;

        // Extract Client ID from message
        int i = 0;
        while (buffer[i] != ' ' && buffer[i] != '\0')
        {
            pid += buffer[i];
            i++;
        }

        cout << "The message received is " << buffer << endl;

        if (joinnow && !mp.count(pid))
        {
            cout << "New client : " << pid << " joined the chat .." << endl;

            const char *arr = pid.c_str();
            int tfd = open(arr, O_WRONLY);
            mp[pid] = tfd;
        }
        else
        {
            for (auto i : mp)
            {
                if (i.first != pid)
                {
                    write(mp[i.first], buffer + pid.length() + 1, strlen(buffer + pid.length() + 1));
                }
            }
        }
    }
}
