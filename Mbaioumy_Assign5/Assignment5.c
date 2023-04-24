// Done By Mazen Baioumy (250924925)
// CS3305 Assignment 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

// Create balance/account/client variables to use for calculations
int numberaccounts = 0;
int accbalance[10];
int numberclients = 0;
char *c_operation[10];

// creating the lock variable to be used by the threads
pthread_mutex_t lock;

//Start the threading process to read characters from file
void *task_thread(void *arg) {
  char *arg_line = (char *)arg;
  char *sub_string;
  char *temp_array[50];
  int sub_string_count = 0;
  sub_string = strtok(arg_line, " ");
//Process through other tokens and variables
  while (sub_string != NULL) {
    temp_array[sub_string_count] = sub_string;
    sub_string = strtok(NULL, " ");
    sub_string_count = sub_string_count + 1;
  }
// total actions 
  int total = (sub_string_count - 1) / 3;
  for (int action_count = 0; action_count< total; action_count++) {
    char *typeoftransaction = temp_array[(action_count* 3) + 1];
    int transaction = atoi(temp_array[(action_count* 3) + 3]);   
    char *picktheaccount = temp_array[(action_count* 3) + 2];
    int accountnumber = (int)(picktheaccount[7] - '0');
    // lock before depositing/withdraw
    pthread_mutex_lock(&lock);
    //If the type of transaction is deposit process and calculate it
    if (strcmp(typeoftransaction, "deposit") == 0) {
      accbalance[accountnumber-1] = accbalance[accountnumber - 1] + transaction;
    }
    //else if statement to read withdraw transaction type and process it
    else if (strcmp(typeoftransaction, "withdraw") == 0) {
      if (accbalance[accountnumber-1] >= transaction) {
        accbalance[accountnumber-1] = accbalance[accountnumber-1] - transaction;
      }   
    } 
    // unlock after deposit/withdraw
    pthread_mutex_unlock(&lock); 
  }
  return 0;
}
// main function for code
int main(void) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t result;
  //Open the assignment 5 file containing the data
  fp = fopen("assignment_5_input.txt", "r");
  // this while loop is to go through the file and store the important information
  // where needed to be used later on by the threads
  while ((getline(&line, &len, fp)) != -1) {
    int sub_string_count= 0;
    char *sub_string;
    char copy[100];
    strcpy(copy, line);
    char *temp_array[50];
    sub_string = strtok(line, " ");  
  //Create while loop to process through tokens and data
    while (sub_string != NULL) {
      temp_array[sub_string_count] = sub_string;
      sub_string = strtok(NULL, " ");
      sub_string_count = sub_string_count + 1;
    }
    if (sub_string_count > 3) {
      c_operation[numberclients] = malloc(sizeof(copy));
      strcpy(c_operation[numberclients], copy);
      numberclients++;
    }
      //&& (strcmp(temp_array[1], "balance") == 0)
    else if (sub_string_count == 3) {
      accbalance[numberaccounts] = atoi(temp_array[2]);
      numberaccounts++;
    }
  }
  // declearing other variables required for threading
  pthread_t threads[numberclients];

  // for loop for creating threads
  for (int i = 0; i < numberclients; i++) {
    pthread_create(&threads[i], NULL, &task_thread, c_operation[i]);
  }
  for (int i = 0; i < numberclients; i++)
    pthread_join(threads[i], NULL);

  //Print statement for providing number of accounts and clients
  printf("No. of Accounts: %d\nNo. of Clients: %d\n", numberaccounts,numberclients);

  //Statement for printing account and balance and number of each
  for (int i = 0; i < numberaccounts; i++) {
    printf("account%i balance %i\n", i + 1, accbalance[i]);
  }
  return 0;
}