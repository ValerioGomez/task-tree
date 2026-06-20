#include <iostream>

#define ORDEN_BSTAR 5
#define MAX_LLAVES_BSTAR (ORDEN_BSTAR - 1)
#define MIN_LLAVES_BSTAR ((2 * ORDEN_BSTAR - 2) / 3)  // 2/3 de ocupación

template <typename T>
struct NodoBStar {
    int num_llaves;
    T llaves[MAX_LLAVES_BSTAR];
    NodoBStar<T>* hijos[ORDEN_BSTAR];
    NodoBStar<T>* padre;
    NodoBStar<T>* hermano_izq;
    NodoBStar<T>* hermano_der;
    bool es_hoja;
    
    NodoBStar() : num_llaves(0), padre(NULL), 
                  hermano_izq(NULL), hermano_der(NULL), es_hoja(true) {
        for(int i = 0; i < ORDEN_BSTAR; i++) {
            hijos[i] = NULL;
        }
    }
    
    bool estaLleno() const {
        return num_llaves >= MAX_LLAVES_BSTAR;
    }
    
    bool estaBajoMinimo() const {
        // Mínimo 2/3 de capacidad
        return num_llaves < MIN_LLAVES_BSTAR;
    }
};

template <typename T>
class ArbolBStar {
private:
    NodoBStar<T>* raiz;
    int orden;
    
public:
    ArbolBStar(int m = ORDEN_BSTAR) : raiz(NULL), orden(m) {}
    
    void insertar(T dato);
    void eliminar(T dato);
    void mostrarEstadisticas();  // Muestra factor de ocupación
    
private:
    // Función clave: redistribución antes de split
    void redistribuirOAbrir(NodoBStar<T>* nodo, T dato);
    void prestarAIzquierdo(NodoBStar<T>* nodo, NodoBStar<T>* hermano);
    void prestarADerecho(NodoBStar<T>* nodo, NodoBStar<T>* hermano);
    void dividirConHermano(NodoBStar<T>* nodo1, NodoBStar<T>* nodo2);
    
    NodoBStar<T>* buscarNodoHoja(T dato);
    void insertarEnNodo(NodoBStar<T>* nodo, T dato);
};

// Implementación de inserción con redistribución
template <typename T>
void ArbolBStar<T>::insertar(T dato) {
    if(raiz == NULL) {
        raiz = new NodoBStar<T>();
        raiz->llaves[0] = dato;
        raiz->num_llaves = 1;
        return;
    }
    
    // NodoBStar<T>* nodo = buscarNodoHoja(dato);
    // if(!nodo->estaLleno()) {
    //     insertarEnNodo(nodo, dato);
    // } else {
    //     redistribuirOAbrir(nodo, dato);
    // }
}

template <typename T>
void ArbolBStar<T>::redistribuirOAbrir(NodoBStar<T>* nodo, T dato) {
    // 1. Buscar hermano con espacio
    NodoBStar<T>* hermano = NULL;
    
    if(nodo->hermano_der != NULL && !nodo->hermano_der->estaLleno()) {
        hermano = nodo->hermano_der;
        // Redistribuir: mover llaves de nodo a hermano
        // prestarADerecho(nodo, hermano);
        // insertarEnNodo(nodo, dato);
        return;
    }
    
    if(nodo->hermano_izq != NULL && !nodo->hermano_izq->estaLleno()) {
        hermano = nodo->hermano_izq;
        // Redistribuir: mover llaves de hermano a nodo
        // prestarAIzquierdo(nodo, hermano);
        // insertarEnNodo(nodo, dato);
        return;
    }
    
    // 2. Ambos hermanos llenos o no existen: hacer split
    if(hermano == NULL) {
        // No hay hermano o ambos están llenos -> split tradicional
        // dividirConHermano(nodo, nodo);
    } else {
        // Split con hermano: 2 nodos se dividen en 3
        // dividirConHermano(nodo, hermano);
    }
}

int main() {
    std::cout << "Implementacion de Arbol B* con redistribucion" << std::endl;
    return 0;
}
