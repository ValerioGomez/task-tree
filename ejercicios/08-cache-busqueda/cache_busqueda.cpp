// ============================================================================
// EJERCICIO 08: CACHÉ DE BÚSQUEDA (MEMOIZACIÓN EN ÁRBOLES)
// Archivo: ejercicios/08-cache-busqueda/cache_busqueda.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic cache_busqueda.cpp -o cache_busqueda
// ============================================================================
//
// PROBLEMA:
//   Una búsqueda normal en BST recorre los nodos desde la raíz cada vez.
//   Si buscamos el mismo valor 1000 veces, hacemos 1000 * O(log n) comparaciones.
//
// SOLUCIÓN — CACHÉ DE BÚSQUEDA:
//   Guardar el resultado de cada búsqueda en una tabla de acceso rápido.
//   La próxima vez que se busque el mismo valor → respuesta INMEDIATA (O(1)).
//
// CONCEPTO:
//   Esto se llama MEMOIZACIÓN (del inglés "memoization"):
//   guardar resultados de operaciones costosas para reutilizarlos.
//   Es la base de la Programación Dinámica y de los sistemas de caché
//   en bases de datos y sistemas operativos.
//
// IMPLEMENTACIONES en este archivo:
//
//   1. TABLA HASH SIMPLE — caché con array de acceso O(1)
//      (implementada manualmente en C++98)
//
//   2. CACHÉ CON std::map — O(log n) pero simple y robusta
//      (usando la STL disponible en C++98)
//
//   3. CACHÉ LRU (Least Recently Used) — descarta la entrada más antigua
//      cuando el caché está lleno
//      (simula el comportamiento de cachés reales de bases de datos)
//
//   4. BST CON CACHÉ INTEGRADO — el árbol recuerda automáticamente
//      cada búsqueda realizada
//
// ============================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>       // std::map — disponible en C++98
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// ESTRUCTURA DEL ÁRBOL BST
// ─────────────────────────────────────────────────────────────────────────────
struct NodoArbol {
    int dato;
    struct NodoArbol *padre;
    struct NodoArbol *izquierdo;
    struct NodoArbol *derecho;
};
typedef struct NodoArbol *NodoPtr;

NodoPtr crearArbol(int x) {
    NodoPtr p = (NodoPtr)malloc(sizeof(struct NodoArbol));
    p->dato = x; p->padre = p->izquierdo = p->derecho = NULL;
    return p;
}
NodoPtr insertar(NodoPtr p, int x) {
    if (!p) return crearArbol(x);
    if (x < p->dato) { NodoPtr iz = insertar(p->izquierdo,  x); p->izquierdo  = iz; iz->padre = p; }
    else             { NodoPtr dr = insertar(p->derecho, x); p->derecho = dr; dr->padre = p; }
    return p;
}
NodoPtr buscarNodo(NodoPtr p, int x) {
    if (!p) return NULL;
    if (x == p->dato) return p;
    if (x < p->dato) return buscarNodo(p->izquierdo,  x);
    else             return buscarNodo(p->derecho, x);
}
void liberarArbol(NodoPtr p) {
    if (!p) return;
    liberarArbol(p->izquierdo); liberarArbol(p->derecho); free(p);
}

// ============================================================================
//  1. TABLA HASH SIMPLE — caché O(1) implementada con array
// ============================================================================
//
//  Funciona con una función hash: hash(valor) = valor % TABLA_SIZE
//  Colisiones resueltas por encadenamiento (lista enlazada en cada celda)
//
// ============================================================================
#define TABLA_SIZE 64   // tamaño de la tabla hash

struct EntradaHash {
    int       clave;           // valor buscado
    NodoPtr  nodo;            // puntero al nodo encontrado (NULL si no existe)
    bool      existe;          // ¿la clave existe en el árbol?
    bool      ocupado;         // ¿esta celda tiene un valor?
    struct EntradaHash *siguiente;  // para colisiones
};
typedef struct EntradaHash *EntradaPtr;

struct TablaHash {
    EntradaPtr celdas[TABLA_SIZE];
    int total_busquedas;
    int aciertos_cache;
    int fallos_cache;
};

void inicializarTabla(TablaHash &t) {
    for (int i = 0; i < TABLA_SIZE; i++) t.celdas[i] = NULL;
    t.total_busquedas = t.aciertos_cache = t.fallos_cache = 0;
}

int funcionHash(int clave) {
    int h = clave % TABLA_SIZE;
    return h < 0 ? -h : h;  // asegurar positivo
}

