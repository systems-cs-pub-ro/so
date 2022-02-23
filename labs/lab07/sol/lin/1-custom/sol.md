There are two possible solutions:

 1. Use bash comands to count (1) the number of PC (program counter) values
 within the program and (2) the number of PC values only from `hash_search_index`

```
perf script -f ip,sym | grep -v '#' | wc -l 
perf script -f ip,sym | grep -v '#' | grep hash_search_index | wc -l
```

Divide the two values and compare the result with the output from `perf report`. 

2. By using the `Python` script given.

Check if `sample['ip']` is between 0x080488d9 and 0x8048985. The address 
can be found using `objdump` - the first and the last address from `hash_search_index`.

```
if (sample['ip'] <= 0x8048985) and (sample['ip'] >= 0x080488d9):
    hash_search_index_events += 1
</code>
```

Check if `perf report` is showing the same percentage as the `Python` script.
