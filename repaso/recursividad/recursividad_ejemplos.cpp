// ============================================================================
// REPASO: RECURSIVIDAD EN C++
// Archivo: repaso/recursividad/recursividad_ejemplos.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic recursividad_ejemplos.cpp -o recursividad_ej
// ============================================================================
//
// Temas cubiertos:
//   1. Factorial (recursión lineal)
//   2. Fibonacci (recursión múltiple)
//   3. Torres de Hanói
//   4. Suma de elementos de un arreglo
//   5. Búsqueda binaria recursiva
//   6. Potencia recursiva (dividir y conquistar)
//   7. Anticipos: funciones sobre listas enlazadas (base para árboles)
// ============================================================================

#include <iostream>
#include <cstdlib>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// 1. FACTORIAL
//    n! = n * (n-1)!     caso base: 0! = 1
// ─────────────────────────────────────────────────────────────────────────────
int factorial(int n) {
    if (n <= 1) return 1;          // caso base
    return n * factorial(n - 1);  // llamada recursiva
}

void ejemplo_factorial() {
    cout << "\n=== 1. FACTORIAL ===" << endl;
    for (int i = 0; i <= 10; i++) {
        cout << i << "! = " << factorial(i) << endl;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. FIBONACCI
//    fib(n) = fib(n-1) + fib(n-2)    casos base: fib(0)=0, fib(1)=1
//    NOTA: esta versión es O(2^n) — se muestra por claridad, no por eficiencia
// ─────────────────────────────────────────────────────────────────────────────
int fibonacci(int n) {
    if (n <= 0) return 0;                                    // caso base 1
    if (n == 1) return 1;                                    // caso base 2
    return fibonacci(n - 1) + fibonacci(n - 2);             // doble recursión
}

void ejemplo_fibonacci() {
    cout << "\n=== 2. FIBONACCI (primeros 15) ===" << endl;
    cout << "Secuencia: ";
    for (int i = 0; i < 15; i++) {
        cout << fibonacci(i);
        if (i < 14) cout << ", ";
    }
    cout << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. TORRES DE HANÓI
//    Mover n discos de 'origen' a 'destino' usando 'auxiliar'
// ─────────────────────────────────────────────────────────────────────────────
int hanoi_movimientos = 0;

void hanoi(int n, char origen, char destino, char auxiliar) {
    if (n == 1) {
        cout << "  Mover disco 1 de " << origen << " a " << destino << endl;
        hanoi_movimientos++;
        return;
    }
    // 1. Mover n-1 discos del origen al auxiliar
    hanoi(n - 1, origen, auxiliar, destino);
    // 2. Mover el disco más grande al destino
    cout << "  Mover disco " << n << " de " << origen << " a " << destino << endl;
    hanoi_movimientos++;
    // 3. Mover n-1 discos del auxiliar al destino
    hanoi(n - 1, auxiliar, destino, origen);
}

void ejemplo_hanoi() {
    cout << "\n=== 3. TORRES DE HANOI (3 discos) ===" << endl;
    hanoi_movimientos = 0;
    hanoi(3, 'A', 'C', 'B');
    cout << "Total de movimientos: " << hanoi_movimientos
         << " (2^3 - 1 = 7)" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. SUMA DE ARREGLO (recursiva)
// ─────────────────────────────────────────────────────────────────────────────
int sumaArreglo(int arr[], int n) {
    if (n <= 0) return 0;                           // caso base: arreglo vacío
    return arr[n - 1] + sumaArreglo(arr, n - 1);   // último + suma del resto
}

void ejemplo_suma_arreglo() {
    cout << "\n=== 4. SUMA DE ARREGLO ===" << endl;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = 10;
    cout << "Arreglo: {1, 2, ..., 10}" << endl;
    cout << "Suma recursiva = " << sumaArreglo(arr, n) << " (esperado: 55)" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 5. BÚSQUEDA BINARIA RECURSIVA
//    Precondición: el arreglo debe estar ordenado
// ─────────────────────────────────────────────────────────────────────────────
int busquedaBinaria(int arr[], int izq, int der, int objetivo) {
    if (izq > der) return -1;                    // caso base: no encontrado

    int medio = (izq + der) / 2;

    if (arr[medio] == objetivo)
        return medio;                            // caso base: encontrado
    else if (arr[medio] < objetivo)
        return busquedaBinaria(arr, medio + 1, der, objetivo);   // mitad derecha
    else
        return busquedaBinaria(arr, izq, medio - 1, objetivo);   // mitad izquierda
}

void ejemplo_busqueda_binaria() {
    cout << "\n=== 5. BUSQUEDA BINARIA RECURSIVA ===" << endl;
    int arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    int n = 10;

    int buscar1 = 23;
    int buscar2 = 50;

    int idx1 = busquedaBinaria(arr, 0, n - 1, buscar1);
    int idx2 = busquedaBinaria(arr, 0, n - 1, buscar2);

    if (idx1 != -1)
        cout << buscar1 << " encontrado en indice " << idx1 << endl;
    else
        cout << buscar1 << " NO encontrado" << endl;

    if (idx2 != -1)
        cout << buscar2 << " encontrado en indice " << idx2 << endl;
    else
        cout << buscar2 << " NO encontrado" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 6. POTENCIA RECURSIVA — dividir y conquistar
//    base^exp = (base^(exp/2))^2          si exp es par
//    base^exp = base * base^(exp-1)       si exp es impar
// ─────────────────────────────────────────────────────────────────────────────
long long potencia(long long base, int exp) {
    if (exp == 0) return 1;                       // caso base
    if (exp % 2 == 0) {
        long long mitad = potencia(base, exp / 2);
        return mitad * mitad;                     // dividir y conquistar
    } else {
        return base * potencia(base, exp - 1);
    }
}

void ejemplo_potencia() {
    cout << "\n=== 6. POTENCIA (dividir y conquistar) ===" << endl;
    cout << "2^10  = " << potencia(2, 10)  << " (esperado: 1024)"   << endl;
    cout << "3^5   = " << potencia(3, 5)   << " (esperado: 243)"    << endl;
    cout << "5^0   = " << potencia(5, 0)   << " (esperado: 1)"      << endl;
    cout << "10^6  = " << potencia(10, 6)  << " (esperado: 1000000)" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 7. ANTICIPOS: RECURSIÓN SOBRE LISTAS ENLAZADAS
//    Estos patrones se usan EXACTAMENTE igual en árboles
// ─────────────────────────────────────────────────────────────────────────────
struct NodoLista {
    int dato;
    struct NodoLista *sig;
};
typedef struct NodoLista *ListaPtr;

// Contar nodos recursivamente
int contarNodos(ListaPtr p) {
    if (p == NULL) return 0;               // caso base
    return 1 + contarNodos(p->sig);        // 1 + resto
}

// Imprimir lista recursivamente
void imprimirLista(ListaPtr p) {
    if (p == NULL) {
        cout << "NULL" << endl;
        return;
    }
    cout << p->dato << " -> ";
    imprimirLista(p->sig);                 // llamada recursiva
}

// Suma de todos los elementos
int sumaLista(ListaPtr p) {
    if (p == NULL) return 0;
    return p->dato + sumaLista(p->sig);
}

void ejemplo_listas_recursivas() {
    cout << "\n=== 7. RECURSION SOBRE LISTAS (patron para arboles) ===" << endl;

    // Crear lista: 5 -> 10 -> 15 -> 20 -> NULL
    ListaPtr n1 = (ListaPtr)malloc(sizeof(struct NodoLista));
    ListaPtr n2 = (ListaPtr)malloc(sizeof(struct NodoLista));
    ListaPtr n3 = (ListaPtr)malloc(sizeof(struct NodoLista));
    ListaPtr n4 = (ListaPtr)malloc(sizeof(struct NodoLista));

    n1->dato = 5;  n1->sig = n2;
    n2->dato = 10; n2->sig = n3;
    n3->dato = 15; n3->sig = n4;
    n4->dato = 20; n4->sig = NULL;

    cout << "Lista: ";
    imprimirLista(n1);
    cout << "Cantidad de nodos: " << contarNodos(n1) << endl;
    cout << "Suma de elementos: " << sumaLista(n1)   << endl;

    // NOTA: compare estos patrones con las funciones de árbol:
    //   if (p == NULL) return 0;          <- caso base para nodo nulo
    //   return f(p->izquierdo) + f(p->derecho);  <- llamadas recursivas izq y der

    free(n1); free(n2); free(n3); free(n4);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  REPASO DE RECURSIVIDAD EN C++98" << endl;
    cout << "============================================" << endl;

    ejemplo_factorial();
    ejemplo_fibonacci();
    ejemplo_hanoi();
    ejemplo_suma_arreglo();
    ejemplo_busqueda_binaria();
    ejemplo_potencia();
    ejemplo_listas_recursivas();

    cout << "\n============================================" << endl;
    cout << "  FIN DEL REPASO DE RECURSIVIDAD" << endl;
    cout << "============================================" << endl;

    return 0;
}

