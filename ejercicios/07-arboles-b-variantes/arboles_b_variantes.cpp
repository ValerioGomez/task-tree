// ============================================================================
// EJERCICIO 07: ÁRBOLES B, B+ y B* — COMPARATIVA
// Archivo: ejercicios/07-arboles-b-variantes/arboles_b_variantes.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic arboles_b_variantes.cpp -o b_variantes
// ============================================================================
//
// DIFERENCIAS CLAVE:
//
//  Árbol B   : llaves y datos en TODOS los nodos (raíz, internos y hojas)
//  Árbol B+  : llaves en internos SOLO para guiar; datos SOLO en hojas
//              las hojas se enlazan en lista doblemente encadenada
//  Árbol B*  : como B, pero el mínimo de ocupación es 2/3 (no 1/2)
//              retarda los splits redistribuyendo con hermanos primero
//
// Este archivo implementa:
//   PARTE 1 — Árbol B  (orden 4, igual que ejercicio 05)
//   PARTE 2 — Árbol B+ (orden 4, hojas enlazadas)
//   PARTE 3 — Árbol B* (orden 4, redistribución antes de split)
//   PARTE 4 — Demostración comparativa con el mismo conjunto de datos
//
// ============================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

// ============================================================================
//  PARTE 1 — ÁRBOL B (referencia, ya visto en ejercicio 05)
// ============================================================================
//
//  Propiedad: datos en TODOS los nodos
//
//  Ejemplo con {5, 15, 25, 35, 45}:
//
//          [25]
//         /    \  .
//      [15]   [35|45]
//      /   \  .
//   [5]   [20]
//
//  Al buscar 15: se encuentra en el nodo INTERNO [15] → termina allí.
//
// ============================================================================

namespace ArbolB {

#define B_ORDEN      4
#define B_MAX_LLAVES (B_ORDEN - 1)

struct Nodo {
    int num_llaves;
    int llaves[B_MAX_LLAVES];
    struct Nodo *hijos[B_ORDEN];
    bool es_hoja;
};
typedef struct Nodo *NodoPtr;

NodoPtr crearNodo(bool hoja) {
    NodoPtr p = (NodoPtr)malloc(sizeof(struct Nodo));
    p->num_llaves = 0;
    p->es_hoja    = hoja;
    for (int i = 0; i < B_ORDEN;      i++) p->hijos[i] = NULL;
    for (int i = 0; i < B_MAX_LLAVES; i++) p->llaves[i] = 0;
    return p;
}

bool buscar(NodoPtr n, int k) {
    if (n == NULL) return false;
    int i = 0;
    while (i < n->num_llaves && k > n->llaves[i]) i++;
    if (i < n->num_llaves && k == n->llaves[i]) return true;
    if (n->es_hoja) return false;
    return buscar(n->hijos[i], k);
}

void splitChild(NodoPtr padre, int i, NodoPtr hijoLleno) {
    int medio      = B_MAX_LLAVES / 2;
    NodoPtr nuevo  = crearNodo(hijoLleno->es_hoja);
    nuevo->num_llaves = B_MAX_LLAVES - medio - 1;
    for (int j = 0; j < nuevo->num_llaves; j++)
        nuevo->llaves[j] = hijoLleno->llaves[j + medio + 1];
    if (!hijoLleno->es_hoja)
        for (int j = 0; j <= nuevo->num_llaves; j++)
            nuevo->hijos[j] = hijoLleno->hijos[j + medio + 1];
    hijoLleno->num_llaves = medio;
    for (int j = padre->num_llaves; j >= i + 1; j--)
        padre->hijos[j + 1] = padre->hijos[j];
    padre->hijos[i + 1] = nuevo;
    for (int j = padre->num_llaves - 1; j >= i; j--)
        padre->llaves[j + 1] = padre->llaves[j];
    padre->llaves[i]   = hijoLleno->llaves[medio];
    padre->num_llaves += 1;
}

void insertarNoLleno(NodoPtr n, int k) {
    int i = n->num_llaves - 1;
    if (n->es_hoja) {
        while (i >= 0 && k < n->llaves[i]) { n->llaves[i+1] = n->llaves[i]; i--; }
        n->llaves[i+1]  = k;
        n->num_llaves  += 1;
    } else {
        while (i >= 0 && k < n->llaves[i]) i--;
        i++;
        if (n->hijos[i]->num_llaves == B_MAX_LLAVES) {
            splitChild(n, i, n->hijos[i]);
            if (k > n->llaves[i]) i++;
        }
        insertarNoLleno(n->hijos[i], k);
    }
}

NodoPtr insertar(NodoPtr raiz, int k) {
    if (raiz == NULL) {
        NodoPtr r = crearNodo(true);
        r->llaves[0] = k; r->num_llaves = 1;
        return r;
    }
    if (raiz->num_llaves == B_MAX_LLAVES) {
        NodoPtr nueva = crearNodo(false);
        nueva->hijos[0] = raiz;
        splitChild(nueva, 0, raiz);
        insertarNoLleno(nueva, k);
        return nueva;
    }
    insertarNoLleno(raiz, k);
    return raiz;
}

void imprimir(NodoPtr n, int nivel) {
    if (n == NULL) return;
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << "[";
    for (int i = 0; i < n->num_llaves; i++) {
        cout << n->llaves[i];
        if (i < n->num_llaves - 1) cout << "|";
    }
    cout << "]" << (n->es_hoja ? " <hoja>" : "") << endl;
    if (!n->es_hoja)
        for (int i = 0; i <= n->num_llaves; i++)
            imprimir(n->hijos[i], nivel + 1);
}

void traverse(NodoPtr n) {
    if (n == NULL) return;
    int i;
    for (i = 0; i < n->num_llaves; i++) {
        if (!n->es_hoja) traverse(n->hijos[i]);
        cout << n->llaves[i] << " ";
    }
    if (!n->es_hoja) traverse(n->hijos[i]);
}

void liberar(NodoPtr n) {
    if (n == NULL) return;
    if (!n->es_hoja) for (int i = 0; i <= n->num_llaves; i++) liberar(n->hijos[i]);
    free(n);
}

} // namespace ArbolB

