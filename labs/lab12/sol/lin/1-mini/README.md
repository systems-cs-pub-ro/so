# Additional Hints

## Task 1
* `ls -l /dev` will list all node devices.
* Check the snippet below:

```
brw-rw---T 1 root disk 8, 1 May 14 19:50 /dev/sda1
b - block device, 8 - major, 1 - minor
```

## Task 3
* Include `<sys/mount.h>`

## Task 6
* `man opendir`
* Include `<sys/types.h>` and `<dirent.h>`
