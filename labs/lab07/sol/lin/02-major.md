We are going to monitor `L1-dcache-load` and `L1-dcache-load-misses`.
We can observe that the number of `dcache` misses is much larger
when we iterate over columns first.

```
$ perf stat -e L1-dcache-load-misses ./row
```

Performance counter stats for `./row`:

| 415.973 | L1-dcache-load-misses |
|---|---|
| 0,144622930 | seconds time elapsed |


```
$ perf stat -e L1-dcache-load-misses ./column
```

Performance counter stats for `./column`:

| 18.051.604 | L1-dcache-load-misses |
|---|---|
| 0,482737143 | seconds time elapsed |
