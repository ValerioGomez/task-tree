// ============================================================================
// REPASO: PUNTEROS EN C++
// Archivo: repaso/punteros/punteros_ejemplos.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic punteros_ejemplos.cpp -o punteros_ej
// ============================================================================
//
// Temas cubiertos:
//   1. Punteros básicos: declaración, asignación, desreferencia
//   2. Aritmética de punteros
//   3. Punteros y arreglos
//   4. Punteros dobles (puntero a puntero)
//   5. Punteros a structs (el -> operador)
//   6. Punteros NULL y verificación
//   7. Asignación dinámica de memoria con malloc/free
// ============================================================================

#include <iostream>
#include <cstdlib>   // malloc, free
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Estructura auxiliar para los ejemplos
// ─────────────────────────────────────────────────────────────────────────────
struct Nodo {
    int dato;
    struct Nodo *siguiente;
};

typedef struct Nodo *NODOPTR;

// ─────────────────────────────────────────────────────────────────────────────
// 1. PUNTEROS BÁSICOS
// ─────────────────────────────────────────────────────────────────────────────
void ejemplo_basico() {
    cout << "\n=== 1. PUNTEROS BASICOS ===" << endl;

    int x = 42;
    int *p = &x;          // p almacena la DIRECCION de x

    cout << "Valor de x      : " << x   << endl;
    cout << "Direccion de x  : " << &x  << endl;
    cout << "Valor de p      : " << p   << endl;  // misma direccion
    cout << "Valor de *p     : " << *p  << endl;  // desreferencia = 42

    // Modificar x a través del puntero
    *p = 100;
    cout << "Despues de *p=100, x = " << x << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. ARITMÉTICA DE PUNTEROS
// ─────────────────────────────────────────────────────────────────────────────
void ejemplo_aritmetica() {
    cout << "\n=== 2. ARITMETICA DE PUNTEROS ===" << endl;

    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;           // p apunta al primer elemento

    cout << "p[0] = " << *p     << endl;  // 10
    p++;
    cout << "p[1] = " << *p     << endl;  // 20
    p += 2;
    cout << "p[3] = " << *p     << endl;  // 40

    // Recorrido con punteros
    cout << "Array completo: ";
    for (int *ptr = arr; ptr < arr + 5; ptr++) {
        cout << *ptr << " ";
    }
    cout << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. PUNTEROS DOBLES (puntero a puntero)
// ─────────────────────────────────────────────────────────────────────────────
void ejemplo_doble_puntero() {
    cout << "\n=== 3. PUNTEROS DOBLES ===" << endl;

    int x = 5;
    int *p  = &x;    // puntero a int
    int **pp = &p;   // puntero a puntero a int

    cout << "x    = " << x    << endl;
    cout << "*p   = " << *p   << endl;
    cout << "**pp = " << **pp << endl;

    // Modificar x a través de pp
    **pp = 999;
    cout << "Despues de **pp=999, x = " << x << endl;

    // Uso típico: cambiar un puntero desde una función
    // (la función recibe int** para poder modificar el int*)
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. PUNTEROS A STRUCTS
// ─────────────────────────────────────────────────────────────────────────────
void ejemplo_puntero_struct() {
    cout << "\n=== 4. PUNTEROS A STRUCTS ===" << endl;

    // Asignación en stack
    struct Nodo n;
    n.dato = 7;
    n.siguiente = NULL;

    struct Nodo *p = &n;

    // Dos formas equivalentes de acceder a miembros:
    cout << "(*p).dato  = " << (*p).dato  << endl;  // forma 1
    cout << "p->dato    = " << p->dato    << endl;  // forma 2 (más común)

    // Modificar a través del puntero
    p->dato = 99;
    cout << "Despues p->dato=99, n.dato = " << n.dato << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 5. MEMORIA DINÁMICA CON malloc/free
// ─────────────────────────────────────────────────────────────────────────────
void ejemplo_malloc() {
    cout << "\n=== 5. MEMORIA DINAMICA (malloc/free) ===" << endl;

    // Crear un nodo dinámicamente
    NODOPTR n = (NODOPTR)malloc(sizeof(struct Nodo));
    if (n == NULL) {
        cout << "ERROR: malloc fallo" << endl;
        return;
    }
    n->dato = 42;
    n->siguiente = NULL;
    cout << "Nodo creado con dato = " << n->dato << endl;

    // Crear una lista enlazada de 3 nodos
    NODOPTR cabeza = (NODOPTR)malloc(sizeof(struct Nodo));
    NODOPTR medio  = (NODOPTR)malloc(sizeof(struct Nodo));
    NODOPTR cola   = (NODOPTR)malloc(sizeof(struct Nodo));

    cabeza->dato = 1;  cabeza->siguiente = medio;
    medio->dato  = 2;  medio->siguiente  = cola;
    cola->dato   = 3;  cola->siguiente   = NULL;

    // Recorrer la lista
    cout << "Lista enlazada: ";
    NODOPTR actual = cabeza;
    while (actual != NULL) {
        cout << actual->dato;
        if (actual->siguiente != NULL) cout << " -> ";
        actual = actual->siguiente;
    }
    cout << endl;

    // SIEMPRE liberar la memoria
    free(cola);
    free(medio);
    free(cabeza);
    free(n);
    cout << "Memoria liberada correctamente." << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 6. PUNTEROS NULL — verificación antes de usar
// ─────────────────────────────────────────────────────────────────────────────
void ejemplo_null() {
    cout << "\n=== 6. PUNTEROS NULL ===" << endl;

    int *p = NULL;

    if (p == NULL) {
        cout << "p es NULL: no se puede desreferenciar" << endl;
    }

    // Asignar y usar
    int x = 10;
    p = &x;

    if (p != NULL) {
        cout << "Ahora p apunta a: " << *p << endl;
    }

    // Regla de oro: SIEMPRE verificar NULL antes de usar un puntero
    // que viene de malloc o que puede no haberse inicializado.
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  REPASO DE PUNTEROS EN C++98" << endl;
    cout << "============================================" << endl;

    ejemplo_basico();
    ejemplo_aritmetica();
    ejemplo_doble_puntero();
    ejemplo_puntero_struct();
    ejemplo_malloc();
    ejemplo_null();

    cout << "\n============================================" << endl;
    cout << "  FIN DEL REPASO DE PUNTEROS" << endl;
    cout << "============================================" << endl;

    return 0;
}

