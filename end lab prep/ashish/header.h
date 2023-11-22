#include <pcap.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/un.h>

#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#define PROMISCUOUS_MODE 1
#define NON_PROMISCUOUS_MODE 0

#include <cstring>
#include <memory.h>

// bits/stdc++.h headers
#include <iostream>
// #include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// #include <list>
#include <deque>
#include <queue>
// #include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <iterator>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>
#include <thread>
#include <mutex>
#include <semaphore.h>
