//Done By Mazen Baioumy
//Student Number: 250924925
//CS3305 Assignment-1


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
    //Using fork to create parent process
    pid_t parent =  fork();

    //If statement incase forking is unsuccessful to exit code
    if (parent < 0)
    {
        printf ("Fork not Successful");
        exit(1);
    }
    //else if statement for creating new child
    else if (parent == 0)
    {
        pid_t child1 = fork();
        //We will wait until child 1 is complete before moving on to creating child 2

        if (child1 > 0)
        {
            wait(NULL);
        }
        //if child creation is successful, we perform tasks of child 1
        else if (child1 == 0)
        {
            printf("parent process (PID %d) created child1 successfully (PID %d)\n", getppid(), getpid());
			printf("parent process (PID %d) is waiting for child1 (PID %d) to complete\n",getppid(), getpid());

            pid_t child11 = fork();
            //Exit if forking is unsuccessful
            if (child11 < 0)
            {
                printf("Forking Not Successful");
                exit(1);
            }

            else if (child11 == 0)
            { 
            //Please change in excel function the file path of your external program.
            int pid = getpid();
            char * mypid[6];
            sprintf(mypid, "%d", pid);
            printf("parent (PID %d) created child11 (PID %d)\n", getppid(), getpid());
			printf("child11 (PID %d) is calling an external program [external_program.out]\n", getpid());
            execl("/Users/mazenwael/Downloads/3305/Assignment_1/external_program-1", mypid, NULL);
            //Please change in excel function the file path of your external program. 
            exit(1);
			}
            // condition for the child11 to perform the required tasks
            else if (child11 > 0){
				wait(NULL);
				printf("child1 (PID %d) is now completed\n", getpid());
				exit(1);
			}
        }
        //Creating of child 2 after child 1 and 1.1 are created and terminated.
        pid_t child2 = fork();

        if (child2 < 0)
        {
            printf("Forking not Successful");
            exit(1);
        }
        //if we are in the child_2 then we need to execute the external program "exterinal program.o"

        else if (child2 == 0)
        {
            //Please change in excel function the file path of your external program. 
            int pid = getpid();
            char * mypid[6];
            sprintf(mypid, "%d", pid);
            printf("parent (PID %d) created child2 (PID %d)\n", getppid(), getpid());
			printf("child2 (PID %d) is calling an external program [external_program.out]\n", getpid());
            execl("/Users/mazenwael/Downloads/3305/Assignment_1/external_program-1", mypid, NULL);
            //Change file path in this fucntion to your external_program file path.
	    }
		else if (child2 > 0){
			wait(NULL);
		}
    }
}