#include <iostream>

#define ORDEN_BPLUS 4
#define MAX_LLAVES_BPLUS (ORDEN_BPLUS - 1)

// Nodo interno (solo llaves guía)
template <typename T>
struct NodoInternoBPlus {
    int num_llaves;
    T llaves_guia[MAX_LLAVES_BPLUS];     // SOLO llaves, NO datos
    NodoInternoBPlus<T>* hijos[ORDEN_BPLUS];
    NodoInternoBPlus<T>* padre;
    
    NodoInternoBPlus() : num_llaves(0), padre(NULL) {
        for(int i = 0; i < ORDEN_BPLUS; i++) {
            hijos[i] = NULL;
        }
    }
};

// Nodo hoja (contiene datos y puntero al siguiente)
template <typename T>
struct NodoHojaBPlus {
    int num_llaves;
    T datos[MAX_LLAVES_BPLUS];           // DATOS completos
    NodoHojaBPlus<T>* siguiente;          // Puntero para lista enlazada
    NodoHojaBPlus<T>* anterior;           // Puntero para lista doblemente enlazada
    NodoInternoBPlus<T>* padre;           // Referencia al padre interno
    
    NodoHojaBPlus() : num_llaves(0), siguiente(NULL), anterior(NULL), padre(NULL) {}
};

// Árbol B+
template <typename T>
class ArbolBPlus {
private:
    NodoInternoBPlus<T>* raiz;
    NodoHojaBPlus<T>* primera_hoja;      // Para recorridos secuenciales
    int orden;
    
public:
    ArbolBPlus(int m = ORDEN_BPLUS) : raiz(NULL), primera_hoja(NULL), orden(m) {}
    
    // Operaciones
    T* buscar(T clave);
    T* buscarRango(T inicio, T fin);     // QUERY BETWEEN
    void insertar(T dato);
    void eliminar(T dato);
    void recorrerHojas();                 // Recorrido secuencial
    
private:
    void insertarEnHoja(NodoHojaBPlus<T>* hoja, T dato);
    void dividirHoja(NodoHojaBPlus<T>* hoja);
    void dividirInterno(NodoInternoBPlus<T>* nodo);
    NodoHojaBPlus<T>* buscarHoja(T clave);
};

// Simulación de query BETWEEN
void simulacionPostgreSQL() {
    ArbolBPlus<int> indice;
    
    // Insertar datos
    for(int i = 1; i <= 1000; i++) {
        // indice.insertar(i);
    }
    
    // Query: SELECT * FROM tabla WHERE id BETWEEN 150 AND 250
    // int* resultados = indice.buscarRango(150, 250);
    
    // Eficiencia: Solo recorre hojas enlazadas
    // 1. Encuentra hoja con 150 (O(log n))
    // 2. Recorre 'siguiente' punteros hasta 250 (O(k) donde k = 100 elementos)
    // VS Árbol B: Recorre nodos internos para cada elemento (O(k * log n))
    std::cout << "Simulacion PostgreSQL para B+ completada." << std::endl;
}

int main() {
    simulacionPostgreSQL();
    return 0;
}
