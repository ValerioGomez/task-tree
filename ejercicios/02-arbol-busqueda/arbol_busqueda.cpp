// ============================================================================
// EJERCICIO 02: ÁRBOL BINARIO DE BÚSQUEDA (BST)
// Archivo: ejercicios/02-arbol-busqueda/arbol_busqueda.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic arbol_busqueda.cpp -o arbol_busqueda
// ============================================================================
//
// Propiedad BST:
//   Para todo nodo N:
//     - Todos los valores en el subárbol izquierdo < N.dato
//     - Todos los valores en el subárbol derecho   >= N.dato
//
// Operaciones implementadas:
//   - find       : búsqueda recursiva
//   - insertar     : inserción recursiva manteniendo propiedad BST
//   - size       : contar nodos
//   - minimum    : valor mínimo (nodo más a la izquierda)
//   - maximum    : valor máximo (nodo más a la derecha)
//   - deleteNode : eliminación con 3 casos
//
// Prueba con: {15, 4, 8, 7, 4, 3, 19, 5, 7, 9, 16, 5, 17}
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
// OPERACIONES PRIMITIVAS (del ejercicio 01)
// ─────────────────────────────────────────────────────────────────────────────
NodoPtr createNode() {
    NodoPtr p = (NodoPtr)malloc(sizeof(struct NodoArbol));
    return p;
}

void freeNode(NodoPtr p) {
    free(p);
}

NodoPtr crearArbol(int x) {
    NodoPtr root = createNode();
    root->dato   = x;
    root->padre = NULL;
    root->izquierdo   = NULL;
    root->derecho  = NULL;
    return root;
}

void asignarHijoIzquierdo(NodoPtr pNode, int x) {
    if (pNode == NULL) return;
    NodoPtr nuevo = crearArbol(x);
    nuevo->padre = pNode;
    pNode->izquierdo   = nuevo;
}

void asignarHijoDerecho(NodoPtr pNode, int x) {
    if (pNode == NULL) return;
    NodoPtr nuevo = crearArbol(x);
    nuevo->padre = pNode;
    pNode->derecho  = nuevo;
}

void liberarArbol(NodoPtr p) {
    if (p == NULL) return;
    liberarArbol(p->izquierdo);
    liberarArbol(p->derecho);
    freeNode(p);
}

// ─────────────────────────────────────────────────────────────────────────────
// VISUALIZACIÓN (árbol girado 90°)
// ─────────────────────────────────────────────────────────────────────────────
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
// BÚSQUEDA RECURSIVA
//   Retorna true si 'data' está en el subárbol con raíz 'pNode'
// ─────────────────────────────────────────────────────────────────────────────
bool find(NodoPtr pNode, int data) {
    if (pNode == NULL)        return false;     // caso base: no encontrado
    if (data == pNode->dato)  return true;      // caso base: encontrado
    if (data < pNode->dato)
        return find(pNode->izquierdo, data);         // buscar en izquierda
    else
        return find(pNode->derecho, data);        // buscar en derecha
}

// ─────────────────────────────────────────────────────────────────────────────
// INSERCIÓN RECURSIVA
//   Si data < nodo → va a la izquierda
//   Si data >= nodo → va a la derecha
//   Retorna el puntero a la raíz actualizada
// ─────────────────────────────────────────────────────────────────────────────
NodoPtr insertar(NodoPtr pNode, int data) {
    if (pNode == NULL) {
        return crearArbol(data);              // crear hoja nueva
    }
    if (data < pNode->dato) {
        NodoPtr nuevoIzq = insertar(pNode->izquierdo, data);
        pNode->izquierdo       = nuevoIzq;
        nuevoIzq->padre  = pNode;
    } else {
        NodoPtr nuevoDer = insertar(pNode->derecho, data);
        pNode->derecho      = nuevoDer;
        nuevoDer->padre  = pNode;
    }
    return pNode;
}

// ─────────────────────────────────────────────────────────────────────────────
// CONTAR NODOS (size)
// ─────────────────────────────────────────────────────────────────────────────
int size(NodoPtr pNode) {
    if (pNode == NULL) return 0;
    return 1 + size(pNode->izquierdo) + size(pNode->derecho);
}

// ─────────────────────────────────────────────────────────────────────────────
// MÍNIMO — nodo más a la izquierda
// ─────────────────────────────────────────────────────────────────────────────
int minimum(NodoPtr pNode) {
    if (pNode == NULL) {
        cout << "ERROR: arbol vacio en minimum()" << endl;
        return -1;
    }
    if (pNode->izquierdo == NULL) return pNode->dato;  // caso base: llegamos al más izq
    return minimum(pNode->izquierdo);
}

// Versión que retorna el NODO (útil para deleteNode)
NodoPtr minNode(NodoPtr pNode) {
    if (pNode == NULL) return NULL;
    if (pNode->izquierdo == NULL) return pNode;
    return minNode(pNode->izquierdo);
}

// ─────────────────────────────────────────────────────────────────────────────
// MÁXIMO — nodo más a la derecha
// ─────────────────────────────────────────────────────────────────────────────
int maximum(NodoPtr pNode) {
    if (pNode == NULL) {
        cout << "ERROR: arbol vacio en maximum()" << endl;
        return -1;
    }
    if (pNode->derecho == NULL) return pNode->dato; // caso base
    return maximum(pNode->derecho);
}

