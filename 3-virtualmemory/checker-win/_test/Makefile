#CPPFLAGS = /DDEBUG /DLOG_LEVEL=LOG_DEBUG
CFLAGS = /nologo /W3 /EHsc $(CPPFLAGS)

all: run_test.obj vmsim_test_win.obj common_win.obj vmsim_test.obj

run_test.obj: run_test.c run_test.h vmsim_test.h

vmsim_test_win.obj: vmsim_test_win.c common.h

common_win.obj: common_win.c common.h util.h debug.h

vmsim_test.obj: vmsim_test.c run_test.h vmsim.h vmsim_aux.h debug.h

clean:
	del /Q /S *.obj run_test.exe
