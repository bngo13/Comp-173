#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	/* Get user input for voters and vote amount */
	int voterCount, voteRounds = 0;

	printf("Enter the number of voters: ");
	scanf("%d", &voterCount);

	printf("Enter the number of rounds: ");
	scanf("%d", &voteRounds);

	if (voterCount < 3 || voteRounds < 1) {
		return -1;
	}

	/* Start voting */
	// Initialize Random Num Gen	
	time_t t;
	srand((unsigned) time(&t));

	int pipes[voterCount][2];
	int votes[voterCount];

	for (int i = 0; i < voterCount; i++) {
		int parentPipe[2];
		pipe(parentPipe);
		pipes[i][0] = parentPipe[0];
		pipes[i][1] = parentPipe[1];
	}

	// Pretty Printing
	printf("\n\n");

	// Start voting
	
	pid_t pid;

	for (int i = 0; i < voteRounds; i++) {
		pid = getpid(); // Get parent pid after every run

		printf("----- Round %d -----\n", i + 1);
		for (int i = 0; i < voterCount; i++) {
			pid = fork();
			//int vote = rand() % 2;
			if (pid == 0) {
				/* Child Process */

				// Vote Gathering
				srand(time(0) + getpid()); // Set random seed plus pid for more randomness
				int vote = rand() % 2;

				// Write vote to pipe
				write(pipes[i][1], &vote, sizeof(vote));

				// Exit Child
				return 0;
			}
		}
		
		// Wait for children
		wait(NULL);

		int voteCount = 0;
		for (int i = 0; i < voterCount; i++) {
			if (1) {
				// Parent
				
				// Read votes from children
				read(pipes[i][0], &votes[i], sizeof(votes[i]));
				printf("Voter %d Votes: %s\n", i, votes[i] == 0 ? "NO" : "YES");
				voteCount += votes[i] == 0 ? -1 : 1;
				
			}
		}

		printf("--- Round %d: Voting Result: %s\n\n", i + 1, voteCount == 0 ? "TIE" : voteCount > 0 ? "YES" : "NO");
	}

	

	return 0;
}