// Guardar en caché
void guardarCache(TablaHash &t, int clave, NodoPtr nodo, bool existe) {
    int idx = funcionHash(clave);
    // Verificar si ya existe
    EntradaPtr e = t.celdas[idx];
    while (e != NULL) {
        if (e->clave == clave) { e->nodo = nodo; e->existe = existe; return; }
        e = e->siguiente;
    }
    // Crear nueva entrada
    EntradaPtr nueva = (EntradaPtr)malloc(sizeof(struct EntradaHash));
    nueva->clave     = clave;
    nueva->nodo      = nodo;
    nueva->existe    = existe;
    nueva->ocupado   = true;
    nueva->siguiente = t.celdas[idx];   // encadenar
    t.celdas[idx]    = nueva;
}

// Buscar en caché: retorna true si está en caché, escribe en 'resultado'
bool obtenerCache(TablaHash &t, int clave, NodoPtr &resultado, bool &existe) {
    int idx = funcionHash(clave);
    EntradaPtr e = t.celdas[idx];
    while (e != NULL) {
        if (e->clave == clave) {
            resultado = e->nodo;
            existe    = e->existe;
            return true;   // ¡HIT! encontrado en caché
        }
        e = e->siguiente;
    }
    return false;   // MISS: no está en caché
}

void liberarTabla(TablaHash &t) {
    for (int i = 0; i < TABLA_SIZE; i++) {
        EntradaPtr e = t.celdas[i];
        while (e) { EntradaPtr sig = e->siguiente; free(e); e = sig; }
        t.celdas[i] = NULL;
    }
}

// Búsqueda con caché hash
NodoPtr buscarConCacheHash(NodoPtr arbol, TablaHash &cache, int x) {
    cache.total_busquedas++;
    NodoPtr resultado; bool existe;

    if (obtenerCache(cache, x, resultado, existe)) {
        cache.aciertos_cache++;
        cout << "  [CACHE HIT ] buscar(" << x << ") → directo, sin recorrer arbol" << endl;
        return existe ? resultado : NULL;
    }

    // MISS: hacer la búsqueda real en el árbol
    cache.fallos_cache++;
    NodoPtr nodo = buscarNodo(arbol, x);
    guardarCache(cache, x, nodo, nodo != NULL);
    cout << "  [CACHE MISS] buscar(" << x << ") → recorrio el arbol, guardado en cache" << endl;
    return nodo;
}

// ============================================================================
//  2. CACHÉ CON std::map (C++98 STL)
// ============================================================================
//
//  std::map<int, NodoPtr> guarda pares (clave → puntero)
//  Lookup en O(log n) — más lento que hash pero más simple
//  Usa un árbol rojo-negro internamente (¡es un árbol adentro de otro árbol!)
//
// ============================================================================
struct CacheMap {
    map<int, NodoPtr> tabla;
    map<int, bool>     existe_en_arbol;
    int hits;
    int misses;
    CacheMap() : hits(0), misses(0) {}
};

NodoPtr buscarConCacheMap(NodoPtr arbol, CacheMap &cache, int x) {
    // Buscar en caché
    map<int, bool>::iterator it = cache.existe_en_arbol.find(x);
    if (it != cache.existe_en_arbol.end()) {
        cache.hits++;
        cout << "  [MAP HIT ] buscar(" << x << ") → directo desde std::map" << endl;
        return it->second ? cache.tabla[x] : NULL;
    }
    // Búsqueda real
    cache.misses++;
    NodoPtr nodo = buscarNodo(arbol, x);
    cache.tabla[x]          = nodo;
    cache.existe_en_arbol[x] = (nodo != NULL);
    cout << "  [MAP MISS] buscar(" << x << ") → recorrio el arbol, guardado en map" << endl;
    return nodo;
}

// ============================================================================
//  3. CACHÉ LRU (Least Recently Used)
// ============================================================================
//
//  Política: cuando el caché está lleno, descarta la entrada que hace MÁS
//  tiempo que no se usa (la menos recientemente usada).
//
//  Estructura: lista doblemente enlazada + tabla hash
//    - La lista mantiene el orden de uso (frente = más reciente)
//    - El hash permite acceso O(1) a cualquier entrada
//
// ============================================================================
#define LRU_CAPACIDAD 4   // caché pequeño para ver los descartes

struct NodoLRU {
    int       clave;
    NodoPtr  valor;
    bool      existe;
    struct NodoLRU *anterior;
    struct NodoLRU *siguiente;
};

struct CacheLRU {
    NodoLRU *cabeza;    // más recientemente usado
    NodoLRU *cola;      // menos recientemente usado (candidato a descartar)
    int      tamano;
    int      capacidad;
    int      hits;
    int      misses;
    int      descartes;
};

