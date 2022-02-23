After running `perf record` and `perf report`, we can see that `strlen` is called
multiple times for the same string. An optimization will be
to compute the string length only once outside the loop 

```
perf record -e cycles:pp -c 10000 -d ./find-char
```

Output example:

99,44%  find-char  libc-2.19.so       [.] strlen