// ============================================================================
//  PARTE 2 — ÁRBOL B+
// ============================================================================
//
//  DIFERENCIAS con B:
//  1. Los nodos INTERNOS solo tienen llaves (para guiar la búsqueda)
//  2. Los nodos HOJA tienen llaves + datos
//  3. Las hojas están enlazadas entre sí en una lista → recorrido secuencial O(n)
//  4. TODA llave aparece en una hoja; las llaves internas son COPIAS
//
//  Ejemplo con {5, 10, 15, 20, 25}:
//
//          [15]          ← nodo interno: solo guía
//         /    \  .
//     [5|10]  [15|20|25] ← hojas: tienen los datos reales
//        ↔                ← las hojas se enlazan (lista enlazada)
//
//  Al buscar 15: se lee el nodo interno [15], baja a la hoja derecha → encontrado
//  Ventaja: recorrido SECUENCIAL muy eficiente (base de datos: ORDER BY)
//
// ============================================================================

namespace ArbolBPlus {

#define BP_ORDEN      4
#define BP_MAX_LLAVES (BP_ORDEN - 1)

struct NodoHoja {
    int num_llaves;
    int llaves[BP_MAX_LLAVES];
    struct NodoHoja *siguiente;    // lista enlazada de hojas
    struct NodoHoja *anterior;
};

struct NodoInterno {
    int num_llaves;
    int llaves[BP_MAX_LLAVES];     // copias de llaves para guiar búsqueda
    void *hijos[BP_ORDEN];         // pueden ser NodoInterno* o NodoHoja*
    bool hijos_son_hojas;          // ¿el siguiente nivel son hojas?
};

typedef struct NodoHoja    *HojaPtr;
typedef struct NodoInterno *InternoPtr;

// Para simplificar la implementación usamos una estructura unificada
struct NodoBP {
    int  num_llaves;
    int  llaves[BP_MAX_LLAVES];
    struct NodoBP *hijos[BP_ORDEN];
    struct NodoBP *siguiente;      // solo para hojas
    bool  es_hoja;
};
typedef struct NodoBP *BPPtr;

BPPtr crearNodoBP(bool hoja) {
    BPPtr p = (BPPtr)malloc(sizeof(struct NodoBP));
    p->num_llaves = 0;
    p->es_hoja    = hoja;
    p->siguiente  = NULL;
    for (int i = 0; i < BP_ORDEN;      i++) p->hijos[i] = NULL;
    for (int i = 0; i < BP_MAX_LLAVES; i++) p->llaves[i] = 0;
    return p;
}

// Buscar en B+: SIEMPRE llega hasta una hoja
bool buscar(BPPtr n, int k) {
    if (n == NULL) return false;
    if (n->es_hoja) {
        for (int i = 0; i < n->num_llaves; i++)
            if (n->llaves[i] == k) return true;
        return false;
    }
    // Navegar por nodos internos
    int i = 0;
    while (i < n->num_llaves && k >= n->llaves[i]) i++;
    return buscar(n->hijos[i], k);
}

// Split de hoja: la llave del medio se COPIA al padre (no se mueve)
void splitHoja(BPPtr padre, int idx, BPPtr hoja) {
    int medio    = BP_MAX_LLAVES / 2 + 1;
    BPPtr nueva  = crearNodoBP(true);
    nueva->num_llaves = hoja->num_llaves - medio;
    for (int j = 0; j < nueva->num_llaves; j++)
        nueva->llaves[j] = hoja->llaves[j + medio];
    hoja->num_llaves = medio;

    // Enlazar hojas
    nueva->siguiente = hoja->siguiente;
    hoja->siguiente  = nueva;

    // La primera llave de la hoja nueva SUBE al padre (es una copia)
    int llave_que_sube = nueva->llaves[0];

    // Hacer espacio en padre
    for (int j = padre->num_llaves; j > idx; j--) {
        padre->llaves[j]     = padre->llaves[j-1];
        padre->hijos[j+1]    = padre->hijos[j];
    }
    padre->llaves[idx]   = llave_que_sube;
    padre->hijos[idx+1]  = nueva;
    padre->num_llaves   += 1;
}

// Split de nodo interno (igual que B)
void splitInterno(BPPtr padre, int idx, BPPtr nodo) {
    int medio    = BP_MAX_LLAVES / 2;
    BPPtr nuevo  = crearNodoBP(false);
    nuevo->num_llaves = BP_MAX_LLAVES - medio - 1;
    for (int j = 0; j < nuevo->num_llaves; j++)
        nuevo->llaves[j] = nodo->llaves[j + medio + 1];
    for (int j = 0; j <= nuevo->num_llaves; j++)
        nuevo->hijos[j] = nodo->hijos[j + medio + 1];
    nodo->num_llaves = medio;

    for (int j = padre->num_llaves; j > idx; j--) {
        padre->llaves[j]     = padre->llaves[j-1];
        padre->hijos[j+1]    = padre->hijos[j];
    }
    padre->llaves[idx]   = nodo->llaves[medio]; // sube y NO queda en nodo
    padre->hijos[idx+1]  = nuevo;
    padre->num_llaves   += 1;
}

void insertarEnNodo(BPPtr n, int k);

void insertarNoLleno(BPPtr n, int k) {
    if (n->es_hoja) {
        int i = n->num_llaves - 1;
        while (i >= 0 && k < n->llaves[i]) { n->llaves[i+1] = n->llaves[i]; i--; }
        n->llaves[i+1]  = k;
        n->num_llaves  += 1;
    } else {
        int i = 0;
        while (i < n->num_llaves && k >= n->llaves[i]) i++;
        BPPtr hijo = n->hijos[i];
        if (hijo->num_llaves == BP_MAX_LLAVES) {
            if (hijo->es_hoja) splitHoja(n, i, hijo);
            else               splitInterno(n, i, hijo);
            if (k >= n->llaves[i]) i++;
        }
        insertarNoLleno(n->hijos[i], k);
    }
}

BPPtr insertar(BPPtr raiz, int k) {
    if (raiz == NULL) {
        BPPtr r = crearNodoBP(true);
        r->llaves[0] = k; r->num_llaves = 1;
        return r;
    }
    if (raiz->num_llaves == BP_MAX_LLAVES) {
        BPPtr nueva = crearNodoBP(false);
        nueva->hijos[0] = raiz;
        if (raiz->es_hoja) splitHoja(nueva, 0, raiz);
        else               splitInterno(nueva, 0, raiz);
        insertarNoLleno(nueva, k);
        return nueva;
    }
    insertarNoLleno(raiz, k);
    return raiz;
}

// RECORRIDO SECUENCIAL — ventaja exclusiva del B+
// Encuentra la hoja más izquierda y recorre la lista enlazada
void recorridoSecuencial(BPPtr raiz) {
    if (raiz == NULL) return;
    // Bajar hasta la hoja más izquierda
    BPPtr hoja = raiz;
    while (!hoja->es_hoja) hoja = hoja->hijos[0];
    // Recorrer la lista de hojas
    while (hoja != NULL) {
        for (int i = 0; i < hoja->num_llaves; i++)
            cout << hoja->llaves[i] << " ";
        hoja = hoja->siguiente;
    }
}

void imprimir(BPPtr n, int nivel) {
    if (n == NULL) return;
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << (n->es_hoja ? "HOJA[" : "INT [");
    for (int i = 0; i < n->num_llaves; i++) {
        cout << n->llaves[i];
        if (i < n->num_llaves - 1) cout << "|";
    }
    cout << "]" << endl;
    if (!n->es_hoja)
        for (int i = 0; i <= n->num_llaves; i++)
            imprimir(n->hijos[i], nivel + 1);
}

void liberar(BPPtr n) {
    if (n == NULL) return;
    if (!n->es_hoja) for (int i = 0; i <= n->num_llaves; i++) liberar(n->hijos[i]);
    free(n);
}

} // namespace ArbolBPlus

