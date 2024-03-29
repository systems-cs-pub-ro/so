/**
 * SO
 * Lab IPC
 *
 * Task #2, Linux
 *
 * Unix implementation of semaphore functions
 */
#ifndef HASHTABLE_H_
#define HASHTABLE_H_ 1

#include "hash.h"
#include "common.h"

struct htable {
	char words[BUCKET_COUNT][MAX_WORDS_PER_BUCKET][MAX_WORD_SIZE];
	unsigned int first_free[BUCKET_COUNT];
};

/**
 * Zero out the content of a hashtable
 * @param sems if non-NULL  - list of semaphores
 */
void htable_clear(struct htable *table, sema_t *sems);

/**
 * Add a word to the hashtable
 * @return 1 on success
 */
int htable_insert(struct htable *table, const char *word);

/**
 * Remove a word from a hashtable
 * @return 1 if found and removed
 */
int htable_delete(struct htable *table, const char *word);

/**
 * Print a hashtable
 */
void htable_print(struct htable *table, sema_t *sems);

#endif
