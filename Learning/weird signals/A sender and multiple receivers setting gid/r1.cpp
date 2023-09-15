#include <unistd.h>
#include <stdio.h>
#include <signal.h>
void handler(int sig) {
  printf("Signal received in receiver 1\n");
}
int main() {
  // Get the pgrp of the current process.
  int pgrp = getpgrp();
  printf("The pgrp of the r1 is %d\n", pgrp);
  signal(SIGUSR1, handler);
  pause();
  pgrp = getpgrp();
  printf("The pgrp of the r1 is now %d\n", pgrp);

  return 0;
}
