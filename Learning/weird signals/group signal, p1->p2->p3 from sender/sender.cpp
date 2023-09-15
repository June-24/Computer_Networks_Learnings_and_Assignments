// #include<bits/stdc++.h>
// #include<unistd.h>
// #include<signal.h>
// using namespace std;

// int main(){
//     int gid;
//     cout<<"Enter the GID number: ";
//     cin>>gid;
//     killpg(gid, SIGUSR1);
// }

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t p1_pid = ...; // Replace with the PID of p1
    pid_t p2_pid = ...; // Replace with the PID of p2
    pid_t new_pgrp = 6969; // Replace with the desired new PGRP

    if (setpgid(p1_pid, new_pgrp) == -1) {
        perror("setpgid for p1");
        return 1;
    }

    if (setpgid(p2_pid, new_pgrp) == -1) {
        perror("setpgid for p2");
        return 1;
    }

    // Now both p1 and p2 belong to the same new PGRP (new_pgrp)

    // Optionally, you can wait for both processes to finish
    // This is just for demonstration; you may have a different use case
    int status;
    waitpid(p1_pid, &status, 0);
    waitpid(p2_pid, &status, 0);

    return 0;
}
