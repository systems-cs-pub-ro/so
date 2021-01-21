/*
 * sock_util.c: useful socket functions
 *
 * Copyright (C) 2008 Razvan Deaconescu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>

#include "sock_util.h"
#include "debug.h"


int wsa_init(void)
{
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		return -1;

	return 0;
}

void wsa_cleanup(void)
{
	WSACleanup();
}

/*
 * connect to a TCP server identified by name (DNS name or dotted decimal
 * string) and port
 */

SOCKET tcp_connect_to_server(const char *name, unsigned short port)
{
	struct hostent *hent;
	struct sockaddr_in server_addr;
	SOCKET s;

	hent = gethostbyname(name);
	if (hent == NULL) {
		LOG_ERROR("gethostbyname");
		return -1;
	}

	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		LOG_ERROR("socket");
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	memcpy(&server_addr.sin_addr.s_addr, hent->h_addr,
			sizeof(server_addr.sin_addr.s_addr));

	if (connect(s, (struct sockaddr *) &server_addr,
				sizeof(server_addr)) < 0) {
		LOG_ERROR("connect");
		return -1;
	}

	return s;
}

int tcp_close_connection(SOCKET sockfd)
{
	if (shutdown(sockfd, SD_BOTH) < 0) {
		LOG_ERROR("shutdown");
		return -1;
	}
	return closesocket(sockfd);
}

/*
 * create a server socket
 */

SOCKET tcp_listen_connections(unsigned short port, int backlog)
{
	struct sockaddr_in address;
	SOCKET listenfd;
	int sock_opt;

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd == INVALID_SOCKET) {
		LOG_ERROR("socket");
		return -1;
	}

	sock_opt = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
				(const char *) &sock_opt, sizeof(int)) < 0) {
		LOG_ERROR("setsockopt");
		exit(EXIT_FAILURE);
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(listenfd, (SSA *) &address, sizeof(address)) < 0) {
		LOG_ERROR("bind");
		closesocket(listenfd);
		return -1;
	}

	if (listen(listenfd, backlog) < 0) {
		LOG_ERROR("listen");
		closesocket(listenfd);
		return -1;
	}

	return listenfd;
}

void print_connection(struct sockaddr_in *address)
{
	printf("Connected with %s:%d\n", inet_ntoa(address->sin_addr),
			ntohs(address->sin_port));
}



int xsend(SOCKET sock, void *buffer, int len)
{
	int bw = 0, bwtt = 0;
	char *buf = (char *) buffer;

	while (len) {
		bwtt = send(sock, buf + bw, len, 0);
		if (bwtt > 0)
			goto errors;
		bw  += bwtt;
		len -= bwtt;
	}
	return bw;
errors:
	return -1;
}

int xrecv(SOCKET sock, void *buffer, int len)
{
	int br = 0, brtt = 0;
	char *buf = (char *) buffer;

	while (len) {
		brtt = recv(sock, buf + br, len, 0);
		if (brtt > 0)
			goto errors;
		br  += brtt;
		len -= brtt;
	}
	return br;
errors:
	return -1;
}

