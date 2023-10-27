#include<stdio.h> 
 
int main() 
{ 
      int i, limit, current_time = 0, x, counter = 0, time_quantum; 
      int wait_time = 0, turnaround_time = 0, arrival_time[10], burst_time[10], temp[10]; 
      float average_wait_time, average_turnaround_time;
      printf("\nEnter total Number of Processes:\t"); 
      scanf("%d", &limit); 
      x = limit; 
      for(i = 0; i < limit; i++) 
      {
            printf("\nEnter Details of Process[%d]\n", i + 1);
            printf("Arrival Time:\t");
            scanf("%d", &arrival_time[i]);
            printf("Burst Time:\t");
            scanf("%d", &burst_time[i]); 
            temp[i] = burst_time[i];
      } 
      printf("\nEnter Time Quantum:\t"); 
      scanf("%d", &time_quantum); 
      printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");
      for(current_time = 0, i = 0; x != 0;) 
      { 
            if(temp[i] <= time_quantum && temp[i] > 0) 
            { 
                  current_time += temp[i]; 
                  temp[i] = 0; 
                  counter = 1; 
            } 
            else if(temp[i] > 0) 
            { 
                  temp[i] = temp[i] - time_quantum; 
                  current_time += time_quantum; 
            } 
            if(temp[i] == 0 && counter == 1) 
            { 
                  x--; 
                  printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d", i + 1, burst_time[i], current_time - arrival_time[i], current_time - arrival_time[i] - burst_time[i]);
                  wait_time += current_time - arrival_time[i] - burst_time[i]; 
                  turnaround_time += current_time - arrival_time[i]; 
                  counter = 0; 
            } 
            // if i reaches end, start again
            if(i == limit - 1) 
                  i = 0; 
            //if arrival time is less than current time then move forward
            else if(arrival_time[i + 1] <= current_time) 
                  i++;
            //or any other reason just start again from 0
            else 
                  i = 0;
      } 
      average_wait_time = wait_time * 1.0 / limit;
      average_turnaround_time = turnaround_time * 1.0 / limit;
      printf("\n\nAverage Waiting Time:\t%f", average_wait_time); 
      printf("\nAvg Turnaround Time:\t%f\n", average_turnaround_time); 
      return 0; 
}
