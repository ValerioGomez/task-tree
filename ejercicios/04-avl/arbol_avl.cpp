// ============================================================================
// EJERCICIO 04: ÁRBOL AVL (BALANCEADO)
// Archivo: ejercicios/04-avl/arbol_avl.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic arbol_avl.cpp -o arbol_avl
// ============================================================================
//
// Un árbol AVL es un BST en el que, para CADA nodo, la diferencia de alturas
// entre su subárbol izquierdo y su subárbol derecho (factor de equilibrio) es:
//   -1, 0, o +1.
//
// Si se viola esta propiedad tras una inserción, se aplica una rotación para
// restaurar el balance.
//
// Factor de equilibrio (FE):
//   FE = altura(subárbol_derecho) - altura(subárbol_izquierdo)
//   FE = -1  → izquierdo más alto (1 nivel)
//   FE =  0  → perfectamente balanceado
//   FE = +1  → derecho más alto (1 nivel)
//
// Rotaciones:
//   1. Rotación Simple Derecha     (desequilibrio LL: FE <= -2)
//   2. Rotación Simple Izquierda   (desequilibrio RR: FE >= +2)
//   3. Rotación Doble Izq-Der      (desequilibrio LR)
//   4. Rotación Doble Der-Izq      (desequilibrio RL)
//
// ============================================================================

#include <iostream>
#include <cstdlib>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// ESTRUCTURA DEL NODO AVL
// ─────────────────────────────────────────────────────────────────────────────
struct avl_node {
    int dato;
    int altura;                     // altura del subárbol con raíz en este nodo
    struct avl_node *padre;
    struct avl_node *izquierdo;
    struct avl_node *derecho;
};

typedef struct avl_node *AVLNODEPTR;

// ─────────────────────────────────────────────────────────────────────────────
// PRIMITIVAS
// ─────────────────────────────────────────────────────────────────────────────
AVLNODEPTR crearNodoAVL(int x) {
    AVLNODEPTR p = (AVLNODEPTR)malloc(sizeof(struct avl_node));
    p->dato    = x;
    p->altura  = 0;          // nodo hoja: altura 0
    p->padre  = NULL;
    p->izquierdo    = NULL;
    p->derecho   = NULL;
    return p;
}

void liberarArbolAVL(AVLNODEPTR p) {
    if (p == NULL) return;
    liberarArbolAVL(p->izquierdo);
    liberarArbolAVL(p->derecho);
    free(p);
}

// ─────────────────────────────────────────────────────────────────────────────
// ALTURA Y FACTOR DE EQUILIBRIO
// ─────────────────────────────────────────────────────────────────────────────

// Retorna la altura almacenada (-1 para nodo NULL)
int getAltura(AVLNODEPTR p) {
    if (p == NULL) return -1;
    return p->altura;
}

// Recalcula y almacena la altura del nodo
void actualizarAltura(AVLNODEPTR p) {
    if (p == NULL) return;
    int hIzq = getAltura(p->izquierdo);
    int hDer = getAltura(p->derecho);
    p->altura = 1 + (hIzq > hDer ? hIzq : hDer);
}

// Factor de equilibrio: FE = h(derecho) - h(izquierdo)
int factorEquilibrio(AVLNODEPTR p) {
    if (p == NULL) return 0;
    return getAltura(p->derecho) - getAltura(p->izquierdo);
}

// ─────────────────────────────────────────────────────────────────────────────
// ROTACIONES
//
// Notación: p = nodo desbalanceado, q = hijo que sube
// ─────────────────────────────────────────────────────────────────────────────

// ROTACIÓN SIMPLE DERECHA (LL)
// Aplica cuando FE(p) <= -2 y FE(p->izquierdo) <= 0
//
//       p              q
//      / \            / \  .
//     q   C    →    A     p
//    / \                 / \  .
//   A   B              B     C
//
AVLNODEPTR rotacionDerecha(AVLNODEPTR p) {
    AVLNODEPTR q = p->izquierdo;
    AVLNODEPTR B = q->derecho;

    // Realizar rotación
    q->derecho = p;
    p->izquierdo  = B;

    // Actualizar punteros padre
    q->padre = p->padre;
    p->padre = q;
    if (B != NULL) B->padre = p;

    // Actualizar alturas (p primero porque ahora es hijo de q)
    actualizarAltura(p);
    actualizarAltura(q);

    return q;    // q es la nueva raíz del subárbol
}

// ROTACIÓN SIMPLE IZQUIERDA (RR)
// Aplica cuando FE(p) >= +2 y FE(p->derecho) >= 0
//
//   p                  q
//  / \                / \  .
// A   q     →       p     C
//    / \            / \  .
//   B   C          A   B
//
AVLNODEPTR rotacionIzquierda(AVLNODEPTR p) {
    AVLNODEPTR q = p->derecho;
    AVLNODEPTR B = q->izquierdo;

    // Realizar rotación
    q->izquierdo  = p;
    p->derecho = B;

    // Actualizar punteros padre
    q->padre = p->padre;
    p->padre = q;
    if (B != NULL) B->padre = p;

    // Actualizar alturas
    actualizarAltura(p);
    actualizarAltura(q);

    return q;    // q es la nueva raíz del subárbol
}

