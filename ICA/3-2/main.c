#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	char inbuf[11];
	int pipes[2], pid, nbytes; // pipe[1] sender | pipe[0] receiver
	
	char* str = "Hello There";
	int strSize = strlen(str);

	pipe(pipes);

	if ((pid = fork()) > 0) {
		// Parent
		write(pipes[1], str, strSize);
		close(pipes[1]);
		wait(&pid);

	} else {
		// Child
		close(pipes[1]);
		while ((nbytes = read(pipes[0], inbuf, strSize)) > 0) {
			for (int i = 0; i < strSize; i++) {
				if (isupper(inbuf[i])) {
					inbuf[i] = tolower(inbuf[i]);
				} else {
					inbuf[i] = toupper(inbuf[i]);
				}	
			}
			printf("%s\n", inbuf);
		}
		if (nbytes != 0) {
			exit(2);
		}

	}

	return 0;
}
