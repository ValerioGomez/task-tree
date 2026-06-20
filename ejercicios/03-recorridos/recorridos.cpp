// ============================================================================
// EJERCICIO 03: RECORRIDOS DE ÁRBOLES BINARIOS
// Archivo: ejercicios/03-recorridos/recorridos.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic recorridos.cpp -o recorridos
// ============================================================================
//
// Recorridos implementados:
//   1. PREORDEN   (Nodo - Izquierda - Derecha):  raíz primero
//   2. INORDEN    (Izquierda - Nodo - Derecha):  orden ascendente en BST
//   3. POSTORDEN  (Izquierda - Derecha - Nodo):  hijos antes que padre
//   4. ANCHURA    (BFS, nivel por nivel):         usa cola auxiliar
//
// Árbol de prueba:
//
//              15
//            /    \  .
//           4      19
//          / \    /
//         3   8  16
//            / \   \  .
//           5   9  17
//
// ============================================================================

#include <iostream>
#include <cstdlib>
#include <queue>      // std::queue — parte del estándar C++98
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

NodoPtr insertar(NodoPtr p, int x) {
    if (p == NULL) return crearArbol(x);
    if (x < p->dato) {
        NodoPtr izq = insertar(p->izquierdo, x);
        p->izquierdo      = izq;
        izq->padre  = p;
    } else {
        NodoPtr der = insertar(p->derecho, x);
        p->derecho     = der;
        der->padre  = p;
    }
    return p;
}

void liberarArbol(NodoPtr p) {
    if (p == NULL) return;
    liberarArbol(p->izquierdo);
    liberarArbol(p->derecho);
    free(p);
}

