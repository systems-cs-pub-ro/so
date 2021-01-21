/**
 * SO, 2017 - Lab #07, Profiling
 * Task #1, Linux
 *
 * Hash function
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define HASH_SIZE (1 << 17)
#define MAX_WORDS (1 << 16)

struct hash {
	const char *key;
	unsigned key_hash;
	void *value;
};

struct hash hash[HASH_SIZE];

unsigned hash_func(const char *key)
{
	unsigned sum = 0;

	while (*key != 0)
		sum = (sum + *key++) % HASH_SIZE;
	return sum;
}

void hash_init(void)
{
	memset(hash, 0x00, sizeof(hash));
}

void hash_done(void)
{
	int i;

	for (i = 0; i < HASH_SIZE; ++i) {
		free((void *)hash[i].key);
		hash[i].key = NULL;
	}
}

void hash_add(const char *key, void *value)
{
	const unsigned key_hash = hash_func(key);
	unsigned index = key_hash;

	while (hash[index].key != NULL)
		if (++index == HASH_SIZE)
			index = 0;

	hash[index].key = strdup(key);
	hash[index].key_hash = key_hash;
	hash[index].value = value;
}

unsigned hash_search_index(const char *key)
{
	const unsigned key_hash = hash_func(key);
	unsigned index = key_hash;

	while (hash[index].key != NULL &&
			(hash[index].key_hash != key_hash ||
			 strcmp(hash[index].key, key) != 0))
		if (++index == HASH_SIZE)
			index = 0;

	return hash[index].key ? index : HASH_SIZE;
}

int hash_search(const char *key, void **value)
{
	unsigned index = hash_search_index(key);

	if (index == HASH_SIZE)
		return 1;

	*value = hash[index].value;
	return 0;
}

const char *words[MAX_WORDS];
int num_words;

int main(void)
{
	FILE *f;
	char temp[128];
	clock_t start;
	int i;

	f = fopen("english.0", "rt");
	hash_init();

	while (num_words < MAX_WORDS &&
		fgets(temp, sizeof(temp) / sizeof(temp[0]), f) != NULL) {
		words[num_words] = strdup(temp);
		++num_words;
	}

	/* ADDING */
	printf("adding %d english words... ", num_words);
	fflush(stdout);

	start = clock();
	for (i = 0; i < num_words; ++i)
		hash_add(words[i], (void *)words[i]);
	printf("done (%.3f s)\n", 1. * (clock() - start) / CLOCKS_PER_SEC);

	/* SEARCHING EXISTING */
	printf("searching %d english words... ", num_words);
	fflush(stdout);

	start = clock();
	for (i = 0; i < num_words; ++i) {
		int random_index = rand() % num_words;

		hash_search_index(words[random_index]);
	}
	printf("done (%.3f s)\n", 1. * (clock() - start) / CLOCKS_PER_SEC);

	/* SEARCHING NONEXISTING */
	printf("searching %d random letter words... ", num_words);
	fflush(stdout);

	start = clock();
	for (i = 0; i < num_words; ++i) {
		int j = rand() % 8;

		temp[j] = 0;
		while (j--)
			if (rand() % 2)
				temp[j] = rand() % 26 + 'A';
			else
				temp[j] = rand() % 26 + 'a';

		hash_search_index(temp);
	}
	printf("done (%.3f s)\n", 1. * (clock() - start) / CLOCKS_PER_SEC);

	hash_done();
	fclose(f);
	return 0;
}
