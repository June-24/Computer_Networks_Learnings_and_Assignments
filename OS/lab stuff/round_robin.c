#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// 21CSB0B36
// JUNAID
#define NUM_PROCESSES 4
int TIME_QUANTUM;

struct Process
{
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completed;
    int completion_time;
};

struct Process processes[NUM_PROCESSES];

void *executeProcess(void *args)
{
    printf("Executing processes using Round Robin Scheduling...\n");
    int current_time = 0;
    int remaining_processes = NUM_PROCESSES;
    int arr[100] = {0};
    int cnt = 0;
    int back = 0;
    while (remaining_processes > 0)
    {
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (!processes[i].completed && processes[i].arrival_time <= current_time)
            {
                int time_slice = TIME_QUANTUM;
                if (processes[i].remaining_time < TIME_QUANTUM)
                {
                    time_slice = processes[i].remaining_time;
                }
                // Execute the process for the time slice
                processes[i].remaining_time -= time_slice;
                current_time += time_slice;

                printf("Executing Process %d, Remaining Time: %d, CPU time given: %d\n",
                       processes[i].id, processes[i].remaining_time, current_time);

                if (processes[i].remaining_time == 0)
                {
                    processes[i].completed = 1;
                    processes[i].completion_time = current_time;
                    remaining_processes--;

                    printf("Process %d completed, at time T = %d\n", processes[i].id, current_time);
                }
            }
        }
    }
    return NULL;
}
void printData()
{
    int avg_waiting_time = 0;
    int avg_turnaround_time = 0;

    printf("The final data for Round Robin is: \n");
    printf("id ArrivalTime BurstTime CompletionTime TurnAroundTime WaitingTime\n");

    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        printf("%d %d %d %d %d %d\n",
               processes[i].id, processes[i].arrival_time, processes[i].burst_time,
               processes[i].completion_time, processes[i].completion_time - processes[i].arrival_time,
               processes[i].completion_time - processes[i].arrival_time - processes[i].burst_time);

        avg_waiting_time += processes[i].completion_time - processes[i].arrival_time - processes[i].burst_time;
        avg_turnaround_time += processes[i].completion_time - processes[i].arrival_time;
    }

    printf("The average waiting time is: %d\n", avg_waiting_time / NUM_PROCESSES);
    printf("The average turnaround time is: %d\n", avg_turnaround_time / NUM_PROCESSES);
}
int main()
{
    printf("Enter the time quantum: ");
    scanf("%d", &TIME_QUANTUM);

    printf("Enter data for each process:\n\n");
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        processes[i].id = i + 1;
        printf("Enter the arrival time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter the burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completed = 0;
        processes[i].completion_time = 0;
    }
    pthread_t tid;
    pthread_create(&tid, NULL, executeProcess, NULL);

    pthread_join(tid, NULL);

    printData();

    return 0;
}

