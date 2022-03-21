# Recap

## 05. Initializations
* The first initialisation takes longer because the pages form the `.data` section that correspond to the `data1` array need to be mapped.
This causes more reads from the disk.

* The size of the executable is due to the `data1` array.
It is placed in the `.data` section, which is present in the executable on the disk.

## 08. Stack Growth
* The `f1` function calls `f2`.
Both of them allocate one variable on their stacks.
Thus, the variable declared inside the stack frame of `f2` is "higher" on the stack than the one declared by `f1`.
By comparing the 2 memory addresses, we see that the stack "grows downwards".

* However, the C standard does not require the existence of a stack, so the question regards an application of the language in operating systems and modern computer architectures.

## 09. Syscalls
* According to the [`getpid(2)`](https://man7.org/linux/man-pages/man2/getpid.2.html) man page, the `glibc` wrapper stores the value of the `pid` in a cache.
This is a natural optimisation, because the `pid` of a process is assigned on creation and doesn't change throughout its existence.

* As noted in `fork_test.c`, this caching mechanism is "dangerous" when interleaving `glibc` calls and "direct" syscalls.
When `SYS_TYPE` is `SYS_NATIVE`, we can see that the child displays an incorrect `pid`.

## 10. `fork`
The program creates 8 processes.
Because the address space is initially shared between parent and child processes, we can see that the address of `ii` is the same in all processes.

## 11. File descriptors
The file descriptors 0, 1 and to are dedicated to `stdin`, `stdout` and `stderr`.
`pipe` is called every 17th iteration.
Afterwards, the writing end of the pipe is closed and the reading one remains open.
This means that until the 964th iteration of the `while` there are `964 / 17 + 1 = 57` open reading pipe ends.
A process can open at most 1024 files at the same time.
`1024 - 3 - 57 = 964`, which explains why the 964th `open` call fails.

## 14. `malloc` Syscalls
* `libc` preallocates memory on the heap by calling `brk` and multiple calls to `malloc` result in a single call to `brk`.
Therefore, the number of calls to `brk` is sensibly lower than the number of calls to `malloc`.

* The same holds true for the 1000 `free` calls, which result in a signle call to `brk`.

## 15. `fork` faults
* After the first `for` finishes (before the call to `fork`): about `NUM_PAGES` faults occur.
The memory is allocated using demand-paging and every access to a memory page leads to a page fault and the allocation of a new physical page (frame).

* During the first `for` loop in the child process there are no page faults.
The pages are mapped as read-only, (with the copy-on-write bit set) and are only accessed for reading.

* The second `for` loop causes `NUM_PAGES / 2` page faults because the pages are now accessed for writing.

* Following the end of the child process, the parent can see the child's page faults.

* The last `for` loop generates `NUM_PAGES` page faults.
The pages in the `p` array are now remapped as a result of these page faults.
