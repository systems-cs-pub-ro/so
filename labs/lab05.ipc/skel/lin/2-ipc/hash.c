/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Hash function implementation
  */
#include "hash.h"

/**
 * Hash function based on djb2 of Dan Bernstein
 * http://www.cse.yorku.ca/~oz/hash.html
 * @return dispersion value (key)
 */
unsigned int hash(const char *str)
{
	unsigned int hash = 5381;
	int c;

	while ( (c = *str++) != 0 )
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return (hash % BUCKET_COUNT);
}

