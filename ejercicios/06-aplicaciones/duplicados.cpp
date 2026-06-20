// ============================================================================
// EJERCICIO 06 — APLICACIÓN 1: ENCONTRAR DUPLICADOS
// Archivo: ejercicios/06-aplicaciones/duplicados.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic duplicados.cpp -o duplicados
// ============================================================================
//
// Problema: Dado el array {15,4,8,7,4,3,19,5,7,9,16,5,17}, encontrar todos
//           los valores que aparecen más de una vez.
//
// Estrategia: Usar un BST como estructura de apoyo.
//   - Intentar insertar cada número en el árbol.
//   - Si ya existe (find devuelve true), es un duplicado.
//   - Si no existe, insertar normalmente.
//
// Complejidad: O(n log n) promedio
//
// ============================================================================

#include <iostream>
#include <cstdlib>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// ESTRUCTURA DEL NODO
// ─────────────────────────────────────────────────────────────────────────────
struct NodoArbol {
    int dato;
    struct NodoArbol *padre;
    struct NodoArbol *izquierdo;
    struct NodoArbol *derecho;
};
typedef struct NodoArbol *NodoPtr;

// ─────────────────────────────────────────────────────────────────────────────
// PRIMITIVAS
// ─────────────────────────────────────────────────────────────────────────────
NodoPtr crearArbol(int x) {
    NodoPtr root = (NodoPtr)malloc(sizeof(struct NodoArbol));
    root->dato   = x;
    root->padre = NULL;
    root->izquierdo   = NULL;
    root->derecho  = NULL;
    return root;
}

void asignarHijoIzquierdo(NodoPtr pNode, int x) {
    if (pNode == NULL) return;
    NodoPtr n = crearArbol(x);
    n->padre  = pNode;
    pNode->izquierdo = n;
}

void asignarHijoDerecho(NodoPtr pNode, int x) {
    if (pNode == NULL) return;
    NodoPtr n = crearArbol(x);
    n->padre   = pNode;
    pNode->derecho = n;
}

void liberarArbol(NodoPtr p) {
    if (p == NULL) return;
    liberarArbol(p->izquierdo);
    liberarArbol(p->derecho);
    free(p);
}

void imprimirArbol(NodoPtr pNode, int nivel, char prefijo = 'R') {
    if (pNode == NULL) return;
    imprimirArbol(pNode->derecho, nivel + 1, 'd');
    for (int i = 0; i < nivel; i++) cout << "    ";
    
    if (prefijo == 'R')
        cout << "[" << pNode->dato << "]" << endl;
    else if (prefijo == 'd')
        cout << "[" << pNode->dato << "]d" << endl;
    else
        cout << "i[" << pNode->dato << "]" << endl;
        
    imprimirArbol(pNode->izquierdo, nivel + 1, 'i');
}

// ─────────────────────────────────────────────────────────────────────────────
// ENCONTRAR DUPLICADOS (lógica exacta del enunciado)
//
//   Para cada numbers[i]:
//     Recorrer el árbol comparando (p y q)
//     Si numbers[i] == p->dato → es duplicado
//     Si q llega a NULL → insertar
// ─────────────────────────────────────────────────────────────────────────────
void encontrarDuplicados() {
    int numbers[13] = {15, 4, 8, 7, 4, 3, 19, 5, 7, 9, 16, 5, 17};
    int n = 13;

    cout << "\nArray de entrada: ";
    for (int i = 0; i < n; i++) {
        cout << numbers[i];
        if (i < n - 1) cout << ", ";
    }
    cout << endl;

    // Crear el árbol con el primer elemento
    NodoPtr tree = crearArbol(numbers[0]);
    cout << "\nProcesando elemento por elemento:" << endl;
    cout << "  Insertando " << numbers[0] << " (raiz)" << endl;

    for (int i = 1; i < n; i++) {
        NodoPtr p = tree;   // puntero de búsqueda
        NodoPtr q = tree;   // siempre un paso adelante

        // Recorrer hasta encontrar el valor o llegar a NULL
        while (numbers[i] != p->dato && q != NULL) {
            p = q;
            if (numbers[i] < p->dato)
                q = p->izquierdo;
            else
                q = p->derecho;
        }

        // Verificar resultado de la búsqueda
        if (numbers[i] == p->dato) {
            cout << "  *** DUPLICADO encontrado: " << numbers[i] << " ***" << endl;
        } else if (numbers[i] < p->dato) {
            asignarHijoIzquierdo(p, numbers[i]);
            cout << "  Insertando " << numbers[i] << " como hijo izquierdo de " << p->dato << endl;
        } else {
            asignarHijoDerecho(p, numbers[i]);
            cout << "  Insertando " << numbers[i] << " como hijo derecho de " << p->dato << endl;
        }
    }

    cout << "\nArbol resultante (sin duplicados):" << endl;
    imprimirArbol(tree, 0);

    liberarArbol(tree);
}

