#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "barrier.h"

int nproc, shmid_sem, shmid_count;
int *count;
sem_t *sem;

void init_barrier(int numproc) {
    nproc = numproc;
    //shmid_count = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    //count = (int *) shmat(shmid_count, NULL, 0);
    //count[0] = 0; // initialize count to 0

    shmid_sem = shmget(IPC_PRIVATE, 2 * sizeof(sem_t) + sizeof(int), IPC_CREAT | 0600);
    sem = (sem_t *) shmat(shmid_sem, NULL, 0);
    count = (int *) &shmat(shmid_count, NULL, 0)[2];
    count[0] = 0; // initialize count to 0
    sem_init(&sem[0], 1, 1); // sem[0] = count_mutex
    sem_init(&sem[1], 1, 0); // sem[1] = barrier
}

void reach_barrier() {
    sem_wait(&sem[0]);
    count[0]++;
    sem_post(&sem[0]);

    if (count[0] == nproc) {
        // release one process
        sem_post(&sem[1]);
    } else {
        // we are not the last process, 
        // wait at the barrier semaphore until we are freed.
        sem_wait(&sem[1]);

        // now that we are freed.
        sem_post(&sem[1]);
    }
}
    

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(&sem[0]);
        sem_destroy(&sem[1]);
        shmctl(shmid_count, IPC_RMID, 0);
        shmctl(shmid_sem, IPC_RMID, 0);
    }
}


