# 02-debug

Trebuie definit simbolul `DEBUG__`, folosind optiunea `/D` a compilatorului `cl`.
Putem redefini variabila `CFLAGS` astfel:
```
CFLAGS = /nologo /W3 /DDEBUG__
```

Specificarea numelui de fisier makefile se face folosind parametrul `/F` al comenzii `nmake`, de exemplu: `nmake /F Makefile.dbg`.
