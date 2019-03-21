#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

struct sharedArray{
	char * temp[100];
};

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
	char * temp[100] = shmat(shmID, 
	for(i = 0; i < 5; i++){
		printf("In child %d. Shared memory:%s\n", pid, shmPTR->temp[i]);		
	}
	printf("Process %d exiting.\n", getpid());
	shmdt(shmPTR);
	return 0;
}
