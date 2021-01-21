/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Hashtable implementation
  */
#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "common.h"


/**
 * zero out the content of a hashtable
 * @param sems if non-NULL - list of semaphores
 */

void htable_clear(struct htable *table, sema_t *sems){
	int i;

	if (table == NULL) {
		/* silently abort */
		return;
	}

	for (i=0; i<BUCKET_COUNT; i++) {
		memset(table->words[i], 0, sizeof(table->words[i]));
		table->first_free[i] = 0;
	}
}

/**
 * add a word to the hashtable
 * @return 1 on success
 */
int htable_insert(struct htable* table, const char* word) {
	unsigned int key, pos;

	if (table == NULL) {
		/* silently abort */
		return 0;
	}

	key = hash(word);

	/* capacity check */
	if (table->first_free[key] == MAX_WORDS_PER_BUCKET) 
		return 0;

	pos = table->first_free[key];
	strncpy((table->words[key][pos]), word, MAX_WORD_SIZE);
	table->first_free[key]++;

	return 1;
}

/**
 * remove a word from a hashtable
 * @return 1 if found and removed
 */
int htable_delete(struct htable* table, const char* word) {
	unsigned int key, i, j;

	if (table == NULL) {
		/* silently abort */
		return 0;
	}


	key = hash(word);
	
	for (i=0; i<table->first_free[key]; i++)
		if (!strncmp(word, (table->words[key][i]), MAX_WORD_SIZE))
			break;	

	if (i == table->first_free[key]) {
		dprintf("Word %s not found int bucket %d\n", word, key);
		return 0;
	}

	dprintf("Word %s found in bucket %d on pos %d", word, key, i);
		
	for (j=i; j<table->first_free[key]-1; j++)
		strncpy((table->words[key][j]), table->words[key][j+1], MAX_WORD_SIZE);

	i = table->first_free[key]; 

	table->first_free[key] --;

	memset(table->words[key][i], 0, MAX_WORD_SIZE);

	return 1;
}

/**
 * Print the hashtable
 */
void htable_print(struct htable* table, sema_t *sems) {
	unsigned int i, j;

	if (table == NULL) {
		/* silently abort */
		return;
	}

	for (i=0; i<BUCKET_COUNT; i++) {
		if (table->first_free[i] > 0) {
			for (j=0; j<table->first_free[i]; j++)
				printf("%s ", table->words[i][j]);
			printf("\n");
		}
	}
}
