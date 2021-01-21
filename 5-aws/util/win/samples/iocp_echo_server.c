/*
 * iocp-based echo server. Uses IoCompletionPorts to wait for
 * multiple operations.
 *
 * 2011-2017, Operating Systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <winsock2.h>
#include <mswsock.h>

#include "util.h"
#include "debug.h"
#include "sock_util.h"
#include "w_iocp.h"

#define ECHO_LISTEN_PORT		42424

#ifndef BUFSIZ
#define BUFSIZ				8192
#endif


/* server socket file handle */
static SOCKET listenfd;

/* IoCompletionPort handle */
static HANDLE iocp;

enum connection_state {
	STATE_DATA_RECEIVED,
	STATE_DATA_SENT,
	STATE_CONNECTION_CLOSED
};

/* structure acting as a connection handler */
struct connection {
	SOCKET sockfd;
	char recv_buffer[BUFSIZ];
	char send_buffer[BUFSIZ];
	/* buffers used for receiving messages and then echoing them back */
	WSABUF recv_buffers[1];
	WSABUF send_buffers[1];
	size_t bytes_recv;
	size_t bytes_sent;
	WSAOVERLAPPED recv_ov;
	WSAOVERLAPPED send_ov;
};

/*
 * Anonymous structure used to "confine" data regardin asynchronous accept
 * operations (handled through AcceptEx and Io Completion Ports).
 */
static struct {
	SOCKET sockfd;
	char buffer[BUFSIZ];
	size_t len;
	OVERLAPPED ov;
} ac;

/*
 * Initialize connection structure on given socket.
 */

static struct connection *connection_create(SOCKET sockfd)
{
	struct connection *conn = malloc(sizeof(*conn));

	DIE(conn == NULL, "malloc");

	conn->sockfd = sockfd;
	memset(conn->recv_buffer, 0, BUFSIZ);
	memset(conn->send_buffer, 0, BUFSIZ);
	conn->recv_buffers[0].buf = conn->recv_buffer;
	conn->send_buffers[0].buf = conn->send_buffer;
	conn->recv_buffers[0].len = BUFSIZ;
	conn->send_buffers[0].len = BUFSIZ;

	memset(&conn->recv_ov, 0, sizeof(conn->recv_ov));
	memset(&conn->send_ov, 0, sizeof(conn->send_ov));

	return conn;
}

/*
 * Add a non bound socket to the connection. The socket is to be bound
 * by AcceptEx.
 */

static struct connection *connection_create_with_socket(void)
{
	SOCKET s;

	s = socket(PF_INET, SOCK_STREAM, 0);
	DIE(s == INVALID_SOCKET, "socket");

	return connection_create(s);
}

/*
 * Remove connection handler.
 */

static void connection_remove(struct connection *conn)
{
	dlog(LOG_LEVEL, "Remove connection.\n");
	closesocket(conn->sockfd);
	free(conn);
}

/*
 * Copy receive buffer to send buffer (echo).
 */

static void connection_copy_buffers(struct connection *conn)
{
	memcpy(conn->send_buffer, conn->recv_buffer, conn->bytes_recv);
	conn->send_buffers[0].len = conn->bytes_recv;
}

/*
 * Prepare data for overlapped I/O send operation.
 */

static void connection_prepare_socket_send(struct connection *conn)
{
	connection_copy_buffers(conn);
}

/*
 * Use WSASend to asynchronously send message through socket.
 */

static void connection_schedule_socket_send(struct connection *conn)
{
	DWORD flags;
	int rc;

	memset(&conn->send_ov, 0, sizeof(conn->send_ov));

	flags = 0;
	rc = WSASend(
			conn->sockfd,
			conn->send_buffers,
			1,
			NULL,
			flags,
			&conn->send_ov,
			NULL);
	if (rc && (rc != SOCKET_ERROR || WSAGetLastError() != WSA_IO_PENDING))
		exit(EXIT_FAILURE);
}

/*
 * Use WSARecv to asynchronously receive message from socket.
 */

static void connection_schedule_socket_receive(struct connection *conn)
{
	DWORD flags;
	int rc;

	memset(&conn->send_ov, 0, sizeof(conn->send_ov));

	flags = 0;
	rc = WSARecv(
			conn->sockfd,
			conn->recv_buffers,
			1,
			NULL,
			&flags,
			&conn->recv_ov,
			NULL);
	if (rc && (rc != SOCKET_ERROR || WSAGetLastError() != WSA_IO_PENDING))
		exit(EXIT_FAILURE);
}

/*
 * Overllaped I/O send operation completed (as signaled by I/O Completion
 * Port). Close connection.
 */

static void connection_complete_socket_send(struct connection *conn,
	WSAOVERLAPPED *ovp)
{
	/* Closing the socket also removes it from Completion port. */
	connection_remove(conn);
}

/*
 * Overllaped I/O receive operation completed (as signaled by I/O Completion
 * Port). Send message back.
 */