void iniciarLRU(CacheLRU &lru, int cap) {
    lru.cabeza = lru.cola = NULL;
    lru.tamano = 0;
    lru.capacidad = cap;
    lru.hits = lru.misses = lru.descartes = 0;
}

NodoLRU *buscarEnLRU(CacheLRU &lru, int clave) {
    NodoLRU *n = lru.cabeza;
    while (n != NULL) {
        if (n->clave == clave) return n;
        n = n->siguiente;
    }
    return NULL;
}

void moverAlFrente(CacheLRU &lru, NodoLRU *n) {
    if (n == lru.cabeza) return;
    // Desenlazar
    if (n->anterior) n->anterior->siguiente = n->siguiente;
    if (n->siguiente) n->siguiente->anterior = n->anterior;
    if (n == lru.cola) lru.cola = n->anterior;
    // Poner al frente
    n->siguiente = lru.cabeza;
    n->anterior  = NULL;
    if (lru.cabeza) lru.cabeza->anterior = n;
    lru.cabeza = n;
    if (!lru.cola) lru.cola = n;
}

void insertarEnLRU(CacheLRU &lru, int clave, NodoPtr valor, bool existe) {
    if (lru.tamano == lru.capacidad) {
        // Descartar la entrada de la COLA (menos usada)
        NodoLRU *viejo = lru.cola;
        cout << "    [LRU] Descartando clave " << viejo->clave
             << " (la menos usada)" << endl;
        if (viejo->anterior) viejo->anterior->siguiente = NULL;
        lru.cola = viejo->anterior;
        if (lru.cola == NULL) lru.cabeza = NULL;
        free(viejo);
        lru.tamano--;
        lru.descartes++;
    }
    NodoLRU *nuevo = (NodoLRU*)malloc(sizeof(NodoLRU));
    nuevo->clave     = clave;
    nuevo->valor     = valor;
    nuevo->existe    = existe;
    nuevo->anterior  = NULL;
    nuevo->siguiente = lru.cabeza;
    if (lru.cabeza) lru.cabeza->anterior = nuevo;
    lru.cabeza = nuevo;
    if (!lru.cola) lru.cola = nuevo;
    lru.tamano++;
}

NodoPtr buscarConLRU(NodoPtr arbol, CacheLRU &lru, int x) {
    NodoLRU *entrada = buscarEnLRU(lru, x);
    if (entrada != NULL) {
        lru.hits++;
        moverAlFrente(lru, entrada);  // actualizar orden LRU
        cout << "  [LRU HIT ] buscar(" << x << ") → en cache (tam=" << lru.tamano << ")" << endl;
        return entrada->existe ? entrada->valor : NULL;
    }
    lru.misses++;
    NodoPtr nodo = buscarNodo(arbol, x);
    insertarEnLRU(lru, x, nodo, nodo != NULL);
    cout << "  [LRU MISS] buscar(" << x << ") → busco en arbol, cache ahora tiene "
         << lru.tamano << " entradas" << endl;
    return nodo;
}

void liberarLRU(CacheLRU &lru) {
    NodoLRU *n = lru.cabeza;
    while (n) { NodoLRU *sig = n->siguiente; free(n); n = sig; }
    lru.cabeza = lru.cola = NULL;
}

