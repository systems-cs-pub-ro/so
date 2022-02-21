```
$ valgrind --tool=memcheck ./flowers

==10343== Invalid read of size 1
==10343==    at 0x4C29502: __GI_strlen (mc_replace_strmem.c:391)
==10343==    by 0x4E9907A: puts (ioputs.c:37)
==10343==    by 0x4005A4: main (flowers.c:16)
==10343==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==10343==
```

We have 6 types of flowers:

```
const char *flowers[] = {
        "rose", "tulip", "daisy"
        "petunia", "orchid", "lily"
};
```

The following snippet of code contains a logic bug:
```
choice = rand() % 6;
printf("%s\n", flowers[choice]);
```

`choice` can take 5 as a value and it will cause an array index out
of bounds because `flowers` has the length equal to 5.
