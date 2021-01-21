/**
 * SO, 2016
 * Lab #11, Advanced IO Linux
 *
 * Task #1, Linux
 *
 * Example of poll usage
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <poll.h>
#include <wait.h>
#include <string.h>

#include "utils.h"

#define CLIENT_COUNT	10

#define MSG_SIZE	32

#define	ANY		-1
#define PIPE_READ	0
#define PIPE_WRITE	1

int pipes[CLIENT_COUNT][2];
int chld_pid[CLIENT_COUNT];

static int server(void)
{
	struct pollfd pdf[CLIENT_COUNT];
	int i;
	int recv_msgs;
	int recv_count;
	char msg[MSG_SIZE];
	int status, rc;

	printf("server: started\n");

	/* Close write pipe head and init poll */
	for (i = 0; i < CLIENT_COUNT; i++) {
		rc = close(pipes[i][PIPE_WRITE]);
		DIE(rc < 0, "server: close pipe failed");
		pdf[i].fd = pipes[i][PIPE_READ];
		pdf[i].events = POLLIN;
	}

	/* Number of received messages */
	recv_msgs = 0;

	while (recv_msgs < CLIENT_COUNT) {
		printf("server: waiting for messages (recv_msgs = %i)\n",
				recv_msgs);
		rc = poll(pdf, CLIENT_COUNT, -1);
		DIE(rc < 0, "poll failed");

		for (i = 0; i < CLIENT_COUNT; i++) {
			if ((pdf[i].revents & POLLIN) != 0) {
				recv_msgs++;
				recv_count = read(pipes[i][PIPE_READ], msg,
						MSG_SIZE);
				DIE(recv_count < 0, "read");

				msg[recv_count] = '\0';
				printf("received:%s\n", msg);
			}
		}
	}

	printf("server: going to wait for clients to end\n");

	for (i = 0; i < CLIENT_COUNT; ++i) {
		rc = close(pipes[i][PIPE_READ]);
		DIE(rc < 0, "close failed");

		rc = waitpid(ANY, &status, 0);
		DIE(rc < 0, "waitpid failed");
	}

	printf("server: exiting\n");
	return 0;
}

static int client(unsigned int index)
{
	char msg[MSG_SIZE];
	int rand_no, rc;

	/* Close read pipe head, wait random time and send a message */
	printf("client %i: started\n", index);

	rc = close(pipes[index][PIPE_READ]);
	DIE(rc < 0, "close failed");

	srandom(index);

	sleep(random()%10);

	printf("client %i: writing message\n", index);

	rand_no = (char)(random()%30);
	sprintf(msg, "<%i>:<%c>", getpid(), 'a'+rand_no);
	rc = write(pipes[index][PIPE_WRITE], msg, strlen(msg));
	DIE(rc < 0, "write failed");

	rc = close(pipes[index][PIPE_WRITE]);
	DIE(rc < 0, "close failed");

	printf("client %i: exiting\n", index);

	return 0;
}


int main(void)
{
	int i, rc;
	pid_t pid;

	/* Init pipes */
	for (i = 0 ; i < CLIENT_COUNT; i++) {
		rc = pipe(pipes[i]);
		DIE(rc < 0, "pipe call failed");
	}

	/* Crate clients as child processes */
	for (i = 0; i < CLIENT_COUNT; i++) {
		pid = fork();
		if (pid < 0) {
			perror("fork");
			return EXIT_FAILURE;
		} else if (!pid) {
			client(i);
			return EXIT_SUCCESS;
		}
		chld_pid[i] = pid;
	}

	server();

	return EXIT_SUCCESS;
}