// ─────────────────────────────────────────────────────────────────────────────
// ELIMINACIÓN — 3 CASOS
//
//   Caso 1: Nodo es HOJA (sin hijos) → simplemente se elimina
//   Caso 2: Nodo tiene UN hijo → se reemplaza por ese hijo
//   Caso 3: Nodo tiene DOS hijos → se reemplaza por su SUCESOR EN INORDEN
//           (el mínimo del subárbol derecho) y se elimina el sucesor
// ─────────────────────────────────────────────────────────────────────────────
NodoPtr deleteNode(NodoPtr root, int x) {
    if (root == NULL) {
        cout << "  Valor " << x << " no encontrado en el arbol." << endl;
        return NULL;
    }

    if (x < root->dato) {
        // Buscar en subárbol izquierdo
        root->izquierdo = deleteNode(root->izquierdo, x);
        if (root->izquierdo != NULL)
            root->izquierdo->padre = root;
    } else if (x > root->dato) {
        // Buscar en subárbol derecho
        root->derecho = deleteNode(root->derecho, x);
        if (root->derecho != NULL)
            root->derecho->padre = root;
    } else {
        // ¡Encontramos el nodo a eliminar!

        if (root->izquierdo == NULL && root->derecho == NULL) {
            // CASO 1: Nodo hoja
            cout << "  Caso 1 (hoja): Eliminando " << x << endl;
            freeNode(root);
            return NULL;

        } else if (root->izquierdo == NULL) {
            // CASO 2a: Solo tiene hijo derecho
            cout << "  Caso 2 (un hijo): Eliminando " << x
                 << ", sube hijo derecho " << root->derecho->dato << endl;
            NodoPtr temp = root->derecho;
            temp->padre  = root->padre;
            freeNode(root);
            return temp;

        } else if (root->derecho == NULL) {
            // CASO 2b: Solo tiene hijo izquierdo
            cout << "  Caso 2 (un hijo): Eliminando " << x
                 << ", sube hijo izquierdo " << root->izquierdo->dato << endl;
            NodoPtr temp = root->izquierdo;
            temp->padre  = root->padre;
            freeNode(root);
            return temp;

        } else {
            // CASO 3: Dos hijos — reemplazar con sucesor en inorden
            NodoPtr sucesor = minNode(root->derecho);
            cout << "  Caso 3 (dos hijos): Eliminando " << x
                 << ", reemplazando con sucesor " << sucesor->dato << endl;
            root->dato  = sucesor->dato;           // copiar valor del sucesor
            root->derecho = deleteNode(root->derecho, sucesor->dato); // eliminar sucesor
            if (root->derecho != NULL)
                root->derecho->padre = root;
        }
    }
    return root;
}

// ─────────────────────────────────────────────────────────────────────────────
// RECORRIDO INORDEN (para verificar que sigue ordenado después de delete)
// ─────────────────────────────────────────────────────────────────────────────
void inorden(NodoPtr pNode) {
    if (pNode == NULL) return;
    inorden(pNode->izquierdo);
    cout << pNode->dato << " ";
    inorden(pNode->derecho);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  EJERCICIO 02: ARBOL BINARIO DE BUSQUEDA" << endl;
    cout << "============================================" << endl;

    // ─── Insertar el array de prueba ──────────────────────────────────────
    int numbers[] = {15, 4, 8, 7, 4, 3, 19, 5, 7, 9, 16, 5, 17};
    int n = 13;

    NodoPtr tree = NULL;
    cout << "\nInsertando: ";
    for (int i = 0; i < n; i++) {
        cout << numbers[i];
        if (i < n - 1) cout << ", ";
        tree = insertar(tree, numbers[i]);
    }
    cout << endl;

    // ─── Visualizar el árbol ──────────────────────────────────────────────
    cout << "\nEstructura del BST (girado 90 grados):" << endl;
    cout << "(derecha arriba = mayor, izquierda abajo = menor)" << endl << endl;
    imprimirArbol(tree, 0);

    // ─── Estadísticas básicas ─────────────────────────────────────────────
    cout << "\n--- Estadisticas ---" << endl;
    cout << "Cantidad de nodos : " << size(tree)    << endl;
    cout << "Valor minimo      : " << minimum(tree) << endl;
    cout << "Valor maximo      : " << maximum(tree) << endl;

    // ─── Recorrido inorden (debe ser ascendente) ──────────────────────────
    cout << "\nRecorrido inorden (debe ser ascendente): " << endl;
    inorden(tree);
    cout << endl;

    // ─── Búsqueda ─────────────────────────────────────────────────────────
    cout << "\n--- Busqueda ---" << endl;
    int buscar[] = {7, 15, 100, 3, 20};
    for (int i = 0; i < 5; i++) {
        cout << "find(" << buscar[i] << ") = "
             << (find(tree, buscar[i]) ? "ENCONTRADO" : "NO encontrado") << endl;
    }

    // ─── Eliminación: los 3 casos ─────────────────────────────────────────
    cout << "\n--- Eliminacion (3 casos) ---" << endl;

    // Caso 1: eliminar hoja (9 es hoja en este árbol)
    cout << "Eliminar 9 (hoja):" << endl;
    tree = deleteNode(tree, 9);
    cout << "Inorden despues: "; inorden(tree); cout << endl;

    // Caso 2: eliminar nodo con un hijo (16 tiene solo hijo derecho 17)
    cout << "\nEliminar 16 (un hijo):" << endl;
    tree = deleteNode(tree, 16);
    cout << "Inorden despues: "; inorden(tree); cout << endl;

    // Caso 3: eliminar nodo con dos hijos (4 tiene hijos 3 y 8)
    cout << "\nEliminar 4 (dos hijos):" << endl;
    tree = deleteNode(tree, 4);
    cout << "Inorden despues: "; inorden(tree); cout << endl;

    cout << "\nArbol final:" << endl;
    imprimirArbol(tree, 0);

    liberarArbol(tree);

    cout << "\n============================================" << endl;
    cout << "  FIN DEL EJERCICIO 02" << endl;
    cout << "============================================" << endl;

    return 0;
}

