#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h> //
#include <sys/shm.h>  // shared mem library routines like shmget(), shmat(), shmdt()
#include <sys/stat.h> // flags like S_XXXX
#include <unistd.h>   // contains calls like fork(0 wait() etc
#include <time.h>

#define MAX_ELEMENTS 100
int A[MAX_ELEMENTS], B[MAX_ELEMENTS];
int *C;
//shared Memory(across parent and child)
int shmKey = 1234;
int main()
{
	int pid0, pid1, pid2, pid3;
	int a, b, c, d;
	int n = 4, numElementsPerChild = MAX_ELEMENTS / n, i;
	int shmId, shmSize = MAX_ELEMENTS * sizeof(int);

	//Create share memory segment
	shmId = shmget(shmKey, shmSize, IPC_CREAT | 0666);

	//shmat() system return the address which is shared between
	//all children and parent also.
	C = (int*)shmat(shmId, NULL, 0);
	int j, k, l;
	srand((int)time(NULL));
	for (j = 0; j < MAX_ELEMENTS; j++) {
		A[j] = (rand() % 13) + 1;
	}
	for (k = 0; k < MAX_ELEMENTS; k++) {
		B[k] = (rand() % 13) + 1;
	}

	//create a child
	if ((pid0 = fork()) == 0) {
		int childId = 0, i;
		printf("Child0 = %d\n", getpid());
		for(i = 0; childId*numElementsPerChild<=i<(childId+1); i++){
			C[i] = A[i] + B[i]; 
		}
		exit(100);
	} else if ((pid1 = fork()) == 0) {
		int childId = 1, i;
		printf("Child1 = %d\n", getpid());
		for(i = 0; childId*numElementsPerChild<=i<(childId+1); i++){
			C[i] = A[i] + B[i];
		}
		exit(200);
	} else if ((pid2 = fork()) == 0) {
		int childId = 2, i;
		printf("Child2 = %d\n", getpid());
		for(i = 0; childId*numElementsPerChild<=i<(childId+1); i++){
			C[i] = A[i] + B[i];
		}
		exit(3);
	} else if ((pid3 = fork()) == 0) {
		//FILL IN: simlar code like child 0
		int childId = 3, i;
                printf("Child3 = %d\n", getpid());
		for(i = 0; childId*numElementsPerChild<=i<(childId+1); i++){
			C[i] = A[i] + B[i];
		}
		exit(4);
	}
	
	//wait for children to finish calculating their individual sums
	wait(&a);
	wait(&b);
	wait(&c);
	wait(&d);

	// get the return status, returned thru exit(xxx) by child
	printf ("Parent a=%d\n",  WEXITSTATUS(a));

	// FILL IN: Parent print arrays A[], B[], C[]
	for(j=0; j<MAX_ELEMENTS; j++){
		printf("A[%d] = %d\n",j,A[j]);
	}
	for(k=0; k<MAX_ELEMENTS; k++){
		printf("B[%d] = %d\n",k,B[k]);
	}
	for(l=0; l<MAX_ELEMENTS; l++){
		printf("C[%d] = %d\n",l,C[l]);
	}

	// check if C[i] = A[i] + B[i] for all i

	// remove the shared memory

	shmctl(shmId, IPC_RMID, 0);
}
