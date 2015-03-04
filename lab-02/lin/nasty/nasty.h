/** 
 * SO, 2011
 * Lab #2, Procese
 * 
 * nasty.h - Helper driver header file
 */
 
#ifndef NASTY_H_
#define NASTY_H_

/*FIXME: do we really need nasty.h?
 * everything can be placed inside nasty.c
 */

#define NASTY_DRIVER_VERSION "1.0.0"
#define NASTY_DRIVER_NAME    "nasty"
#define MAX_CHUNK_SIZE	     16

char poetry[] = "\n\
	I have a dog.\n\
	His name is Fred.\n\
	He won't play fetch.\n\
	He won't play dead.\n\n\
	He won't shake hands\n\
	or sit or stay\n\
	or bark or beg\n\
	or run and play.\n\n\
	He won't roll over,\n\
	shake or crawl.\n\
	In fact, he won't\n\
	do tricks at all.\n\n\
	When folks ask why\n\
	I tell them that's\n\
	because my dog\n\
	was raised by cats.\n";


#endif /* nasty.h */
