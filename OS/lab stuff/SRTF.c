#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include<math.h>
#define NUM_PROCESSES 5
// Structure to represent a process
struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completed;
    int completion_time;
    int firstTimeCPU;
};

struct Process processes[5];
void* executeProcess(void* args) {
    int currenttime=0;
    while (1) {
        int min_remaining_time = -1;
        int next_process_index = -1;
        

        // Finding the shortest remaining time in the available queue
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!processes[i].completed && processes[i].arrival_time <= currenttime) {
                if (min_remaining_time == -1 || processes[i].remaining_time < min_remaining_time) {
                    min_remaining_time = processes[i].remaining_time;
                    next_process_index = i;
                }
            }
        }

        if (next_process_index != -1) {
        //using 1 unit time...
            if(processes[next_process_index].firstTimeCPU>currenttime){
            	processes[next_process_index].firstTimeCPU=currenttime;
            }
            processes[next_process_index].remaining_time--;
            currenttime++;

            printf("Executing Process %d, Remaining Time: %d->%d, CPU time given: %d->%d\n", processes[next_process_index].id,processes[next_process_index].remaining_time+1, processes[next_process_index].remaining_time,currenttime-1,currenttime);

            if (processes[next_process_index].remaining_time == 0) {
                processes[next_process_index].completed = 1;
                processes[next_process_index].completion_time=currenttime;
                printf("Process %d completed, at time T = %d\n", processes[next_process_index].id,currenttime);
            }
        } else {
            printf("CPU idle\n");
            currenttime++;
            sleep(1); 
        }
        int all_completed = 1;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!processes[i].completed) {
                all_completed = 0;
                break;
            }
        }
        if (all_completed) {
            break;
        }
    }
}
void* printdata(void* args){
	int avg_waiting_time=0;
	int avg_turn_around_time=0;
	int avg_Response_time=0;
	printf("the final data for the SRTF is: \n");
	printf("id   ArrivalTime     BurstTime     CompletionTime     TurnAroundTime   WaitingTime     ResponseTime\n");
	for(int i=0;i<NUM_PROCESSES;i++){
		printf("%d           ",processes[i].id);
		printf("%d               ",processes[i].arrival_time);
		printf("%d               ",processes[i].burst_time);
		printf("%d              ",processes[i].completion_time);
		printf("%d                    ",processes[i].completion_time-processes[i].arrival_time);
		printf("%d                 ",processes[i].completion_time-processes[i].arrival_time-processes[i].burst_time);
		printf("%d              \n",processes[i].firstTimeCPU-processes[i].arrival_time);
		avg_waiting_time+=processes[i].completion_time-processes[i].arrival_time-processes[i].burst_time;
		avg_turn_around_time+=processes[i].completion_time-processes[i].arrival_time;
		avg_Response_time+=processes[i].firstTimeCPU-processes[i].arrival_time;
	}
	printf("The average waiting time is: %d\n",avg_waiting_time/NUM_PROCESSES);
	printf("The average turnaound time is: %d\n",avg_turn_around_time/NUM_PROCESSES);
	printf("The average response time is: %d\n\n",avg_Response_time/NUM_PROCESSES);
	

}
int main() {
    // Initialize processes with arrival and burst times
    printf("Enter data for each process: \n\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        processes[i].id = i + 1;
        printf("Enter the arrival time for the process %d: ",i+1);
        scanf("%d",&processes[i].arrival_time);
        printf("Enter the burst time for the process %d: ",i+1);
        scanf("%d",&processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completed = 0;
        processes[i].completion_time=0;
        processes[i].firstTimeCPU=9999;
    }
    
    printf("\nThe data entered is: \n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
    	printf("id: %d, arrivaltime: %d,bursttime: %d \n",processes[i].id,processes[i].arrival_time,processes[i].burst_time);
    }
    
    pthread_t tid;
    pthread_create(&tid,0,executeProcess,(void*)0);
    pthread_join(tid,NULL);
    pthread_t tid1;
    pthread_create(&tid1,0,printdata,(void*)0);
    pthread_join(tid1,NULL);
    return 0;
}
