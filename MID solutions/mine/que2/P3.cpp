#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
   while (1)
   {
      cout << "Hello from P3" << endl;
      fflush(stdout); // Flush the output to make it available for reading

      sleep(10);
   }
}
