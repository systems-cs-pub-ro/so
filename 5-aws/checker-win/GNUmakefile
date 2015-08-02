.PHONY: all clean run pack build-pre build-post

all: run

build-pre:

build-post:

run:
	./run_all.sh

pack:
	zip -r run_test_win.zip _test/ GNUmakefile NMakefile.checker \
		run_all.sh README nc.exe

clean:
	-rm -f *~