static void connection_complete_socket_receive(struct connection *conn,
	WSAOVERLAPPED *ovp)
{
	BOOL bRet;
	DWORD flags;

	bRet = WSAGetOverlappedResult(
			conn->sockfd,
			ovp,
			&conn->bytes_recv,
			FALSE,
			&flags);
	DIE(bRet == FALSE, "WSAGetOverlappedResult");

	/* In case of no bytes received, consider connection terminated. */
	if (conn->bytes_recv == 0) {
		connection_remove(conn);
		return;
	}

	connection_prepare_socket_send(conn);
	connection_schedule_socket_send(conn);
}

/*
 * Schedule overlapped operation for accepting a new connection.
 */

static void create_iocp_accept(void)
{
	BOOL bRet;

	memset(&ac, 0, sizeof(ac));

	/* Create simple socket for acceptance */
	ac.sockfd = socket(PF_INET, SOCK_STREAM, 0);
	DIE(ac.sockfd == INVALID_SOCKET, "socket");

	/* Launch overlapped connection accept through AcceptEx. */
	bRet = AcceptEx(
			listenfd,
			ac.sockfd,
			ac.buffer,
			0,
			128,
			128,
			&ac.len,
			&ac.ov);
	DIE(bRet == FALSE && WSAGetLastError() != ERROR_IO_PENDING, "AcceptEx");
}

/*
 * Handle a new connection request on the server socket.
 */

static void handle_new_connection(OVERLAPPED *ovp)
{
	struct connection *conn;
	char abuffer[64];
	HANDLE hRet;
	int rc;

	rc = setsockopt(
			ac.sockfd,
			SOL_SOCKET,
			SO_UPDATE_ACCEPT_CONTEXT,
			(char *) &listenfd,
			sizeof(listenfd)
		  );
	DIE(rc < 0, "setsockopt");

	rc = get_peer_address(ac.sockfd, abuffer, 64);
	if (rc < 0) {
		ERR("get_peer_address");
		return;
	}

	dlog(LOG_DEBUG, "Accepted connection from %s\n", abuffer);

	/* Instantiate new connection handler. */
	conn = connection_create(ac.sockfd);

	/* Add socket to IoCompletionPort. */
	hRet = w_iocp_add_key(iocp, (HANDLE) conn->sockfd, (ULONG_PTR) conn);
	DIE(hRet != iocp, "w_iocp_add_key");

	/* Schedule receive operation. */
	connection_schedule_socket_receive(conn);

	/* Use AcceptEx to schedule new connection acceptance. */
	create_iocp_accept();

}

/*
 * Process overlapped I/O operation: data has been received from or
 * has been sent to the socket.
 */

static void handle_aio(struct connection *conn, size_t bytes, OVERLAPPED *ovp)
{
	if (ovp == &conn->send_ov) {
		dlog(LOG_LEVEL, "Send operation completed.\n");
		connection_complete_socket_send(conn, ovp);
	} else if (ovp == &conn->recv_ov) {
		dlog(LOG_LEVEL, "Recv operation completed.\n");
		connection_complete_socket_receive(conn, ovp);
	} else
		dlog(LOG_ERR, "Unknown operation completed.\n");
}

int main(void)
{
	BOOL bRet;
	HANDLE hRet;

	wsa_init();

	iocp = w_iocp_create();
	DIE(iocp == NULL, "w_iocp_create");

	/* Create server socket. */
	listenfd = tcp_create_listener(ECHO_LISTEN_PORT,
		DEFAULT_LISTEN_BACKLOG);
	DIE(listenfd == INVALID_SOCKET, "tcp_create_listener");

	hRet = w_iocp_add_handle(iocp, (HANDLE) listenfd);
	DIE(hRet != iocp, "w_iocp_add_handle");

	/* Use AcceptEx to schedule new connection acceptance. */
	create_iocp_accept();

	dlog(LOG_INFO, "Server waiting for connections on port %d\n",
		ECHO_LISTEN_PORT);

	/* server main loop */
	while (1) {
		OVERLAPPED *ovp;
		ULONG_PTR key;
		DWORD bytes;

		/* Wait for overlapped I/O. */
		bRet = w_iocp_wait(iocp, &bytes, &key, &ovp);
		if (bRet == FALSE) {
			DWORD err;

			err = GetLastError();
			if (err == ERROR_NETNAME_DELETED) {
				connection_remove((struct connection *) key);
				continue;
			}
			DIE(bRet == FALSE, "w_iocp_wait");
		}

		/*
		 * Switch I/O notification types. Consider
		 *   - new connection requests (on server socket);
		 *   - socket communication (on connection sockets).
		 */

		if (key == listenfd) {
			dlog(LOG_DEBUG, "New connection\n");
			handle_new_connection(ovp);
		} else {
			handle_aio((struct connection *) key, bytes, ovp);
		}
	}

	wsa_cleanup();

	return 0;
}
