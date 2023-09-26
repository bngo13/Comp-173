#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
	/* Get user input for voters and vote amount */
	int voterCount, voteRounds = 0;

	printf("Enter the number of voters: ");
	scanf("%d", &voterCount);

	printf("Enter the number of rounds: ");
	scanf("%d", &voteRounds);
	
	/* Start voting */
	// Initialize Random Num Gen	
	time_t t;
	srand((unsigned) time(&t));





	return 0;
}
