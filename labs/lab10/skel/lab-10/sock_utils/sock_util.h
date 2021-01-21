/*
 * sock_util.h: useful socket macros and structures
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

#ifndef SOCK_UTIL_H_
#define SOCK_UTIL_H_	1

#include <stdio.h>
#include <winsock2.h>

/* Visual Studio must include those */
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")


#define SSA	struct sockaddr

int wsa_init(void);
void wsa_cleanup(void);

int parse_size(const char *nptr, size_t *val);

SOCKET tcp_connect_to_server(const char *name, unsigned short port);
int tcp_close_connection(SOCKET s);
SOCKET tcp_listen_connections(unsigned short port, int backlog);
void print_connection(struct sockaddr_in *address);
int xsend(SOCKET sock, void *buffer, int len);
int xrecv(SOCKET sock, void *buffer, int len);

#endif
