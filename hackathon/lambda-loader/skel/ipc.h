#ifndef _IPC_H
#define _IPC_H

#define BUFSIZE 1024
#define MAX_CLIENTS 1024
#define SOCKET_NAME "/tmp/sohack.socket"

int create_socket();
int connect_socket(int);
ssize_t send_socket(int, const char *, size_t);
ssize_t recv_socket(int, char *buf, size_t);
void close_socket(int);

#endif /* _IPC_H */
