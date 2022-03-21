/**
 * SO
 * Lab #5
 *
 * Task #2, Linux
 *
 * Generic queue functions
 */
#ifndef GENERIC_QUEUE_H
#define GENERIC_QUEUE_H		1

#define MQ_MAXMSG 10

#include "common.h"

/**
 * Create and return a new message queue
 */
msgq_t msgq_create(const char *name);

/**
 * Open an existing queue
 */
msgq_t msgq_get(const char *name);

/**
 * Detach from an existing queue
 */
void msgq_detach(msgq_t x);

/**
 * Destroy an existing queue
 */
void msgq_destroy(msgq_t x);

/**
 * Send a message through a queue
 */
void msgq_send(msgq_t m, message_t *x);

/**
 * Receive a message from a queue
 */
void msgq_recv(msgq_t m, message_t *x);

#endif