// ============================================================================
//  PARTE 3 — ÁRBOL B* (descripción + demo de política de llenado)
// ============================================================================
//
//  DIFERENCIAS con B:
//  1. Mínimo de ocupación: 2/3 (B usa 1/2)
//  2. ANTES de hacer split, intenta redistribuir con el hermano (izq o der)
//  3. Si AMBOS hermanos están llenos → split de 2 nodos en 3 (no 1 en 2)
//
//  Consecuencia: los nodos están más llenos en promedio → árbol más bajo →
//  menos accesos a disco.
//
//  La implementación completa del B* es extensa; aquí mostramos la política
//  de redistribución con un ejemplo controlado.
//
// ============================================================================

namespace ArbolBStar {

#define BS_ORDEN       4
#define BS_MAX_LLAVES  (BS_ORDEN - 1)          // 3
#define BS_MIN_LLAVES  ((2 * (BS_ORDEN-1))/3)  // ≈ 2 (mínimo 2/3)

struct Nodo {
    int num_llaves;
    int llaves[BS_MAX_LLAVES];
    struct Nodo *hijos[BS_ORDEN];
    bool es_hoja;
};
typedef struct Nodo *NodoPtr;

NodoPtr crearNodo(bool hoja) {
    NodoPtr p = (NodoPtr)malloc(sizeof(struct Nodo));
    p->num_llaves = 0;
    p->es_hoja    = hoja;
    for (int i = 0; i < BS_ORDEN;      i++) p->hijos[i] = NULL;
    for (int i = 0; i < BS_MAX_LLAVES; i++) p->llaves[i] = 0;
    return p;
}

// Redistribuir: mover una llave del hermano izquierdo al nodo actual
// pasando por el padre (rotación)
bool redistribuirDesdeIzq(NodoPtr padre, int idx) {
    if (idx == 0) return false;             // no hay hermano izquierdo
    NodoPtr hermano = padre->hijos[idx-1];
    NodoPtr actual  = padre->hijos[idx];
    if (hermano->num_llaves <= BS_MIN_LLAVES) return false; // hermano justo lleno

    // Hacer espacio al inicio de actual
    for (int j = actual->num_llaves; j > 0; j--)
        actual->llaves[j] = actual->llaves[j-1];
    if (!actual->es_hoja) {
        for (int j = actual->num_llaves + 1; j > 0; j--)
            actual->hijos[j] = actual->hijos[j-1];
    }

    // La llave del padre baja a actual
    actual->llaves[0] = padre->llaves[idx-1];
    if (!hermano->es_hoja)
        actual->hijos[0] = hermano->hijos[hermano->num_llaves];

    actual->num_llaves++;

    // La última llave del hermano sube al padre
    padre->llaves[idx-1] = hermano->llaves[hermano->num_llaves-1];
    hermano->num_llaves--;

    cout << "  [B*] Redistribucion desde hermano izquierdo" << endl;
    return true;
}

// Redistribuir desde hermano derecho
bool redistribuirDesDer(NodoPtr padre, int idx) {
    if (idx >= padre->num_llaves) return false; // no hay hermano derecho
    NodoPtr hermano = padre->hijos[idx+1];
    NodoPtr actual  = padre->hijos[idx];
    if (hermano->num_llaves <= BS_MIN_LLAVES) return false;

    // La llave del padre baja al final de actual
    actual->llaves[actual->num_llaves] = padre->llaves[idx];
    if (!hermano->es_hoja)
        actual->hijos[actual->num_llaves+1] = hermano->hijos[0];
    actual->num_llaves++;

    // La primera llave del hermano sube al padre
    padre->llaves[idx] = hermano->llaves[0];
    for (int j = 0; j < hermano->num_llaves-1; j++)
        hermano->llaves[j] = hermano->llaves[j+1];
    if (!hermano->es_hoja)
        for (int j = 0; j < hermano->num_llaves; j++)
            hermano->hijos[j] = hermano->hijos[j+1];
    hermano->num_llaves--;

    cout << "  [B*] Redistribucion desde hermano derecho" << endl;
    return true;
}

void splitChild(NodoPtr padre, int idx, NodoPtr hijo) {
    int medio    = BS_MAX_LLAVES / 2;
    NodoPtr nuevo = crearNodo(hijo->es_hoja);
    nuevo->num_llaves = BS_MAX_LLAVES - medio - 1;
    for (int j = 0; j < nuevo->num_llaves; j++)
        nuevo->llaves[j] = hijo->llaves[j + medio + 1];
    if (!hijo->es_hoja)
        for (int j = 0; j <= nuevo->num_llaves; j++)
            nuevo->hijos[j] = hijo->hijos[j + medio + 1];
    hijo->num_llaves = medio;
    for (int j = padre->num_llaves; j > idx; j--) {
        padre->llaves[j]     = padre->llaves[j-1];
        padre->hijos[j+1]    = padre->hijos[j];
    }
    padre->hijos[idx+1]  = nuevo;
    padre->llaves[idx]   = hijo->llaves[medio];
    padre->num_llaves++;
    cout << "  [B*] Split (redistribucion no posible)" << endl;
}

void insertarNoLleno(NodoPtr n, int k) {
    int i = n->num_llaves - 1;
    if (n->es_hoja) {
        while (i >= 0 && k < n->llaves[i]) { n->llaves[i+1] = n->llaves[i]; i--; }
        n->llaves[i+1]  = k;
        n->num_llaves  += 1;
    } else {
        while (i >= 0 && k < n->llaves[i]) i--;
        i++;
        NodoPtr hijo = n->hijos[i];
        if (hijo->num_llaves == BS_MAX_LLAVES) {
            // B*: intentar redistribuir ANTES de hacer split
            if (!redistribuirDesdeIzq(n, i) && !redistribuirDesDer(n, i)) {
                splitChild(n, i, hijo);
                if (k > n->llaves[i]) i++;
            }
        }
        insertarNoLleno(n->hijos[i], k);
    }
}

NodoPtr insertar(NodoPtr raiz, int k) {
    if (raiz == NULL) {
        NodoPtr r = crearNodo(true);
        r->llaves[0] = k; r->num_llaves = 1;
        return r;
    }
    if (raiz->num_llaves == BS_MAX_LLAVES) {
        NodoPtr nueva = crearNodo(false);
        nueva->hijos[0] = raiz;
        splitChild(nueva, 0, raiz);
        insertarNoLleno(nueva, k);
        return nueva;
    }
    insertarNoLleno(raiz, k);
    return raiz;
}

void traverse(NodoPtr n) {
    if (n == NULL) return;
    int i;
    for (i = 0; i < n->num_llaves; i++) {
        if (!n->es_hoja) traverse(n->hijos[i]);
        cout << n->llaves[i] << " ";
    }
    if (!n->es_hoja) traverse(n->hijos[i]);
}

void imprimir(NodoPtr n, int nivel) {
    if (n == NULL) return;
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << "[";
    for (int i = 0; i < n->num_llaves; i++) {
        cout << n->llaves[i];
        if (i < n->num_llaves - 1) cout << "|";
    }
    cout << "]" << (n->es_hoja ? " <hoja>" : "") << endl;
    if (!n->es_hoja)
        for (int i = 0; i <= n->num_llaves; i++)
            imprimir(n->hijos[i], nivel + 1);
}

void liberar(NodoPtr n) {
    if (n == NULL) return;
    if (!n->es_hoja) for (int i = 0; i <= n->num_llaves; i++) liberar(n->hijos[i]);
    free(n);
}

} // namespace ArbolBStar

