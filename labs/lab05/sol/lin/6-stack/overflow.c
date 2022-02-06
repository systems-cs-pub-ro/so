/**
  * SO, 2016
  * Lab #5
  *
  * Task #6, lin
  *
  * Stack smashing
  */
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#if defined(__i386__)
#define get_esp(esp) __asm__("movl %%esp, %0;":"=&r"(esp))
#define get_ebp(ebp) __asm__("movl %%ebp, %0;":"=&r"(ebp))

typedef int mytype;
int pos = 7;

#elif defined(__x86_64)
#define get_esp(esp) __asm__("mov %%rsp, %0;":"=&r"(esp))
#define get_ebp(ebp) __asm__("mov %%rbp, %0;":"=&r"(ebp))

typedef long mytype;
int pos = 5;

#else
  #error "Unknown architecture"
#endif

int i, size;
mytype *esp, *ebp;
mytype *p;


struct stack_element {
	mytype *address;
	mytype value;
};

/*
 * stack vector represents the program stack
 * each element contains an address from the stack and the actual value
 * at that address
 */
struct stack_element stack[128];

/**
 * show_snapshot - prints the stack in the following format:
 * [address: value]
 */
void show_snapshot(void)
{
	for (i = 0; i < size; i++)
		printf("[%p]:0x%lx\n", stack[i].address, stack[i].value);
}

/**
 * save the stack between ebp and esp in the stack vector
 */
void take_snapshot(void)
{
	for (p = ebp; p >= esp; p--) {
		stack[size].address = p;
		stack[size].value = *p;
		size++;
	}
}

void show_message(void)
{
	printf("s0rU1lz\n");
	exit(EXIT_FAILURE);
}

void f2(mytype a)
{
	mytype v[3];

	v[0] = 6;
	v[1] = 7;
	v[2] = 8;

	/* TODO2: use v so that when function 'f2' returns,
	 * the 'show_message' function executes
	 */
	v[pos] = (mytype)show_message;

	/* save current stack pointer */
	get_esp(esp);

	/* use previously saved esp and epb pointers */
	take_snapshot();
}

static void f1()
{
	mytype v[3] = {3, 4 ,5};
	f2(0xdeadbeef);
}

int main(void)
{
	mytype v[3] = {1, 2 ,3};

	/* save current ebp for later use */
	get_ebp(ebp);

	f1();

	/* analize the stack */
	show_snapshot();

	return 0;
}
