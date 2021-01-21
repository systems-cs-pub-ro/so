/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Generic queue functions
  */
#ifndef GENERIC_QUEUE_H
#define GENERIC_QUEUE_H		1

#define MQ_MAXMSG	10

#include "common.h"

/**
 * create and return a new message queue
 */
msgq_t msgq_create(const char* name);

/**
 * open an existing queue
 */
msgq_t msgq_get(const char* name);

/**
 * detach from an existing queue
 */
void   msgq_detach(msgq_t x);

/**
 * destroy an existing queue
 */
void   msgq_destroy(msgq_t x);

/**
 * send a message through a queue
 */
void   msgq_send(msgq_t m, message_t *x);

/**
 * receive a message from a queue
 */
void   msgq_recv(msgq_t m, message_t *x);

#endif
