#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5

int main() {

    int i, j, k, pid, shmid;

    sem_t *sem;

    shmid = shmget(IPC_PRIVATE, NUM_PROCESSES * sizeof(sem_t), IPC_CREAT | 0600);
    sem = (sem_t *) shmat(shmid, NULL, 0);

    for (k = 0; k < NUM_PROCESSES; k++) {
      sem_init(&sem[k], 1, 0);
    }

    sem_post(&sem[0]);

    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {
            break;
        }
    }

    if(pid == 0) {
        sem_wait(&sem[i]);
        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");
        //sem_wait(sem[i]);
        if (i < NUM_PROCESSES - 1) {
            sem_post(&sem[i + 1]);
        }
    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);
        
        for(i=0; i<NUM_PROCESSES; i++) 
            sem_destroy(&sem[i]);

        shmctl(shmid, IPC_RMID, 0);
    }

}

