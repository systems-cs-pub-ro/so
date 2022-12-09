#ifndef SERVER_H
#define SERVER_H

/* lambda function without parameter */
typedef void (*lambda_func_t)(void);

/* lambda function with parameter */
typedef void (*lambda_param_func_t)(const char *);

struct lib {
	/* Name of the output file where a function call's output is stored */
	char *outputfile;
	/* Name of the library and function to be called */
	char *libname;
	char *funcname;
	/* Name of the filename that is passed as input for the function */
	char *filename;

	/* Handle for the loaded dynamic library */
	void *handle;

	/* Pointer to a function without arguments */
	lambda_func_t run;
	/* Pointer to a function with a filename as an argument */
	lambda_param_func_t p_run;
};

#endif /* SERVER_H */
