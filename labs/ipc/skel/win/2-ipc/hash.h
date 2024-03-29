/**
 * SO
 * Lab IPC
 *
 * Task #2, Windows
 *
 * Hash function header
 */
#ifndef HASH_H_
#define HASH_H_	1

#define MAX_WORD_SIZE (1<<4)

#define BUCKET_COUNT (1<<8)

#define MAX_WORDS_PER_BUCKET (1<<8)

unsigned int hash(const char *str);

#endif
