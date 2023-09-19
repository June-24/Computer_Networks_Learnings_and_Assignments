#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Function to handle the signal
void signal_handler(int signum) {
    printf("Received signal %d\n", signum);
    // You can perform custom actions here when the signal is received
}

int main() {
    // Create a sigaction structure
    struct sigaction sa;


    // Initialize the sa struct
    sa.sa_handler = signal_handler; // Function to handle the signal
    sa.sa_flags = 0; // No special flags

    // Register the signal handler using sigaction
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Waiting for SIGINT (Ctrl+C)...\n");

    // Keep the program running until a signal is received
    while (1) {
        // Your main program logic here
    }

    return 0;
}

