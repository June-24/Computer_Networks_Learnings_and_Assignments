#include <iostream>
#include <signal.h>

int main() {


    // Send a signal to all receiver processes
    killpg(5043, SIGUSR1); // (6969 is the process group id)

    std::cout << "Signal sent to all receivers" << std::endl;

    return 0;
}
