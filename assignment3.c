/* CS3305 Assignment 3
 * Done By: Mazen Baioumy
 * Student Number: 250924925
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <math.h>


//TO Run CODE (gcc assignment3.c -o assignment3)            
//Then Enter  (./assignment3 "33" "4")


// Creating array to be used by sprintf for printing output
char outputs[256];

// Creating an array for pipe function to create pipe
int mypipe[2];

// Creating value for holding pipe size and number of values
int pipeS;

// Initialize S which will be the subtraction of both integers inputted
int S;

// This function is executed by thread 1 for  subtraction
void *subtract(void *thread_id) {
    int *id = (int*)thread_id;

    // Array for use while reading from pipe
    char ar[pipeS];

    // Reads from mypipe the information
    read(mypipe[0], ar, pipeS);

    // Get X and Y from the array by splitting around ","
    char * xToken = strtok(ar, ",");
    char * yToken = strtok(NULL, "");

    // Convert inputs from strings to integers
    int X = atoi(xToken) , Y = atoi(yToken);

    // Print the statement and the values
    sprintf(outputs, "Thread (TID %d) reads X = %d and Y = %d from the pipe", *id, X, Y);
    puts(outputs);

    // Subtracts X and Y
    S = X - Y;
    sprintf(ar, "%d", S);
    pipeS = strlen(ar);
    // Write to pipe and print statement
    write(mypipe[1], ar, pipeS);
    sprintf(outputs, "Thread (TID %d) writes X-Y = %d to the pipe", *id, S);
    puts(outputs);

    return 0;
}

// This function is executed by thread 1
void *prime_check(void *thread_id) {
    int *id = (int*)thread_id;

//Array for use while reading from pipe
    char ar[pipeS];

//Thread reads from pipe
    read(mypipe[0], ar, pipeS);

//Convert S to int
    S = atoi(ar);

//Print the statement and the values
    sprintf(outputs, "Thread (TID %d) reads X-Y = %d from the pipe", *id, S);
    puts(outputs);

//Initialize (i) and flag for true/false
    int i, flag = 1;
//Iterate from 2 to sqrt(S)
    for (i=2; i <= sqrt(S); i++) {
//If S is divisble by any number between 2 it is not prime, Use remainder with i
        if (S % i == 0)
        {
            flag = 0;
            break;
        }
        
    }
//Create if statments to print depending on input wether prime or not
    if (S <= 1)
        flag = 0;
//Create if statement if flag is true then print prime number
    if (flag == 1){
        sprintf(outputs, "Thread (TID %d) identifies X-Y = %d as prime number", *id, S);
        puts(outputs);
    }
//Create if statement in case value is not prime number
    else {
        sprintf(outputs, "Thread (TID %d) identifies X-Y = %d as a not prime number", *id, S);
        puts(outputs);
    }


    sprintf(ar, "%d", S);
//Write to pipe and print statement
    write(mypipe[1], ar, pipeS);
    sprintf(outputs, "Thread (TID %d) writes X-Y = %d to the pipe", *id, S);
    puts(outputs);

    return 0;
}

// This function is executed by thread 2
void *reverse_num(void *thread_id) {
    int *id = (int*)thread_id;

    char ar[pipeS];

// Thread reads from pipe
    read(mypipe[0], ar, pipeS);

// Convert S to int
    S = atoi(ar);
    sprintf(outputs, "Thread (TID %d) reads X-Y = %d from the pipe", *id, S);
    puts(outputs);

//Initialize reverse, remainder. Reverse will hold reversed number
    int reverse = 0, remainder;
//Create while loop to process formula for 
    while (S)
    {
        //Divide number by 10 then store remainder
        remainder = S % 10;
        //Multipy by 10 then add remainder then store  in reverse
        reverse = reverse * 10 + remainder;
        //Divide by 10 and update value to eliminate the last digit that is already reversed
        S = S/10;
    }
    
// Print the statement and the values reversed
    sprintf(outputs, "Thread (TID %d) identifies the reverse of the number is: %d", *id, reverse);
    puts(outputs);

    return 0;
}



int main(int argc, char *argv[2]) {
//Create Parent Pid and click parent pid 
    int parent_pid = getpid();

// Creating an error statement incase piping fails
    if (pipe(mypipe) < 0) {
        perror("Pipe Error");
        exit(0);
    }

// Accepts two integers from User in command line using Argv
    char *xArg = argv[1];
    int X = atoi(xArg);
    char *yArg = argv[2];
    int Y = atoi(yArg);

//Create an array for pipe
    char ar[strlen(argv[1]) + strlen(argv[1]) + 2];

//Print recieved values
    sprintf(outputs, "Parent (PID %d) receives X = %d and Y = %d from the user", parent_pid, X, Y);
    puts(outputs);

//Parent writes X and Y to the pipe
    sprintf(ar, "%d,%d", X, Y);
    pipeS = strlen(ar);
    write(mypipe[1], ar, pipeS);
    sprintf(outputs, "Parent (PID %d) writes X = %d and Y = %d to the pipe", parent_pid, X, Y);
    puts(outputs);

    
    
    
// Creating Thread IDS from (1-3)
    pthread_t thread_1, thread_2, thread_3;
    int thread_id_1 = 1, thread_id_2 = 2, thread_id_3 = 3;

     
// Creation of Thread 1 responsible for subtraction of two numbers
    if (pthread_create(&thread_1, NULL, subtract, &thread_id_1)) {
        sprintf(outputs, "An error has occurred while creating thread 1\n");
        puts(outputs);
        exit(1);
    }
    pthread_join(thread_1, NULL);

    
        
// Creation of Thread 2 responsible for identifying prime
    if (pthread_create(&thread_2, NULL, prime_check, &thread_id_2)) {
        sprintf(outputs, "An error has occurred while creating thread 2\n");
        puts(outputs);
        exit(1);
    }
    pthread_join(thread_2, NULL);

    
    
// Creation of Thread 3 responsible for reversing number
    if (pthread_create(&thread_3, NULL, reverse_num, &thread_id_3)) {
        sprintf(outputs, "An error has occurred while creating thread 3\n");
        puts(outputs);
        exit(1);
    }
    pthread_join(thread_3, NULL);
}