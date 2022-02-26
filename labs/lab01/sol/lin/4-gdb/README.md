# 04-gdb

Adresele afisate mai jos vor diferi intre rulari succesive.
```
student@vagrant:~/so/labs/lab01/skel/lin/04-gdb$ gdb fault
(gdb) run
[...]
Program received signal SIGSEGV, Segmentation fault.
print_array (v=0x7fffffffdbf0, n=5) at fault.c:19
19                      printf("%d ", v[i]);
(gdb) bt
#0  print_array (v=0x7fffffffdbf0, n=5) at fault.c:19
#1  0x000055555555529e in show_inv (v=0x7fffffffdbf0, n=5) at fault.c:38
#2  0x0000555555555314 in main () at fault.c:47
(gdb) print i
$1 = 816
(gdb) print v[i]
Cannot access memory at address 0x7ffffffff000

Observam ca se incearca accesarea unei zone de memorie nealocata.
De ce?
Pentru ca vectorul `v` are doar 5 elemente, dar se incearca printarea elementului cu indexul 816.
De ce nu am primit eroarea mai devreme?
Desi vectorul v are 5 elemente alocate, eroarea se primeste doar la accesarea celui de-al 816-lea element?
Raspunsul este in [laboratorul de memorie virtuala](https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-06).
```

Acum corectam eroarea.
In functia `print_array`, inlocuim
```c
for(i = 0; i < ARRAY_MAX_SIZE; i++)
```
cu
```c
for(i = 0; i < n; i++)
```

Incercam din nou sa rulam programul:
```
student@vagrant:~/so/labs/lab01/skel/lin/04-gdb$ gdb fault
(gdb) run
Program received signal SIGFPE, Arithmetic exception.
0x0000555555555252 in inv_sum (v=0x7fffffffdbf0, n=5) at fault.c:30
30                      isum += 1 / v[i];
(gdb) print v[i]
$1 = 0
```

De data aceasta, problema apare din cauza unei impartiri la 0.
Corectam `for`-ul din functia `show_inv`:
```c
for (i = 0; i < n; i++) {
	if (!v[i])
		continue;

	isum += 1 / v[i];
}
```

Programul ruleaza fara erori acum, insa rezultatul aparent este 0, intrucat toate operatiile sunt cu numere intregi :).
Tineti minte: `1/1000 == 0`, pe cand `1/1000.0 == 0.001`.
