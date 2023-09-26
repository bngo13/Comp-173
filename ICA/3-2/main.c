#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	int childPipe[2];
	int parentPipe[2];


	char strBuf[11];
	char* sendString = "Hi There";
	int strSize = strlen(sendString);

	pipe(childPipe);
	pipe(parentPipe);

	pid_t pid = fork();

	if (pid == 0) { // Child
		read(childPipe[0], strBuf, strSize);
		
		printf("Child Received %s\n", strBuf);

		// Modify to reverse case
		for (int i = 0; i < strSize; i++) {
			if (isupper(strBuf[i])) {
				strBuf[i] = tolower(strBuf[i]);

			} else {
				strBuf[i] = toupper(strBuf[i]);
			}
		}

		printf("Child Sending %s\n", strBuf);

		// Write to parent

		write(parentPipe[1], strBuf, strSize);
		
	} else if (pid > 0) { // Parent
		printf("Parent Sending %s\n", sendString);

		// Write to Child
		write(childPipe[1], sendString, strSize);
		wait(NULL); // Wait for child to finish

		// Read from child
		read(parentPipe[0], strBuf, strSize);
		printf("Parent Received %s\n", strBuf);
	} else {
		return -1;
	}
}
