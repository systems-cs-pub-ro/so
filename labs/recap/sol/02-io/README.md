# 02: IO

Apelul [`splice`](https://man7.org/linux/man-pages/man2/splice.2.html) implementează un mecanism de tipy `ZeroCopy`; el copiază dintr-un pipe într-un file descriptor.

Datorită existenţei unui cache în sistemul de operare în care sunt menţinute rezultatele operaţiilor IO **(pagecache)**, o a doua accesare a fişierului se va face mult mai rapid, datele servindu-se din acest cache.

La trunchierea fişierului se lucrează cu o structură ce este menţinută în memorie **(inode)**. Deci diferenţele nu vor fi foarte mari.

Dacă folosim un fişier care este deja în memorie (cum ar fi `/dev/zero`), se va vedea o diferenţă notabilă doar la trunchierea fişierului.
