#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<semaphore.h>
#include "sharedArray.h"


int main(int argc, char * argv[]){
	int i;
	char * ptr;
	pid_t pid = getpid();
	struct sharedArray * holder = (struct sharedArray*)malloc(sizeof(struct sharedArray));
	unsigned long key = strtoul(argv[0], &ptr, 10);
	int listStart = (int) strtol(argv[2], &ptr, 10);
	int shmID = (int) strtol(argv[1], &ptr, 10);
	int childNum = (int) strtol(argv[3], &ptr, 10);
	shmID = shmget(key, sizeof(struct sharedArray*), O_RDONLY);
	struct sharedArray * shmPTR = (struct sharedArray *) shmat(shmID, NULL, 0);
	sem_t *sem;
	sem = sem_open("pSem", O_EXCL);
	sem_wait(sem);
	for(i = 0; i < 100; i++){
		shmID = shmget(key, sizeof(char *), O_RDONLY);
		shmPTR->temp[i] = (char *) shmat(shmID, NULL, 0);
	}
	printf("Child %d Shared memory ID:%d.\n", pid, shmID);
	printf("Child %d Shared memory key:%li\n", pid, key);
	for(i = 0; i < 5; i++){
		printf("In child %d. Shared memory:%s\n", pid, shmPTR->temp[i]);		
	}
	sem_post(sem);
	sem_close(sem);
	printf("Process %d exiting.\n", getpid());
	shmdt(shmPTR);
	sem_unlink("pSem");
	return 0;
}
