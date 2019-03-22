#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<semaphore.h>
#include<string.h>
#include<ctype.h>


int main(int argc, char * argv[]){
	int i, j = 0, k, nonAlpha, noPal = 0, pal = 0;
	char * ptr;
	char palHolder[80][5], noPalHolder[80][5];
	char temp, temp1;
	pid_t pid = getpid();
	unsigned long key = strtoul(argv[0], &ptr, 10);
	int listStart = (int) strtol(argv[1], &ptr, 10);
	int childNum = (int) strtol(argv[2], &ptr, 10);
	int shmID = shmget(key, sizeof(char[80][100]), O_RDONLY);
	//struct sharedArray *shmPTR = (struct sharedArray*) shmat(shmID, NULL, 0);
	char (*shmPTR)[100] = shmat(shmID, NULL, 0);
	for(i = listStart; i < (listStart + 5); i++){
		j = 0;
		strtok(shmPTR[i], "\n");
		k = strlen(shmPTR[i])-1;
		while (k > j){
			nonAlpha = 0;
			temp = tolower(shmPTR[i][j]);
			temp1 = tolower(shmPTR[i][k]);
			if (temp < 'a' || temp > 'z'){
				j++;
				nonAlpha = 1;
			}
			if (temp1 < 'a' || temp1 > 'z'){
				k--;
				nonAlpha = 1;
			}
			if ((temp != temp1) && (nonAlpha == 0)){
				strcpy(shmPTR[i], noPalHolder[noPal];
				noPal++;
				break;
			}
			if (nonAlpha == 0){
				k--;
				j++;
			}
		}
		if (k <= j){
			strcpy(shmPTR[i], palHolder[pal]);
			pal++;
		}
	}
	sem_t *sem;
	sem = sem_open("pSem", O_EXCL);
	sem_wait(sem);
	sem_post(sem);
	sem_close(sem);
	printf("Process %d exiting.\n", getpid());
	shmdt(shmPTR);
	return 0;
}
