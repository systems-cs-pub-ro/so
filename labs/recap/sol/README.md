# Recap

## 05. Initializations
* Prima inițializare durează mai mult pentru că trebuie să fie mapate valorile inițiale din executabil, deci se fac accese la disc.

* Pentru că vectorul data1 este inițializat, el este memorat în fișierul executabil; astfel se explică dimensiunea fișierului.

## 08. Stack Growth
* Funcția `f1` apelează `f2`. Ambele conțin o variabilă pe stivă.
Variabila declarată în `f2` se află, în acest fel, mai "sus" în stivă decât cea din `f1`.
Comparând cele două adrese de memorie putem decide în ce sens crește stiva.

* Totuși, standardul C nu specifică existența unei stive, deci întrebarea vizează o aplicare a limbajului la sisteme de operare și arhitecturi moderne.

## 09. Syscalls
* Conform paginii de manual [`getpid(2)`](https://man7.org/linux/man-pages/man2/getpid.2.html) wrapperul din glibc foloseşte un cache pentru a reţine valoarea `pid`-ului.
Acest lucru este "natural" întrucât `pid`-ul unui proces este asignat la creare şi nu se schimbă în decursul existenţei procesului.

* După cum observăm din exemplul `fork_test.c` acest caching este "periculos" dacă se folosesc funcţii glibc intercalate cu apeluri "directe".
În exemplul `fork_test.c` în cazul in care `SYS_TYPE` are valoarea `SYS_NATIVE` se observă că în copil `pid`-ul este afişat greşit. 

## 10. `fork`
Se crează 8 procese.
Întrucât spaţiul de adrese este iniţial partajat observăm că adresa variabilei `ii` este aceeaşi în toate procesele.
Valorile variabilei `ii` se deduc uşor din rularea programului.

## 11. File descriptors
File descriptorii 0, 1 și 2 sunt rezervați pentru `stdin`, `stdout` și `stderr`.
La fiecare a 17-a iterație se apelează `pipe`, după care este închis capătul de scriere (cel de citire rămâne deschis).
Acest lucru înseamnă că până la a 964-a iterație a `while`-ului au rămas deschise `964 / 17 + 1 = 57` capete de citire.
`1024 - 3 - 57 = 964`, ceea ce explică de ce eșuează deschiderea fișierului la a 964-a iterație.

## 14. `malloc` Syscalls
* Biblioteca standard C face prealocare folosind `brk`.
În acest fel, mai multe apeluri de bibliotecă `malloc` vor rezulta într-un singur apel `brk`.
Din acest motiv, numărul de apeluri de sistem `brk` este vizibil mai mic față de numărul de apeluri de biblioteca `malloc`.

* Cele 1000 de apeluri de bibliotecă `free`, rezultă într-un singur apel `brk`.
Apelul `brk` este folosit pentru modificarea dimensiunii heapului.
Un apel `free` (sau un set de apeluri) va rezulta în repoziționarea finalului zonei de heap.

## 15. `fork` faults
* După primul ciclu `for` (înainte de `fork`): se obțin circa `NUM_PAGES` faulturi.
Memoria a fost alocată folosind demand-paging, iar fiecare acces la o pagină de memorie va conduce la apariția unui page fault și alocarea unei pagini fizice.

* După primul ciclu for din procesul copil, nu se obține niciun page fault.
Paginile sunt mapate read-only (copy-on-write) și sunt accesate doar pentru citire.
Nu este alterat conținutul și nu apar page faulturi.

* După al doilea ciclu `for` din procesul copil apar `NUM_PAGES / 2` faulturi.
Paginile sunt mapate read-only, iar fiecare acces va genera un page fault (copy-on-write) și maparea paginii în forma read-write.

* După încheierea procesului copil, în procesul părinte sunt vizibile page faulturile apărute.

* În urma ultimului ciclu `for` (procesul părinte), se generează `NUM_PAGES` page faulturi.
Paginile mapate inițial read-only sunt remapate read-write în urma page faulturilor.
