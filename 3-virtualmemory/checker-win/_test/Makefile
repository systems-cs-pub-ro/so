#CPPFLAGS = /DDEBUG /DLOG_LEVEL=LOG_DEBUG
CFLAGS = /nologo /W3 /EHsc $(CPPFLAGS)

all: vmsim_test.obj vmsim_test_win.obj run_test.obj common_win.obj

run_test.obj: run_test.c

vmsim_test.obj: vmsim_test.c

vmsim_test_win.obj: vmsim_test_win.c

common_win.obj: common_win.c common.h

clean:
	del /Q /S *.obj run_test.exe
