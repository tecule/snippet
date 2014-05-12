#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

/* Execute the command using this shell program.  */
#define SHELL "/bin/sh"

int main(int argc, char **argv) {
	my_system("ping localhost -c 10");

	exit(0);
}

int my_system(const char *command) {
	int status;
	pid_t pid;

	pid = fork();
	if (pid == 0) {
		/* This is the child process.  Execute the shell command. */
		execl(SHELL, SHELL, "-c", command, NULL );
		_exit(EXIT_FAILURE);
	} else if (pid < 0) {
		/* The fork failed.  Report failure.  */
		status = -1;
	} else {
		/* This is the parent process.  Wait for the child to complete.  */
		if (waitpid(pid, &status, 0) != pid)
			status = -1;

		printf("child process terminated.\n");
		printf("WIFEXITED: %d\n", WIFEXITED(status));
		printf("WEXITSTATUS: %d\n", WEXITSTATUS(status));
	}
	return status;
}