// ─────────────────────────────────────────────────────────────────────────────
// VARIANTE: contar cuántas veces aparece cada elemento
// ─────────────────────────────────────────────────────────────────────────────
struct NodoConteo {
    int dato;
    int count;               // cuántas veces apareció
    struct NodoConteo *padre;
    struct NodoConteo *izquierdo;
    struct NodoConteo *derecho;
};
typedef struct NodoConteo *NodoConteoPtr;

NodoConteoPtr crearArbolConteo(int x) {
    NodoConteoPtr root = (NodoConteoPtr)malloc(sizeof(struct NodoConteo));
    root->dato   = x;
    root->count  = 1;
    root->padre = NULL;
    root->izquierdo   = NULL;
    root->derecho  = NULL;
    return root;
}

NodoConteoPtr insertarConConteo(NodoConteoPtr p, int x) {
    if (p == NULL) return crearArbolConteo(x);
    if (x == p->dato) {
        p->count++;          // incrementar contador de apariciones
    } else if (x < p->dato) {
        NodoConteoPtr izq = insertarConConteo(p->izquierdo, x);
        p->izquierdo = izq;
        izq->padre = p;
    } else {
        NodoConteoPtr der = insertarConConteo(p->derecho, x);
        p->derecho = der;
        der->padre = p;
    }
    return p;
}

void imprimirConteos(NodoConteoPtr p) {
    if (p == NULL) return;
    imprimirConteos(p->izquierdo);
    if (p->count > 1) {
        cout << "  Valor " << p->dato << ": aparece " << p->count << " veces (DUPLICADO)" << endl;
    } else {
        cout << "  Valor " << p->dato << ": aparece " << p->count << " vez" << endl;
    }
    imprimirConteos(p->derecho);
}

void liberarArbolConteo(NodoConteoPtr p) {
    if (p == NULL) return;
    liberarArbolConteo(p->izquierdo);
    liberarArbolConteo(p->derecho);
    free(p);
}

void contarFrecuencias() {
    int numbers[] = {15, 4, 8, 7, 4, 3, 19, 5, 7, 9, 16, 5, 17};
    int n = 13;

    NodoConteoPtr tree = NULL;
    for (int i = 0; i < n; i++) {
        tree = insertarConConteo(tree, numbers[i]);
    }

    cout << "\n--- VARIANTE: Conteo de frecuencias ---" << endl;
    imprimirConteos(tree);
    liberarArbolConteo(tree);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  APLICACION 1: ENCONTRAR DUPLICADOS" << endl;
    cout << "============================================" << endl;

    cout << "\n=== Metodo original del enunciado ===" << endl;
    encontrarDuplicados();

    contarFrecuencias();

    cout << "\n============================================" << endl;
    cout << "  FIN DE APLICACION 1" << endl;
    cout << "============================================" << endl;

    return 0;
}