// ROTACIÓN DOBLE IZQUIERDA-DERECHA (LR)
// Aplica cuando FE(p) <= -2 y FE(p->izquierdo) > 0
// Primero rota el hijo izquierdo a la izquierda, luego p a la derecha
//
//     p              p              r
//    / \            / \            / \  .
//   q   D   →    r   D    →    q     p
//  / \          / \           / \   / \  .
// A   r        q   C         A   B C   D
//    / \      / \  .
//   B   C    A   B
//
AVLNODEPTR rotacionDobleIzqDer(AVLNODEPTR p) {
    p->izquierdo = rotacionIzquierda(p->izquierdo);   // rotar hijo izquierdo
    if (p->izquierdo != NULL) p->izquierdo->padre = p;
    return rotacionDerecha(p);              // rotar raíz
}

// ROTACIÓN DOBLE DERECHA-IZQUIERDA (RL)
// Aplica cuando FE(p) >= +2 y FE(p->derecho) < 0
// Primero rota el hijo derecho a la derecha, luego p a la izquierda
//
//   p              p                r
//  / \            / \              / \  .
// A   q    →    A   r    →      p     q
//    / \            / \        / \   / \  .
//   r   D          B   q      A   B C   D
//  / \                / \  .
// B   C              C   D
//
AVLNODEPTR rotacionDobleDerIzq(AVLNODEPTR p) {
    p->derecho = rotacionDerecha(p->derecho);   // rotar hijo derecho
    if (p->derecho != NULL) p->derecho->padre = p;
    return rotacionIzquierda(p);            // rotar raíz
}

// ─────────────────────────────────────────────────────────────────────────────
// BALANCEO — aplica la rotación correcta según el factor de equilibrio
// ─────────────────────────────────────────────────────────────────────────────
AVLNODEPTR balancear(AVLNODEPTR p) {
    actualizarAltura(p);
    int fe = factorEquilibrio(p);

    // Desequilibrio hacia la IZQUIERDA (LL o LR)
    if (fe <= -2) {
        if (factorEquilibrio(p->izquierdo) <= 0) {
            cout << "    [Rotacion Simple DERECHA en nodo " << p->dato << "]" << endl;
            p = rotacionDerecha(p);          // caso LL
        } else {
            cout << "    [Rotacion Doble IZQ-DER en nodo " << p->dato << "]" << endl;
            p = rotacionDobleIzqDer(p);      // caso LR
        }
    }
    // Desequilibrio hacia la DERECHA (RR o RL)
    else if (fe >= 2) {
        if (factorEquilibrio(p->derecho) >= 0) {
            cout << "    [Rotacion Simple IZQUIERDA en nodo " << p->dato << "]" << endl;
            p = rotacionIzquierda(p);        // caso RR
        } else {
            cout << "    [Rotacion Doble DER-IZQ en nodo " << p->dato << "]" << endl;
            p = rotacionDobleDerIzq(p);      // caso RL
        }
    }
    return p;
}

// ─────────────────────────────────────────────────────────────────────────────
// INSERCIÓN AVL
// ─────────────────────────────────────────────────────────────────────────────
AVLNODEPTR insertarAVL(AVLNODEPTR p, int x) {
    // Inserción como en BST
    if (p == NULL) {
        return crearNodoAVL(x);
    }
    if (x < p->dato) {
        AVLNODEPTR nuevoIzq = insertarAVL(p->izquierdo, x);
        p->izquierdo = nuevoIzq;
        nuevoIzq->padre = p;
    } else if (x > p->dato) {
        AVLNODEPTR nuevoDer = insertarAVL(p->derecho, x);
        p->derecho = nuevoDer;
        nuevoDer->padre = p;
    } else {
        // Duplicado: no insertar
        return p;
    }

    // Rebalancear en el camino de vuelta
    return balancear(p);
}

// ─────────────────────────────────────────────────────────────────────────────
// VISUALIZACIÓN
// ─────────────────────────────────────────────────────────────────────────────
void imprimirAVL(AVLNODEPTR p, int nivel, char prefijo = 'R') {
    if (p == NULL) return;
    imprimirAVL(p->derecho, nivel + 1, 'd');
    for (int i = 0; i < nivel; i++) cout << "    ";
    
    if (prefijo == 'R')
        cout << "[" << p->dato << " FE:" << factorEquilibrio(p) << "]" << endl;
    else if (prefijo == 'd')
        cout << "[" << p->dato << " FE:" << factorEquilibrio(p) << "]d" << endl;
    else
        cout << "i[" << p->dato << " FE:" << factorEquilibrio(p) << "]" << endl;
        
    imprimirAVL(p->izquierdo, nivel + 1, 'i');
}

