/**
 * SO
 * Recap
 * Task #11
 *
 * Why does `open` fail at the 964th file?
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAGIC 17

static void do_work(int i)
{
	int p[2];
	int rd;
	int wr;
	int status;
	int bytes;
	pid_t pid;
	char buf[10];

	if (pipe(p) < 0) {
		perror("pipe failed");
		exit(1);
	}
	
	rd = p[0];
	wr = p[1];
	
	pid = fork();
	switch (pid) {
	case -1:
		printf("fork failed\n");
		exit(1);

	case 0:		/* Child process */
		close(wr);
		if (dup2(rd, STDIN_FILENO) < 0) {
			perror("dup2 failed");
			exit(1);
		}

		while ((bytes = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
			buf[bytes] = 0;
			printf("CHILD %d : %s\n", i / 17, buf);
		}
		exit(0);

	default:	/* Parent process */
		write(wr, "Hello", 5);
		close(wr);
		waitpid(pid, &status, 0);
		break;
	}
}

int main(void)
{
	int fds[1024];
	int i = 0;

	while (i < 1024) {
		fds[i] = open("test.txt", O_CREAT | O_RDWR, 0777);
		if (fds[i] < 0) {
			printf("Open %d failed\n", i);
			perror("errno:");
			exit(1);
		}

		if (i % MAGIC == 0)
			do_work(i);

		i++;
	}

	return 0;
}
