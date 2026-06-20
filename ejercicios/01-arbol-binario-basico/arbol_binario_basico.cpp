// ============================================================================
// EJERCICIO 01: ÁRBOL BINARIO BÁSICO
// Archivo: ejercicios/01-arbol-binario-basico/arbol_binario_basico.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic arbol_binario_basico.cpp -o arbol_binario
// ============================================================================
//
// Implementa las operaciones primitivas de un árbol binario:
//   - Estructura NodoArbol con dato, padre, izquierdo, derecho
//   - createNode / freeNode / crearArbol
//   - padre / hijoIzquierdo / hijoDerecho
//   - asignarHijoIzquierdo / asignarHijoDerecho
//   - esHijoIzquierdo / esHijoDerecho / hermano
//   - construirEjemplo en 3 formas distintas
//   - Visualización del árbol
// ============================================================================

#include <iostream>
#include <cstdlib>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// DEFINICIÓN DE LA ESTRUCTURA
// ─────────────────────────────────────────────────────────────────────────────
struct NodoArbol {
    int dato;
    struct NodoArbol *padre;
    struct NodoArbol *izquierdo;
    struct NodoArbol *derecho;
};

typedef struct NodoArbol *NodoPtr;

// ─────────────────────────────────────────────────────────────────────────────
// OPERACIONES PRIMITIVAS
// ─────────────────────────────────────────────────────────────────────────────

// Crea un nodo sin inicializar (solo reserva memoria)
NodoPtr createNode() {
    NodoPtr p = (NodoPtr)malloc(sizeof(struct NodoArbol));
    return p;
}

// Libera la memoria de un nodo
void freeNode(NodoPtr p) {
    free(p);
}

// Crea un árbol de UN solo nodo con valor x
NodoPtr crearArbol(int x) {
    NodoPtr root = createNode();
    root->dato   = x;
    root->padre = NULL;
    root->izquierdo   = NULL;
    root->derecho  = NULL;
    return root;
}

// ─────────────────────────────────────────────────────────────────────────────
// FUNCIONES DE ACCESO A RELACIONES FAMILIARES
// ─────────────────────────────────────────────────────────────────────────────

// Retorna el padre del nodo (NULL si es raíz)
NodoPtr padre(NodoPtr pNode) {
    if (pNode == NULL) return NULL;
    return pNode->padre;
}

// Retorna el hijo izquierdo (NULL si no existe)
NodoPtr hijoIzquierdo(NodoPtr pNode) {
    if (pNode == NULL) return NULL;
    return pNode->izquierdo;
}

// Retorna el hijo derecho (NULL si no existe)
NodoPtr hijoDerecho(NodoPtr pNode) {
    if (pNode == NULL) return NULL;
    return pNode->derecho;
}

// ─────────────────────────────────────────────────────────────────────────────
// FUNCIONES DE MODIFICACIÓN
// ─────────────────────────────────────────────────────────────────────────────

// Agrega un hijo izquierdo con valor x a pNode
// Si ya tenía hijo izquierdo, este se reemplaza (¡no se libera!)
void asignarHijoIzquierdo(NodoPtr pNode, int x) {
    if (pNode == NULL) return;
    NodoPtr nuevo = crearArbol(x);
    nuevo->padre  = pNode;
    pNode->izquierdo    = nuevo;
}

// Agrega un hijo derecho con valor x a pNode
void asignarHijoDerecho(NodoPtr pNode, int x) {
    if (pNode == NULL) return;
    NodoPtr nuevo = crearArbol(x);
    nuevo->padre  = pNode;
    pNode->derecho   = nuevo;
}

// ─────────────────────────────────────────────────────────────────────────────
// FUNCIONES DE CLASIFICACIÓN
// ─────────────────────────────────────────────────────────────────────────────

// ¿Es pNode el hijo IZQUIERDO de su padre?
bool esHijoIzquierdo(NodoPtr pNode) {
    if (pNode == NULL || pNode->padre == NULL) return false;
    return pNode->padre->izquierdo == pNode;
}

// ¿Es pNode el hijo DERECHO de su padre?
bool esHijoDerecho(NodoPtr pNode) {
    if (pNode == NULL || pNode->padre == NULL) return false;
    return pNode->padre->derecho == pNode;
}

// Retorna el hermano de pNode (hijo opuesto del mismo padre)
NodoPtr hermano(NodoPtr pNode) {
    if (pNode == NULL || pNode->padre == NULL) return NULL;
    if (esHijoIzquierdo(pNode))
        return pNode->padre->derecho;
    else
        return pNode->padre->izquierdo;
}

// ─────────────────────────────────────────────────────────────────────────────
// INSERCIÓN BÁSICA (para construirEjemplo forma 3)
// ─────────────────────────────────────────────────────────────────────────────
// Inserta x en el árbol respetando orden BST (izq < raíz <= der)
NodoPtr insertar(NodoPtr pNode, int x) {
    if (pNode == NULL) {
        return crearArbol(x);
    }
    if (x < pNode->dato) {
        NodoPtr nuevoIzq = insertar(pNode->izquierdo, x);
        pNode->izquierdo = nuevoIzq;
        nuevoIzq->padre = pNode;
    } else {
        NodoPtr nuevoDer = insertar(pNode->derecho, x);
        pNode->derecho = nuevoDer;
        nuevoDer->padre = pNode;
    }
    return pNode;
}

// ─────────────────────────────────────────────────────────────────────────────
// LIBERAR EL ÁRBOL COMPLETO (postorden)
// ─────────────────────────────────────────────────────────────────────────────
void liberarArbol(NodoPtr pNode) {
    if (pNode == NULL) return;
    liberarArbol(pNode->izquierdo);
    liberarArbol(pNode->derecho);
    freeNode(pNode);
}

