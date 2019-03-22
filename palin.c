#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

int main(int argc, char * argv[]){
	int i, j = 0, k, nonAlpha, noPal = 0, pal = 0;
	char * ptr;
	int palHolder[5], noPalHolder[5];
	char temp, temp1;
	pid_t pid = getpid();
	FILE * outputPal;
	FILE * outputNoPal;
	time_t now;
	unsigned long key = strtoul(argv[0], &ptr, 10);
	int listStart = (int) strtol(argv[1], &ptr, 10);
	int shmID = shmget(key, sizeof(char[80][100]), O_RDONLY);
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
				noPalHolder[noPal] = i;
				noPal++;
				break;
			}
			if (nonAlpha == 0){
				k--;
				j++;
			}
		}
		if (k <= j){
			palHolder[pal] = i;
			pal++;
		}
	}
	sem_t *sem;
	sem = sem_open("pSem", O_EXCL);
	sem_wait(sem);
	time(&now);
	fprintf(stderr, "Process %d entering critical section at %s\n", pid, ctime(&now));
	sleep(2);
	outputPal = fopen(argv[3], "a");
	outputNoPal = fopen(argv[4], "a");
	for(i = 0; i < pal; i++){
		fprintf(outputPal, "%d	%s	%s\n", pid, argv[2], shmPTR[palHolder[i]]);
	}
	for(i = 0; i < noPal; i++){
		fprintf(outputNoPal, "%d	%s	%s\n", pid, argv[2], shmPTR[noPalHolder[i]]);
	}
	fclose(outputPal);
	fclose(outputNoPal);
	sleep(2);
	fprintf(stderr, "Process %d exiting critical section at %s\n", pid, ctime(&now));
	sem_post(sem);
	sem_close(sem);
	printf("Process %d exiting.\n", getpid());
	shmdt(shmPTR);
	return 0;
}
