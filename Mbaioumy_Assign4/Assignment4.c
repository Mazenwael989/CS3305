/* CS3305 Assignment 4
 * Done By: Mazen Baioumy
 */



#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static const int CHUNK_SIZE = 500;
int main() {
// Open the file with if statemenet in case file does not open exit program
  FILE *file_pointer;
  file_pointer = fopen("rr_input.txt", "r");

  if (file_pointer == NULL) {
    printf("file not found!");
    return 1;
  }
 // initialzation for the required variables
  int file_length;
  int result;
  size_t length;
  char *string;
  int test = 1;
//
  while (!feof(file_pointer)) {
    float avg_turnaround = 0.0;
    float avg_waiting = 0.0;
    // get every line in the file until there is no more, perform operation for each line
    if (getline(&string, &length, file_pointer) != -1) {
      int max_process = 0;
      int quantum_time = 0;
      int process_id = 0;
      int arrive_time = 0;
      int burst_time = 0;

      int *process_at = (int *)malloc(sizeof(int) * length);
      int *process_brt = (int *)malloc(sizeof(int) * length);
      printf("Test case #%d:%s", test, string);
      test++;
      char *position = string;
      //Get process Id and add arrival time and burst time, and quantam time
      while (*position != '\n') {

        if (*position == 'p') {

          position++;
          process_id = strtol(position, &position, 10);

          arrive_time = strtol(position, &position, 10);

          burst_time = strtol(position, &position, 10);

          process_at[process_id] = arrive_time;
          process_brt[process_id] = burst_time;
        //Highest process id
          if (process_id > max_process) {
            max_process = process_id;
          }

          position++;
        } else if (*position != 'p') {
          quantum_time = strtol(position, &position, 10);

          printf("Number of Processes: %d, Quantum: %d \n", max_process,
                 quantum_time);
          break;
        }
      }
      //Using Arrays for Round robin algorithm
      int *RR_q = (int *)malloc(sizeof(int) * max_process + 1);
      int *RR_b = (int *)malloc(sizeof(int) * max_process + 1);
      int *RR_e = (int *)malloc(sizeof(int) * max_process + 1);
      int *RR_d = (int *)malloc(sizeof(int) * max_process + 1);
      int *RR_o = (int *)malloc(sizeof(int) * max_process);
      int start_o = 0;
      int end_o = 0;
      int current_o = 0;
      // RR_q -> is process in queue
      // RR_b -> burst time remaining
      // RR_e -> when it entered
      // RR_d -> remaining bursts
      // int for process times, and current processes
      printf("Process Scheduling Started:\n");
      //Initialization of variables for CPU Time
      int completed_process = 0;
      int cpu_time = 0;
      int cpu_mod = 0;
      int current_process = 0;
      int first_time = 0;
      int existing_processes = 0;
      while (completed_process < max_process) {
        printf("CPU Time %d: ", cpu_time);
        //Checks all processes and when CPU time is equal to arrival time adds to round robin, however it isn't in qeue yet
        for (int i = 1; i <= max_process; i++) {
          if (process_at[i] == cpu_time) {
            printf("[p%d Arrived] ", i);
            RR_q[i] = 1;
            RR_b[i] = process_brt[i];
            RR_e[i] = cpu_time;
            existing_processes++;
            if (first_time == 0) {
              current_process = i;
              first_time = 1;
            }
          }
        }
        //if statement for second line to print none for no process in algorithm, until processes run
        if (existing_processes == 0 && first_time == 0) {
          cpu_time++;
          printf("None\n");
          continue;
        }
        //Calculations for the turnaround time and waiting time by going through different processes
        RR_d[current_process] = RR_d[current_process] + 1;
        printf("p%d [%d/%d]\n", current_process, RR_d[current_process],
               RR_b[current_process]);
        
        //Calculations for the turnaround time and waiting time using round robin
        if (RR_d[current_process] == RR_b[current_process]) {
          int turnaround = 1 + cpu_time - RR_e[current_process];
          int waiting = 1 + cpu_time - RR_e[current_process] - RR_b[current_process];
          printf("Process p%d completed with Turn Around Time: %d, Waiting " "Time: %d\n", current_process, turnaround, waiting);
          avg_turnaround = avg_turnaround + 1 + cpu_time - RR_e[current_process];
          avg_waiting = avg_waiting + 1 + cpu_time - RR_e[current_process] - RR_b[current_process];
          completed_process++;
          RR_o[current_o] = 0;
          existing_processes--;
          int count = 0;
          for (int i = 0; i < max_process; i++) {

            if (current_process == RR_o[(i + current_o) % max_process] ||
                RR_o[(i + current_o) % max_process] == 0) {
              continue;
            }
            current_process = RR_o[(i + current_o) % max_process];
            current_o = (i + current_o) % max_process;
            cpu_mod = 0;

            break;
          }
          //Does what previous For loop achieved by adding to Round Robin but not active in qeue yet
          for (int i = 1; i <= max_process; i++) {
            if (process_at[i] == cpu_time) {

              RR_q[i] = 1;
              RR_b[i] = process_brt[i];
              RR_e[i] = cpu_time;
              RR_o[end_o] = i;
              end_o++;
            }
          }
          cpu_time++;
          continue;
        }
      //if we ran all process in qeue then exit
        if (completed_process == max_process) {
          break;
        }
        cpu_time++;
        cpu_mod++;

        //When reached the quantam time, then reset and find next process in qeue
        //Reset to run process equal to burst time
        if (cpu_mod == quantum_time) {
          int count = 0;
          if (existing_processes > 1) {
            for (int i = 0; i <= max_process; i++) {

              if (current_process == RR_o[(i + current_o) % max_process] ||
                  RR_o[(i + current_o) % max_process] == 0) {

                continue;
              }
              current_process = RR_o[(i + current_o) % max_process];
              current_o = (i + current_o) % max_process;
              break;
            }
          }

          cpu_mod = 0;
        }
      //Now add to round robin
        for (int i = 1; i <= max_process; i++) {
          if (process_at[i] == cpu_time - 1) {
            RR_o[end_o] = i;
            end_o++;
          }
        }
      }
      //Print statement for each process
      printf("Process scheduling completed with Avg Turn Around Time: %.2f, "
             "Avg Waiting Time:%.2f\n\n",
             avg_turnaround / max_process, avg_waiting / max_process);
    }
  }
}