#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5

int main() {

    int i, j, pid;

    // create a new shared variable for turn
    int *turn;
    int shmid;

    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    // attach the shared memory region - turn to this process
    turn = shmat(shmid, NULL, 0);

    turn[0] = 0; // intialize turn to 0
    
    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {
            break;
        }
    }

    // busy waiting for my turn
    while (turn[0] != i);

    // it is currently my turn, execute my code
    if(pid == 0) {
        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");
        turn[0]++; // pass turn to next process
    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);

        // detach and destroy shared memory
        shmdt(turn);
        shmctl(shmid, IPC_RMID, 0);
    }
}

