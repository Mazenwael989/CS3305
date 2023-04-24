//CS3350 Assignment 2
//Done By: Mazen Baioumy
//S/N:250924925

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


/*
Creating main function which will 
accept strings from user, and execute required rest of code.
*/
int main(int argc, char **argv)
{
    //initialize the three strings x, y, and z using arg
    char x[strlen(argv[1])+strlen(argv[3])+strlen(argv[2])+5];
    char y[strlen(argv[2])+strlen(argv[3])+4];
    char z[strlen(argv[3])+1];

    
    // creating array for mypipe
    int mypipe[2];	
    // creating my pipe to create communication between two processes
	pipe(mypipe);

    
    //Create error detection for pipe creation
    if (pipe(mypipe) < 0)
    
    {
        printf("A Pipe error occurred!\nExit\n");
        return -1;
    }

	printf("A pipe is created for communication between parent (PID %d) and child\n", getpid());
    
    //Use fork to create parent and child processes
    pid_t parent = fork();
        
    //Execute parent process
    if (parent > 0)
    {
       
        //Parent read x from the pipe
        read(mypipe[0], x, sizeof(x));
        printf("Parent (%d) reads x = \"%s\" from the pipe\n", getpid(), x);

        
        wait(NULL);

       
        //Parent receives in second argument (y) equal to  3305
        strcpy(y, argv[2]);
        printf("Parent (%d) receives y = \"%s\" from the terminal\n", getpid(), y);


        //Parent receieves both x from pipe after being written and concatenated it with y
        strcat(x," ");
        strcat(x,y);
        printf("Parent (%d) receives and concatenates x and y' = \"%s\"\n", getpid(), x);

        
        //Write concatenated x with other arguments from pipe
        write(mypipe[1],x, sizeof(x));
        printf("Parent (%d) writes x = \"%s\" to the pipe\n", getpid(), x);

        
        //Parent reads x from pipe after it has been written
        read(mypipe[0], x, sizeof(x));
        printf("Parent (%d) reads x = \"%s\" from the pipe\n", getpid(), x);



    }
    //Execute child process
    if (parent == 0) 
    {
       //Child has been created
       printf("parent (PID %d) created a child (PID %d)\n", getppid(), getpid());

        //child will take in first argument from terminal x = CS
        strcpy(x, argv[1]);
        printf("Child (%d) receives x = \"%s\" from the terminal\n", getpid(), x);

        //Child will write x = CS into mypipe
        write(mypipe[1], x, sizeof(x));
        printf("Child (%d) writes x = \"%s\" to the pipe\n", getpid(), x);

        
        //Read x from pipe after being written 
        read(mypipe[0],x, sizeof(x));
        printf("Child (%d) reads x = \"%s\" to the pipe\n", getpid(), x);

        
        //Child receives z = is fun from terminal
        strcpy(z, argv[3]);
        printf("Child (%d) recieves z = \"%s\" from the terminal\n", getpid(), z);

        
        //Child concatenates both x and z
        strcat(x," ");
        strcat(x,z);
        printf("Child (%d) receives and concatenates x and z' = \"%s\"\n", getpid(), x);

        //Write x into the pipe to be read by parent
        write(mypipe[1], x, sizeof(x));
        printf("Child (%d) writes x = \"%s\" to the pipe\n", getpid(), x);


    
    }

    else if (parent < 0)
    {
        printf("Your program could not create fork please exit");
    }
    
    

    
    



}
