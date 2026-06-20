#include <iostream>

// Árbol B de orden m (máximo m hijos por nodo)
#define ORDEN_B 3  // m = 3, mínimo 2 hijos por nodo (excepto raíz)
#define MAX_LLAVES_B (ORDEN_B - 1)  // 2 llaves por nodo

template <typename T>
struct NodoB {
    int num_llaves;                          // Cantidad actual de llaves
    T llaves[MAX_LLAVES_B];                 // Datos completos en TODOS los nodos
    NodoB<T>* hijos[ORDEN_B];               // Punteros a hijos
    NodoB<T>* padre;                        // Puntero al padre
    bool es_hoja;                           // Flag: true si es hoja
    
    NodoB() : num_llaves(0), padre(NULL), es_hoja(true) {
        for(int i = 0; i < ORDEN_B; i++) {
            hijos[i] = NULL;
        }
    }
};

template <typename T>
class ArbolB {
private:
    NodoB<T>* raiz;
    int orden;
    
public:
    ArbolB(int m = ORDEN_B) : raiz(NULL), orden(m) {}
    
    // Operaciones básicas
    T* buscar(T clave);
    void insertar(T clave);
    void eliminar(T clave);
    void recorrer();
    
private:
    void dividirNodo(NodoB<T>* nodo);
    void insertarNoLleno(NodoB<T>* nodo, T clave);
    void eliminarDeNodo(NodoB<T>* nodo, T clave);
    void prestarDeHermano(NodoB<T>* nodo, int idx);
    void fusionarNodos(NodoB<T>* nodo, int idx);
};

int main() {
    std::cout << "Implementacion de Arbol B Clasico" << std::endl;
    return 0;
}
