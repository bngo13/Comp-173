/**
 * This program illustrates the functionality of the ipcs command on POSIX systems.
 *
 * sm.c
 *
 * Usage:
 *      gcc -o sm sm.c
 *
 *      ./sm 
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
	int segment_id;  			// identifier of the shared memory segment
	unsigned short mode;		// permissions of the segment
	struct shmid_ds shmbuffer; 	// the shared memory segment 


// Step 1: Create a new shared memory segment using shmget
	segment_id = shmget(IPC_PRIVATE, sizeof(struct shmid_ds), IPC_CREAT |  S_IRUSR | S_IWUSR);

	printf("New shared memory segment created: %d\n\n", segment_id);
	
	
// Step 2: Retrieve the information of the segment and store in shmbuffer		
	if (shmctl(segment_id, IPC_STAT, &shmbuffer) == -1) {
		fprintf(stderr,"Unable to access segment %d\n",segment_id);
		return -1;
	}

// Step 3: output information about the segment in the required format
	printf("SegmentID\tKey\tMode\t\tOwner\tSize\tNumber of attaches\n");
	printf("\n");
	printf("=========\t===\t====\t\t=====\t====\t==================\n\n");

	// Segment ID
	printf("%d\t\t", segment_id);

	// Key
	printf("%d\t", shmbuffer.shm_perm.__key);

	/** report on the permission */
	mode = shmbuffer.shm_perm.mode;

	/** OWNER */
	if (mode & 0400)
		printf("r");
	else
		printf("-");
	if (mode & 0200)
		printf("w");
	else
		printf("-");
	if (mode & 0100)
		printf("x");
	else
		printf("-");

	/** GROUP */
	if (mode & 0040)
		printf("r");
	else
		printf("-");
	if (mode & 0020)
		printf("w");
	else
		printf("-");
	if (mode & 0010)
		printf("x");
	else
		printf("-");

	/** WORLD */
	if (mode & 0004)
		printf("r");
	else
		printf("-");
	if (mode & 0002)
		printf("w");
	else
		printf("-");
	if (mode & 0001)
		printf("x");
	else
		printf("-");

	printf("\t");

	// Owner UID
	printf("%d\t", shmbuffer.shm_perm.uid);

	// Size
	printf("%zu\t", shmbuffer.shm_segsz);

	// Number of attaches
	printf("%zu\n", shmbuffer.shm_nattch);

	printf("\n");


// Step 4: Create a new process using fork

	pid_t pid = fork();


// Step 5: The child process sends a message to the parent process via the 
//         shared memory segment created in Step 1 and the parent prints out 
//         the message it received from the child process

	if (pid == 0) {
		char *sharedMem = (char *)shmat(segment_id, NULL, 0);
		sprintf(sharedMem, "Hello from the child process");
		shmdt(sharedMem);
		return 0;
	} else if (pid > 0) {
		char *sharedMem = (char *)shmat(segment_id, NULL, 0);
		wait(&pid);
		printf("Parent received from child: %s\n", sharedMem);
		shmdt(sharedMem);
	}
	return 0;
}