// ─────────────────────────────────────────────────────────────────────────────
// VISUALIZACIÓN DEL ÁRBOL (girado 90° — raíz a la izquierda)
// ─────────────────────────────────────────────────────────────────────────────
void imprimirArbol(NodoPtr pNode, int nivel) {
    if (pNode == NULL) return;
    // Primero imprime el subárbol derecho (arriba en pantalla)
    imprimirArbol(pNode->derecho, nivel + 1);
    // Luego imprime este nodo con indentación
    for (int i = 0; i < nivel; i++) cout << "    ";
    cout << "[" << pNode->dato << "]" << endl;
    // Luego imprime el subárbol izquierdo (abajo en pantalla)
    imprimirArbol(pNode->izquierdo, nivel + 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// BUILD123 — TRES FORMAS DE CONSTRUIR EL MISMO ÁRBOL
//
//        2
//       / \  .
//      1   3
// ─────────────────────────────────────────────────────────────────────────────

// FORMA 1: crearArbol + asignación directa de punteros
NodoPtr construirEjemplo_1() {
    NodoPtr two = crearArbol(2);
    two->izquierdo  = crearArbol(1);
    two->derecho = crearArbol(3);
    two->izquierdo->padre  = two;
    two->derecho->padre = two;
    return two;
}

// FORMA 2: Tres variables puntero nombradas explícitamente
NodoPtr construirEjemplo_2() {
    NodoPtr one   = crearArbol(1);
    NodoPtr two   = crearArbol(2);
    NodoPtr three = crearArbol(3);
    two->izquierdo   = one;
    two->derecho  = three;
    one->padre   = two;
    three->padre = two;
    return two;
}

// FORMA 3: Usando insertar() recursivo (BST)
NodoPtr construirEjemplo_3() {
    NodoPtr root = NULL;
    root = insertar(root, 2);
    root = insertar(root, 1);
    root = insertar(root, 3);
    return root;
}

// ─────────────────────────────────────────────────────────────────────────────
// VERIFICAR BUILD123
// ─────────────────────────────────────────────────────────────────────────────
void verificarBuild123(NodoPtr root, const char *nombre) {
    cout << "\n--- " << nombre << " ---" << endl;
    cout << "Estructura del arbol:" << endl;
    imprimirArbol(root, 0);
    cout << endl;
    cout << "Raiz         : " << root->dato << endl;
    cout << "Hijo izq     : " << hijoIzquierdo(root)->dato  << endl;
    cout << "Hijo der     : " << hijoDerecho(root)->dato << endl;
    cout << "Padre del 1  : " << padre(hijoIzquierdo(root))->dato  << endl;
    cout << "Padre del 3  : " << padre(hijoDerecho(root))->dato << endl;
    cout << "esHijoIzquierdo(1)    : " << (esHijoIzquierdo(hijoIzquierdo(root))  ? "SI" : "NO") << endl;
    cout << "esHijoDerecho(3)   : " << (esHijoDerecho(hijoDerecho(root)) ? "SI" : "NO") << endl;
    cout << "Hermano de 1 : " << hermano(hijoIzquierdo(root))->dato  << endl;
    cout << "Hermano de 3 : " << hermano(hijoDerecho(root))->dato << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// EJEMPLO: ÁRBOL MÁS COMPLEJO (5 nodos)
//
//           4
//          / \  .
//         2   5
//        / \  .
//       1   3
// ─────────────────────────────────────────────────────────────────────────────
void ejemploArbolComplejo() {
    cout << "\n=== ARBOL COMPLEJO (5 nodos) ===" << endl;

    NodoPtr raiz = crearArbol(4);
    asignarHijoIzquierdo(raiz, 2);
    asignarHijoDerecho(raiz, 5);
    asignarHijoIzquierdo(raiz->izquierdo, 1);
    asignarHijoDerecho(raiz->izquierdo, 3);

    cout << "Estructura:" << endl;
    imprimirArbol(raiz, 0);
    cout << endl;

    NodoPtr nodo2 = raiz->izquierdo;
    NodoPtr nodo1 = nodo2->izquierdo;

    cout << "Padre de nodo 2 : " << padre(nodo2)->dato << endl;
    cout << "Padre de nodo 1 : " << padre(nodo1)->dato << endl;
    cout << "Hermano de nodo 1 (es el 3): " << hermano(nodo1)->dato << endl;
    cout << "esHijoIzquierdo(nodo 2)  : " << (esHijoIzquierdo(nodo2)  ? "SI" : "NO") << endl;
    cout << "esHijoDerecho(nodo 5) : " << (esHijoDerecho(raiz->derecho) ? "SI" : "NO") << endl;

    liberarArbol(raiz);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  EJERCICIO 01: ARBOL BINARIO BASICO" << endl;
    cout << "============================================" << endl;

    // ─── construirEjemplo en tres formas ───────────────────────────────────────────
    cout << "\n=== BUILD123 — TRES FORMAS ===" << endl;

    NodoPtr t1 = construirEjemplo_1();
    verificarBuild123(t1, "Forma 1: crearArbol + asignacion directa");
    liberarArbol(t1);

    NodoPtr t2 = construirEjemplo_2();
    verificarBuild123(t2, "Forma 2: tres variables puntero");
    liberarArbol(t2);

    NodoPtr t3 = construirEjemplo_3();
    verificarBuild123(t3, "Forma 3: usando insertar()");
    liberarArbol(t3);

    // ─── Árbol más complejo ────────────────────────────────────────────────
    ejemploArbolComplejo();

    cout << "\n============================================" << endl;
    cout << "  FIN DEL EJERCICIO 01" << endl;
    cout << "============================================" << endl;

    return 0;
}

