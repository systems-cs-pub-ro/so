/**
 * SO, 2014
 * Lab #6
 *
 * Task #8, lin
 *
 * Changing code will running
 */
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ALIGN_TO_PAGE(addr) ((void *)((intptr_t)(addr) & ~(getpagesize()-1)))

static int pagesize;

/*
08048394 <foo>:
 8048394:       55                      push   %ebp
 8048395:       89 e5                   mov    %esp,%ebp
 8048397:       b8 0c 00 00 00          mov    $0xc,%eax
 804839c:       c9                      leave
 804839d:       c3                      ret
*/
int foo(void)
{
	return 12;
}

int main(void)
{
	void *addr, *addr_aligned, *page_end;
	int (*f)();
	char *crt;

	pagesize = getpagesize();

	// adresa functiei
	addr = &foo;
	printf("adresa functiei %p\n", addr);

	// aliniere adresa a multiplu de pagina
	addr_aligned = ALIGN_TO_PAGE(addr);
	printf("adresa paginii ce contine functia %p\n", addr_aligned);

	// final pagina
	page_end = addr_aligned + pagesize;

	// pid
	printf("process id %i\n", getpid());

	printf("primul apel: foo intoarce %d\n", foo());

	// schimbare protectie pagina
	if (mprotect(addr_aligned, pagesize, PROT_READ|PROT_WRITE|PROT_EXEC)) {
		perror("mprotect");
		return 1;
	}

	// schimbare valoare de intors functie
	f = (int (*)())foo;

	while ((int)f < (int)page_end &&
		((int)*f != 0xb8 && (char)*((char *)f + 1) != 0x0c))
		f++;

	crt = (char *)(++f);
	printf("f = %p\t*bla = %hhx\n", f, *crt);
	*crt = 0xff;

	printf("al doilea apel: foo intoarce %d\n", foo());

	return 0;
}

