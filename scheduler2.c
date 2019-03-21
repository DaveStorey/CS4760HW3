#ifndef SCHEDULER
#define SCHEDULER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>

//Signal handler to catch ctrl-c
static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

struct sharedArray{
	char * temp[100];
};

void scheduler(char* input, char* outfile, int limit, int total){
	int k=0, i=0, j=0, alive = 1, timeFlag = 0, totalFlag = 0, limitFlag = 0, shmID, childStart, totalSpawn = 0, status, noChildFlag = 1;
	//Pointer for the shared memory timer
	struct sharedArray * shmPTR;
	char *palind = NULL;
	struct sharedArray * holder = (struct sharedArray*)malloc(sizeof(struct sharedArray));
	static size_t lineSize = 0;
	ssize_t read;
	char parameter1[32], parameter2[32], parameter3[32], parameter4[32], parameter5[32];
	pid_t pid[total + 1], endID = 1;
	sem_t mutex;
	//Time variables for the time out function
	time_t when, when2;
	//File pointers for input and output, respectively
	FILE * fp;
	FILE * outPut;
	//Key variable for shared memory access.
	unsigned long key;
	srand(time(0));
	key = rand();
	//Setting initial time for later check.
	time(&when);
	outPut = fopen(outfile, "a");
	fp = fopen(input, "r");
	//Check for file error.
	if (outPut == NULL){
		perror("Error");
		printf("Output file could not be created.\n");
		exit(EXIT_SUCCESS);
	}
	if (fp == NULL){
		perror("Error");
		printf("File given: %s, error number: %d\n", input, errno);
		printf("No output generated.\n");
		exit(EXIT_SUCCESS);
	}
	//Get and access shared memory and semaphore, storing strings from file.
	sem_init(&mutex, 1, 1);
	while((read = getline(&palind, &lineSize, fp)) != -1){
		holder->temp[i] = malloc(strlen(palind)+1);
		strcpy(holder->temp[i], palind);
		i++;
	}
	shmID = shmget(key, sizeof(struct sharedArray), IPC_CREAT | IPC_EXCL | 0777);
	shmPTR = (struct sharedArray *) shmat(shmID, NULL, 0);
	shmPTR = holder;
	//for (k = 0; k < i; k++)
		printf("shmPTr address: %p\n", shmPTR);
	//Initializing pids to -1 
	for(k = 0; k <= total; k++){
		pid[k] = -1;
	}
	//Call to signal handler for ctrl-c
	signal(SIGINT, intHandler);
	//While loop keeps running until all children are dead, ctrl-c, or time is reached.
	while((alive > 0) && (keepRunning == 1) && (timeFlag == 0)){
		time(&when2);
		if ((when2 - when) >= 15){
			timeFlag = 1;
		}
		
		/*If statement will only run check for new children to spawn if limit has not been hit.  If limit has been hit, it will allow the clock to continue to increment to allow currently alive children to naturally die.*/
		if((totalFlag == 0) && (limitFlag == 0)){
			if((pid[j] = fork()) == 0){
			//Converting key, shmID and life to char* for passing to exec.
				sprintf(parameter1, "%li", key);
				sprintf(parameter2, "%d", shmID);
				sprintf(parameter3, "%d", childStart);
				sprintf(parameter4, "%d", i);
				sprintf(parameter5, "%p", shmPTR);
				char * args[] = {parameter1, parameter2, parameter3, parameter4, parameter5, NULL};
				//fprintf(outPut, "Child process %d launched.\n", getpid());
				execvp("./palin\0", args);
			}
			else{
			//If statement to reset alive counter after getting into while loop initially
				if (noChildFlag > 0){
					alive--;
					noChildFlag = 0;
				}
				alive++;
				totalSpawn++;
				j++;
			}
			//EndID to check for status of group children
		}
		endID = waitpid(0, &status, WNOHANG | WUNTRACED);
		if (endID == -1){
			alive = 0;
		}
		else if (endID == 0){
		}
		else{
		//For loop steps through array of pids, checking if return value matches.
			for(k = 0; k <= i; k++){
				if(endID == pid[k]){
					alive--;
					if (alive >= 0){
						if (WIFEXITED(status)){
							limitFlag = 0;
							//fprintf(outPut, "Child process %d terminated.\n", pid[k]);
							limitFlag = 0;
						}
						else if (WIFSIGNALED(status)){
							printf("Child ended with an uncaught signal, %d.\n", status);
							limitFlag = 0;
						}
						else if (WIFSTOPPED(status)){
							printf("Child process has stopped.\n");
						}
					}
				}
			}
		}
		//If statements check against total children spawned and instant limit.
		if (totalSpawn >= total){
			totalFlag = 1;
		}
		if (alive >= limit){
			limitFlag = 1;
		}
	}
	if(timeFlag == 1){
		printf("Program has reached its allotted time, exiting.\n");
		//fprintf(outPut, "Scheduler terminated at due to time limit.\n");
	}
	if(totalFlag == 1){
		printf("Program has reached its allotted children, exiting.\n");
		//fprintf(outPut, "Scheduler terminated at due to process limit.\n");
	}
	if(keepRunning == 0){
		printf("Terminated due to ctrl-c signal.\n");
		//fprintf(outPut, "Scheduler terminated due to ctrl-c signal.\n");
	}
	j = 0;
	if(alive > 0){
		while(pid[j] != -1){
			kill(pid[i], SIGQUIT);
			printf("Killing process %d\n", pid[j]);
			j++;
		}
	}
	for(j=0; j<=0; j++){
		free(holder->temp[j]);
	}
	shmdt(shmPTR);
	shmctl(shmID, IPC_RMID, NULL);
	fclose(fp);
	fclose(outPut);
}

#endif
