CFLAGS = /nologo /W3 /EHsc

objs = run_test.obj test_sched.obj test_exec.obj test_io.obj

all: $(objs)

run_test.obj: run_test.c
test_sched.obj: test_sched.c
test_exec.obj: test_exec.c
test_io.obj: test_io.c

clean:
	del /Q /S $(objs) run_test.exe *~

