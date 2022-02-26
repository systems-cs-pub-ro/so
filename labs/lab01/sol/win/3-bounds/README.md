# 03-bounds

## Biblioteca statica

- Obtinem fisierele obiect `min.obj`, `max.obj` si `bounds.obj` folosind optiunea `/c` a compilatorului `cl`.
- Cream biblioteca statica folosind comanda `lib`.
- Legam fisierul obiect de biblioteca statica folosind comanda `link`.
- Nu este nevoie sa folosim directive `__declspec`.

## Biblioteca dinamica
 
- Exportam, respectiv importam explicit functiile folosite:
    - `bounds.c`: `__declspec(dllimport) int get_min(int a, int b);`
    - `min.c`: `__declspec(dllexport) int get_min(int a, int b);`
    - la fel si pentru functia `get_max`
- Cream fisierele obiect la fel ca pentru biblioteca statica.
- Cream biblioteca dinamica si cea de import cu optiunea `/dll` a comenzii `link`
