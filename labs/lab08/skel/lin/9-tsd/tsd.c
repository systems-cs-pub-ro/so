/**
 * SO, 2019
 * Lab #8
 *
 * Task #9, lin
 *
 * Thread Specific Data
 */
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>

/*
 * Se poate observa ca thread_function nu trebuie sa inchida fisierele de log.
 *
 * Aceasta se intampla din cauza faptului ca atunci cand
 * thread_log_key e creata se specifica si o functie de cleanup pentru
 * aceasta.
 *
 * Atunci cand un thread se termina, se va apela acea functie si se va
 * pasa ca parametru valoarea specifica threadului pentru
 * thread_log_key.
 *
 * Functia de cleanup se ocupa de inchiderea fisierului de log.
 */

#define MAX_THREADS 5

/* cheia folosita pentru a asocia pointerul fisierului de log cu threadul */
static pthread_key_t  thread_log_key;

/* scrie message in fisierul de log al threadului curent */
void write_to_thread_log(const char *message)
{
	FILE *thread_log = (FILE *) pthread_getspecific(thread_log_key);

	fprintf(thread_log, "%s\n", message);
}

/* inchide fisierul de log */
void close_thread_log(void *thread_log)
{
	fclose((FILE *) thread_log);
}

/* functia care va fi executata de thread-uri */
void *thread_function(void *args)
{
	char thread_log_filename[200];
	FILE *thread_log;

	/* genereaza filename pentru fisierul de log */
	sprintf(thread_log_filename, "thread%d.log", (int) pthread_self());

	/* deschide fisierul de log */
	thread_log = fopen(thread_log_filename, "w");

	/* stocheaza pointerul in thread-specific data ca thread_log_key */
	pthread_setspecific(thread_log_key, thread_log);

	write_to_thread_log("Thread starting.");

	/* Do some work ... */

	return NULL;
}

int main(void)
{
	int i;
	pthread_t threads[MAX_THREADS];

	/* creeaza in thread-specific data o cheie
	 * care sa asocieze un thread cu pointerul catre fisierul lui de log
	 * si care foloseste close_thread_log ca functie de clean up
	 */
	pthread_key_create(&thread_log_key, close_thread_log);

	/* creeaza thread-urile ce vor executa thread_function */
	for (i = 0; i < MAX_THREADS; ++i)
		pthread_create(&(threads[i]), NULL, thread_function, NULL);

	/* asteptarea terminarii thread-urilor */
	for (i = 0; i < MAX_THREADS; ++i)
		pthread_join(threads[i], NULL);

	/* stergerea cheii */
	pthread_key_delete(thread_log_key);

	return 0;
}
