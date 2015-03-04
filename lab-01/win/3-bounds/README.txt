#README, 03-bounds


#1, Creare biblioteca statica

 - Se obtin fisierele obiect min.obj, max.obj si bounds.obj 
folosind optiunea /c a compilatorului cl.
 - Se obtine biblioteca statica folosind comanda lib.
 - Se leaga fisierul obiect de biblioteca statica folosind comanda link.
 - Nu se folosesc directive __declspec

#2, Creare biblioteca dinamica
 
 - Exportarea/Importarea explicita a functiilor folosite
   - bounds.c: __declspec(dllimport) int get_min(int a, int b);
   - min.c:    __declspec(dllexport) int get_min(int a, int b);
   - la fel si pentru functia get_max
 - Crearea fisierelor obiect la fel ca in #1
 - Crearea bibliotecii dinamice si a biblioteci de import cu optiunea /dll
 pentru comanda link
 