// ============================================================================
//  MAIN
// ============================================================================
int main() {
    cout << "============================================" << endl;
    cout << "  EJERCICIO 08: CACHE DE BUSQUEDA" << endl;
    cout << "  (Memoizacion en Arboles BST)" << endl;
    cout << "============================================" << endl;

    // ─── Construir BST de prueba ──────────────────────────────────────────
    int datos[] = {15, 4, 8, 7, 3, 19, 5, 9, 16, 17, 20, 25, 1, 30};
    int nd = 14;
    NodoPtr arbol = NULL;
    for (int i = 0; i < nd; i++) arbol = insertar(arbol, datos[i]);
    cout << "\nArbol BST construido con " << nd << " nodos." << endl;
    cout << "Valores: 1,3,4,5,7,8,9,15,16,17,19,20,25,30\n" << endl;

    // ─── DEMO 1: Caché Hash ────────────────────────────────────────────────
    cout << "═══════════════════════════════════" << endl;
    cout << "  DEMO 1: CACHE CON TABLA HASH" << endl;
    cout << "═══════════════════════════════════" << endl;
    cout << "\n(Primera ronda — todas son MISS)" << endl;
    TablaHash cache1;
    inicializarTabla(cache1);

    int busquedas[] = {7, 19, 100, 7, 15, 19, 7, 42, 15, 7};
    int nb = 10;
    for (int i = 0; i < nb; i++) {
        NodoPtr r = buscarConCacheHash(arbol, cache1, busquedas[i]);
        cout << "     → " << busquedas[i]
             << (r ? " ENCONTRADO" : " NO existe en arbol") << endl;
    }
    cout << "\nEstadisticas de la tabla hash:" << endl;
    cout << "  Total busquedas : " << cache1.total_busquedas << endl;
    cout << "  Cache HITS      : " << cache1.aciertos_cache
         << " (sin recorrer el arbol)" << endl;
    cout << "  Cache MISSES    : " << cache1.fallos_cache
         << " (recorrio el arbol)" << endl;
    int ahorro = cache1.aciertos_cache * 100 / cache1.total_busquedas;
    cout << "  Ahorro          : " << ahorro << "% de busquedas evitadas" << endl;
    liberarTabla(cache1);

    // ─── DEMO 2: Caché std::map ───────────────────────────────────────────
    cout << "\n═══════════════════════════════════" << endl;
    cout << "  DEMO 2: CACHE CON std::map (C++98 STL)" << endl;
    cout << "═══════════════════════════════════" << endl;
    CacheMap cache2;
    for (int i = 0; i < nb; i++) {
        NodoPtr r = buscarConCacheMap(arbol, cache2, busquedas[i]);
        cout << "     → " << busquedas[i]
             << (r ? " ENCONTRADO" : " NO existe") << endl;
    }
    cout << "\nEstadisticas del std::map:" << endl;
    cout << "  HITS  : " << cache2.hits  << endl;
    cout << "  MISSES: " << cache2.misses << endl;

    // ─── DEMO 3: Caché LRU ────────────────────────────────────────────────
    cout << "\n═══════════════════════════════════" << endl;
    cout << "  DEMO 3: CACHE LRU (capacidad = " << LRU_CAPACIDAD << " entradas)" << endl;
    cout << "═══════════════════════════════════" << endl;
    cout << "(Cuando esta lleno, descarta la entrada menos usada)" << endl << endl;

    CacheLRU lru;
    iniciarLRU(lru, LRU_CAPACIDAD);

    // Secuencia que forzará descartes
    int secLRU[] = {7, 19, 15, 4, 8, 7, 19, 30, 7, 15, 4, 8, 30};
    int nlru = 13;
    for (int i = 0; i < nlru; i++) {
        NodoPtr r = buscarConLRU(arbol, lru, secLRU[i]);
        cout << "     → " << secLRU[i]
             << (r ? " ENCONTRADO" : " NO existe") << endl;
    }
    cout << "\nEstadisticas del LRU (capacidad=" << LRU_CAPACIDAD << "):" << endl;
    cout << "  HITS      : " << lru.hits      << endl;
    cout << "  MISSES    : " << lru.misses    << endl;
    cout << "  Descartes : " << lru.descartes << " (entradas expulsadas)" << endl;
    liberarLRU(lru);

    // ─── Resumen conceptual ───────────────────────────────────────────────
    cout << "\n═══════════════════════════════════" << endl;
    cout << "  RESUMEN: TIPOS DE CACHE" << endl;
    cout << "═══════════════════════════════════" << endl;
    cout << "\n  Tipo         | Lookup | Politica de descarte | Uso tipico" << endl;
    cout << "  -------------|--------|----------------------|----------------" << endl;
    cout << "  Hash simple  | O(1)   | Ninguna (ilimitado)  | Memoizacion" << endl;
    cout << "  std::map     | O(logN)| Ninguna (ilimitado)  | Prototipado" << endl;
    cout << "  LRU          | O(N)   | Menos reciente       | Bases de datos" << endl;
    cout << "  LFU          | O(1)   | Menos frecuente      | Sistemas op." << endl;
    cout << "  MRU          | O(N)   | Mas reciente         | Archivos seq." << endl;

    cout << "\n  APLICACIONES REALES:" << endl;
    cout << "  - MySQL/PostgreSQL : buffer pool LRU para paginas de disco" << endl;
    cout << "  - CPU              : cache L1/L2/L3 con politica LRU" << endl;
    cout << "  - DNS              : cache de resoluciones (TTL = tiempo de vida)" << endl;
    cout << "  - Navegadores      : cache HTTP de paginas web" << endl;
    cout << "  - Prog. Dinamica   : memoizacion de subproblemas (ej: Fibonacci)" << endl;

    liberarArbol(arbol);

    cout << "\n============================================" << endl;
    cout << "  FIN DEL EJERCICIO 08" << endl;
    cout << "============================================" << endl;
    return 0;
}