void inorden(AVLNODEPTR p) {
    if (p == NULL) return;
    inorden(p->izquierdo);
    cout << p->dato << "(FE=" << factorEquilibrio(p) << ") ";
    inorden(p->derecho);
}

// ─────────────────────────────────────────────────────────────────────────────
// VERIFICACIÓN: todos los FE deben ser -1, 0, o +1
// ─────────────────────────────────────────────────────────────────────────────
bool esAVLValido(AVLNODEPTR p) {
    if (p == NULL) return true;
    int fe = factorEquilibrio(p);
    if (fe < -1 || fe > 1) return false;
    return esAVLValido(p->izquierdo) && esAVLValido(p->derecho);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  EJERCICIO 04: ARBOL AVL (BALANCEADO)" << endl;
    cout << "============================================" << endl;

    AVLNODEPTR tree = NULL;

    // ─── Secuencia que provoca rotaciones ────────────────────────────────
    // Insertar: 10, 20, 30 → provoca rotación RR
    // Luego: 15, 5, 1     → provoca rotaciones diversas
    int secuencia[] = {10, 20, 30, 15, 5, 1, 25, 35, 12, 18};
    int n = 10;

    cout << "\nInsertando secuencia (con rotaciones automaticas):" << endl;

    for (int i = 0; i < n; i++) {
        cout << "\nInsertando " << secuencia[i] << ":" << endl;
        tree = insertarAVL(tree, secuencia[i]);

        cout << "  Arbol actual:" << endl;
        imprimirAVL(tree, 2);
        cout << "  AVL valido: " << (esAVLValido(tree) ? "SI" : "NO") << endl;
    }

    // ─── Estado final ────────────────────────────────────────────────────
    cout << "\n=== ARBOL AVL FINAL ===" << endl;
    imprimirAVL(tree, 0);

    cout << "\nInorden (debe ser ascendente):" << endl;
    inorden(tree);
    cout << endl;

    cout << "\nAltura del arbol: " << getAltura(tree) << endl;
    cout << "Es AVL valido   : " << (esAVLValido(tree) ? "SI" : "NO") << endl;

    // ─── Demostración de las 4 rotaciones ────────────────────────────────
    cout << "\n=== DEMOSTRACION: LAS 4 ROTACIONES ===" << endl;

    // Caso LL (rotación simple derecha): insertar 30, 20, 10
    cout << "\n-- Caso LL (Rotacion Simple Derecha) --" << endl;
    cout << "Insertar: 30, 20, 10" << endl;
    AVLNODEPTR tLL = NULL;
    tLL = insertarAVL(tLL, 30);
    tLL = insertarAVL(tLL, 20);
    tLL = insertarAVL(tLL, 10);
    cout << "Resultado:" << endl;
    imprimirAVL(tLL, 1);
    liberarArbolAVL(tLL);

    // Caso RR (rotación simple izquierda): insertar 10, 20, 30
    cout << "\n-- Caso RR (Rotacion Simple Izquierda) --" << endl;
    cout << "Insertar: 10, 20, 30" << endl;
    AVLNODEPTR tRR = NULL;
    tRR = insertarAVL(tRR, 10);
    tRR = insertarAVL(tRR, 20);
    tRR = insertarAVL(tRR, 30);
    cout << "Resultado:" << endl;
    imprimirAVL(tRR, 1);
    liberarArbolAVL(tRR);

    // Caso LR (rotación doble izq-der): insertar 30, 10, 20
    cout << "\n-- Caso LR (Rotacion Doble Izq-Der) --" << endl;
    cout << "Insertar: 30, 10, 20" << endl;
    AVLNODEPTR tLR = NULL;
    tLR = insertarAVL(tLR, 30);
    tLR = insertarAVL(tLR, 10);
    tLR = insertarAVL(tLR, 20);
    cout << "Resultado:" << endl;
    imprimirAVL(tLR, 1);
    liberarArbolAVL(tLR);

    // Caso RL (rotación doble der-izq): insertar 10, 30, 20
    cout << "\n-- Caso RL (Rotacion Doble Der-Izq) --" << endl;
    cout << "Insertar: 10, 30, 20" << endl;
    AVLNODEPTR tRL = NULL;
    tRL = insertarAVL(tRL, 10);
    tRL = insertarAVL(tRL, 30);
    tRL = insertarAVL(tRL, 20);
    cout << "Resultado:" << endl;
    imprimirAVL(tRL, 1);
    liberarArbolAVL(tRL);

    liberarArbolAVL(tree);

    cout << "\n============================================" << endl;
    cout << "  FIN DEL EJERCICIO 04" << endl;
    cout << "============================================" << endl;

    return 0;
}

