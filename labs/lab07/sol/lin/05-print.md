`make print` is working even without an explicit Makefile because it is
a default make rule.

The program output is `BA` because printing at the stdout is done using a 
buffer, but printing at stderr does not make use of a buffer.