// ============================================================================
//  MAIN — Comparativa de los tres árboles con los mismos datos
// ============================================================================
int main() {
    cout << "============================================" << endl;
    cout << "  EJERCICIO 07: ARBOLES B, B+ y B*" << endl;
    cout << "============================================" << endl;

    int datos[] = {10, 20, 5, 6, 12, 30, 7, 17, 3, 25, 15, 8, 35, 40, 1};
    int n = 15;

    cout << "\nDatos a insertar: ";
    for (int i = 0; i < n; i++) { cout << datos[i]; if (i<n-1) cout<<", "; }
    cout << "\nOrden del arbol: 4  |  Max llaves por nodo: 3\n" << endl;

    // ─── Árbol B ──────────────────────────────────────────────────────────
    cout << "╔══════════════════════════════════╗" << endl;
    cout << "║  ARBOL B  (datos en todos los nodos) ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    ArbolB::NodoPtr tB = NULL;
    for (int i = 0; i < n; i++) tB = ArbolB::insertar(tB, datos[i]);
    cout << "\nEstructura:" << endl;
    ArbolB::imprimir(tB, 0);
    cout << "\nRecorrido inorden: ";
    ArbolB::traverse(tB); cout << endl;
    cout << "Buscar 17: " << (ArbolB::buscar(tB, 17) ? "ENCONTRADO" : "NO") << endl;
    cout << "Buscar 99: " << (ArbolB::buscar(tB, 99) ? "ENCONTRADO" : "NO") << endl;
    ArbolB::liberar(tB);

    // ─── Árbol B+ ─────────────────────────────────────────────────────────
    cout << "\n╔══════════════════════════════════╗" << endl;
    cout << "║  ARBOL B+  (datos SOLO en hojas enlazadas) ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    ArbolBPlus::BPPtr tBP = NULL;
    for (int i = 0; i < n; i++) tBP = ArbolBPlus::insertar(tBP, datos[i]);
    cout << "\nEstructura (INT=interno, HOJA=hoja con datos):" << endl;
    ArbolBPlus::imprimir(tBP, 0);
    cout << "\nRecorrido SECUENCIAL (lista enlazada de hojas):" << endl;
    cout << "  ";
    ArbolBPlus::recorridoSecuencial(tBP); cout << endl;
    cout << "Buscar 17: " << (ArbolBPlus::buscar(tBP, 17) ? "ENCONTRADO" : "NO") << endl;
    cout << "Buscar 99: " << (ArbolBPlus::buscar(tBP, 99) ? "ENCONTRADO" : "NO") << endl;
    ArbolBPlus::liberar(tBP);

    // ─── Árbol B* ─────────────────────────────────────────────────────────
    cout << "\n╔══════════════════════════════════╗" << endl;
    cout << "║  ARBOL B*  (redistribucion antes de split) ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    cout << "(Min ocupacion: 2/3 | Redistribuye con hermanos antes de dividir)" << endl;
    ArbolBStar::NodoPtr tBS = NULL;
    for (int i = 0; i < n; i++) {
        cout << "Insertar " << datos[i] << ":" << endl;
        tBS = ArbolBStar::insertar(tBS, datos[i]);
    }
    cout << "\nEstructura final:" << endl;
    ArbolBStar::imprimir(tBS, 0);
    cout << "\nRecorrido inorden: ";
    ArbolBStar::traverse(tBS); cout << endl;
    ArbolBStar::liberar(tBS);

    // ─── Tabla comparativa ────────────────────────────────────────────────
    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║              TABLA COMPARATIVA B vs B+ vs B*              ║" << endl;
    cout << "╠════════════╦══════════╦════════════╦════════════════════╣" << endl;
    cout << "║ Propiedad  ║  Arbol B ║  Arbol B+  ║      Arbol B*      ║" << endl;
    cout << "╠════════════╬══════════╬════════════╬════════════════════╣" << endl;
    cout << "║ Datos en   ║ Todos    ║ Solo hojas ║ Todos              ║" << endl;
    cout << "║ Min ocup.  ║ 1/2      ║ 1/2        ║ 2/3                ║" << endl;
    cout << "║ Hojas      ║ No enl.  ║ Enlazadas  ║ No enlazadas       ║" << endl;
    cout << "║ Busqueda   ║ O(log n) ║ O(log n)   ║ O(log n)           ║" << endl;
    cout << "║ Secuencial ║ O(n)     ║ O(n) rapido║ O(n)               ║" << endl;
    cout << "║ Uso tipico ║ General  ║ DB indices ║ Sistemas de arch.  ║" << endl;
    cout << "╚════════════╩══════════╩════════════╩════════════════════╝" << endl;

    cout << "\n============================================" << endl;
    cout << "  FIN DEL EJERCICIO 07" << endl;
    cout << "============================================" << endl;
    return 0;
}

