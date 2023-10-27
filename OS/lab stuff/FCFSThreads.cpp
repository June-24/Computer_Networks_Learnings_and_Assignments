#include <iostream>
#include <queue>
#include <pthread.h>
using namespace std;
struct Process
{
    int id;
    int arrivalTime;
    int burstTime;
    bool operator<(const Process &other) const
    {
        return arrivalTime > other.arrivalTime;
    }
};
priority_queue<Process> processQueue;
void *FCFS(void *args)
{
    int currentTime = 0;

    cout << "FCFS Schedule:\n";

    while (!processQueue.empty())
    {
        Process currentProcess = processQueue.top();
        processQueue.pop();
        if (currentProcess.arrivalTime > currentTime)
        {
            cout << "CPU idle from " << currentTime << " to " << currentProcess.arrivalTime << endl;
            currentTime = currentProcess.arrivalTime;
        }
        cout << "Process " << currentProcess.id << " is running from time " << currentTime
             << " to " << currentTime + currentProcess.burstTime << endl;
        currentTime += currentProcess.burstTime;
    }
}
int main()
{
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int id, arrivalTime, burstTime;
        cout << "enter the id, arrival time, burstTime: ";
        cin >> id >> arrivalTime >> burstTime;
        processQueue.push({id, arrivalTime, burstTime});
    }
    pthread_t tid;
    pthread_create(&tid, NULL, FCFS, NULL);
    pthread_join(tid, NULL);
    return 0;
}