// ─────────────────────────────────────────────────────────────────────────────
// 1. PREORDEN: Nodo → Izquierda → Derecha
//    Uso: copiar un árbol, serializar, prefijo de expresiones
// ─────────────────────────────────────────────────────────────────────────────
void recorridoPreorden(NodoPtr pNode) {
    if (pNode != NULL) {
        cout << pNode->dato << " ";          // 1. Visitar nodo
        recorridoPreorden(pNode->izquierdo);      // 2. Subárbol izquierdo
        recorridoPreorden(pNode->derecho);     // 3. Subárbol derecho
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. INORDEN: Izquierda → Nodo → Derecha
//    Uso: recorrer BST en orden ascendente
// ─────────────────────────────────────────────────────────────────────────────
void recorridoInorden(NodoPtr pNode) {
    if (pNode != NULL) {
        recorridoInorden(pNode->izquierdo);       // 1. Subárbol izquierdo
        cout << pNode->dato << " ";          // 2. Visitar nodo
        recorridoInorden(pNode->derecho);      // 3. Subárbol derecho
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. POSTORDEN: Izquierda → Derecha → Nodo
//    Uso: liberar árbol, evaluar expresiones, sufijo
// ─────────────────────────────────────────────────────────────────────────────
void recorridoPostorden(NodoPtr pNode) {
    if (pNode != NULL) {
        recorridoPostorden(pNode->izquierdo);     // 1. Subárbol izquierdo
        recorridoPostorden(pNode->derecho);    // 2. Subárbol derecho
        cout << pNode->dato << " ";          // 3. Visitar nodo
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. RECORRIDO EN ANCHURA (BFS — Breadth-First Search)
//    Visita nivel por nivel, de izquierda a derecha
//    Usa una cola: cada nodo encola a sus hijos antes de procesarse
// ─────────────────────────────────────────────────────────────────────────────
void recorridoAnchura(NodoPtr root) {
    if (root == NULL) return;

    queue<NodoPtr> cola;
    cola.push(root);

    while (!cola.empty()) {
        NodoPtr actual = cola.front();
        cola.pop();

        cout << actual->dato << " ";

        // Encolar hijos (izquierdo primero)
        if (actual->izquierdo  != NULL) cola.push(actual->izquierdo);
        if (actual->derecho != NULL) cola.push(actual->derecho);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 4B. RECORRIDO EN ANCHURA CON NIVELES VISIBLES
// ─────────────────────────────────────────────────────────────────────────────
void recorridoAnchuraNiveles(NodoPtr root) {
    if (root == NULL) return;

    queue<NodoPtr> cola;
    cola.push(root);
    int nivel = 0;

    while (!cola.empty()) {
        int tamNivel = (int)cola.size();   // nodos en el nivel actual
        cout << "  Nivel " << nivel << ": ";

        for (int i = 0; i < tamNivel; i++) {
            NodoPtr actual = cola.front();
            cola.pop();
            cout << actual->dato << " ";
            if (actual->izquierdo  != NULL) cola.push(actual->izquierdo);
            if (actual->derecho != NULL) cola.push(actual->derecho);
        }
        cout << endl;
        nivel++;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// FUNCIONES ADICIONALES QUE USAN RECORRIDOS
// ─────────────────────────────────────────────────────────────────────────────

// Contar nodos (usa postorden)
int contarNodos(NodoPtr p) {
    if (p == NULL) return 0;
    return 1 + contarNodos(p->izquierdo) + contarNodos(p->derecho);
}

// Calcular altura del árbol (máxima profundidad)
int altura(NodoPtr p) {
    if (p == NULL) return -1;               // árbol vacío tiene altura -1
    int hIzq = altura(p->izquierdo);
    int hDer = altura(p->derecho);
    return 1 + (hIzq > hDer ? hIzq : hDer);
}

// Contar hojas (nodos sin hijos)
int contarHojas(NodoPtr p) {
    if (p == NULL) return 0;
    if (p->izquierdo == NULL && p->derecho == NULL) return 1;   // es hoja
    return contarHojas(p->izquierdo) + contarHojas(p->derecho);
}

// Suma de todos los valores
int sumaValores(NodoPtr p) {
    if (p == NULL) return 0;
    return p->dato + sumaValores(p->izquierdo) + sumaValores(p->derecho);
}

// Buscar la profundidad de un valor (cuántos niveles desde la raíz)
int profundidad(NodoPtr p, int valor, int nivel) {
    if (p == NULL) return -1;
    if (p->dato == valor) return nivel;
    int izq = profundidad(p->izquierdo,  valor, nivel + 1);
    if (izq != -1) return izq;
    return profundidad(p->derecho, valor, nivel + 1);
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
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  EJERCICIO 03: RECORRIDOS DE ARBOLES" << endl;
    cout << "============================================" << endl;

    // ─── Construir el árbol de prueba ────────────────────────────────────
    // Insertar: {15, 4, 8, 7, 3, 19, 5, 9, 16, 17}
    int nums[] = {15, 4, 8, 7, 3, 19, 5, 9, 16, 17};
    int n = 10;

    NodoPtr tree = NULL;
    for (int i = 0; i < n; i++) {
        tree = insertar(tree, nums[i]);
    }

    cout << "\nArbol de prueba (girado 90 grados):" << endl;
    cout << "(derecha arriba = mayor, izquierda abajo = menor)" << endl << endl;
    imprimirArbol(tree, 0);

    // ─── Los 4 recorridos ────────────────────────────────────────────────
    cout << "\n=== 1. PREORDEN (Nodo - Izq - Der) ===" << endl;
    cout << "Resultado: ";
    recorridoPreorden(tree);
    cout << endl;
    cout << "Uso: serializar el arbol, expresiones en notacion prefija" << endl;

    cout << "\n=== 2. INORDEN (Izq - Nodo - Der) ===" << endl;
    cout << "Resultado: ";
    recorridoInorden(tree);
    cout << endl;
    cout << "Uso: recorrer BST en orden ascendente, ordenar" << endl;

    cout << "\n=== 3. POSTORDEN (Izq - Der - Nodo) ===" << endl;
    cout << "Resultado: ";
    recorridoPostorden(tree);
    cout << endl;
    cout << "Uso: liberar memoria, evaluar expresiones, notacion posfija" << endl;

    cout << "\n=== 4. ANCHURA - BFS (nivel por nivel) ===" << endl;
    cout << "Resultado: ";
    recorridoAnchura(tree);
    cout << endl;
    cout << "Uso: encontrar el camino mas corto, impresion por niveles" << endl;

    cout << "\n=== 4B. ANCHURA CON NIVELES VISIBLES ===" << endl;
    recorridoAnchuraNiveles(tree);

    // ─── Métricas del árbol ──────────────────────────────────────────────
    cout << "\n=== METRICAS DEL ARBOL ===" << endl;
    cout << "Total de nodos : " << contarNodos(tree) << endl;
    cout << "Altura         : " << altura(tree)      << " (niveles - 1)" << endl;
    cout << "Total de hojas : " << contarHojas(tree) << endl;
    cout << "Suma de valores: " << sumaValores(tree) << endl;

    int buscar = 8;
    cout << "Profundidad de " << buscar << ": "
         << profundidad(tree, buscar, 0) << " (niveles desde raiz)" << endl;

    // ─── Comparar los 4 recorridos ───────────────────────────────────────
    cout << "\n=== RESUMEN DE RECORRIDOS ===" << endl;
    cout << "Preorden : "; recorridoPreorden(tree);  cout << endl;
    cout << "Inorden  : "; recorridoInorden(tree);   cout << endl;
    cout << "Postorden: "; recorridoPostorden(tree); cout << endl;
    cout << "Anchura  : "; recorridoAnchura(tree);   cout << endl;

    liberarArbol(tree);

    cout << "\n============================================" << endl;
    cout << "  FIN DEL EJERCICIO 03" << endl;
    cout << "============================================" << endl;

    return 0;
}

