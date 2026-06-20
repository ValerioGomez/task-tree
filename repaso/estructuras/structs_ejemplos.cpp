// ============================================================================
// REPASO: ESTRUCTURAS (STRUCTS) EN C++
// Archivo: repaso/estructuras/structs_ejemplos.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic structs_ejemplos.cpp -o structs_ej
// ============================================================================
//
// Temas cubiertos:
//   1. Definición básica de struct
//   2. Typedef con struct
//   3. Structs con punteros a sí mismas (auto-referencia)
//   4. Structs anidadas
//   5. Pasar structs a funciones (por valor y por puntero)
//   6. Structs que modelan nodos de árbol binario
// ============================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// 1. STRUCT BÁSICA
// ─────────────────────────────────────────────────────────────────────────────
struct Persona {
    char nombre[50];
    int  edad;
    float altura;
};

void ejemplo_struct_basica() {
    cout << "\n=== 1. STRUCT BASICA ===" << endl;

    struct Persona p1;
    strncpy(p1.nombre, "Ana Garcia", 49);
    p1.nombre[49] = '\0';
    p1.edad    = 28;
    p1.altura  = 1.65f;

    cout << "Nombre: " << p1.nombre << endl;
    cout << "Edad  : " << p1.edad   << endl;
    cout << "Altura: " << p1.altura << " m" << endl;
    cout << "Tamano de struct Persona: " << sizeof(struct Persona) << " bytes" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 2. TYPEDEF CON STRUCT
// ─────────────────────────────────────────────────────────────────────────────
struct punto_s {
    double x;
    double y;
};
typedef struct punto_s Punto;        // Alias simple
typedef struct punto_s *PuntoPtr;    // Alias de puntero

void ejemplo_typedef() {
    cout << "\n=== 2. TYPEDEF CON STRUCT ===" << endl;

    Punto p;       // Sin necesidad de escribir "struct punto_s"
    p.x = 3.0;
    p.y = 4.0;
    cout << "Punto: (" << p.x << ", " << p.y << ")" << endl;

    PuntoPtr pp = &p;
    cout << "Via puntero: (" << pp->x << ", " << pp->y << ")" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3. STRUCT AUTO-REFERENCIADA (nodo de lista enlazada)
// ─────────────────────────────────────────────────────────────────────────────
struct nodo_lista {
    int dato;
    struct nodo_lista *siguiente;   // puntero al mismo tipo
};
typedef struct nodo_lista *NodoPtr;

void ejemplo_autorreferencia() {
    cout << "\n=== 3. STRUCT AUTO-REFERENCIADA ===" << endl;

    // Crear lista: 10 -> 20 -> 30 -> NULL
    NodoPtr n1 = (NodoPtr)malloc(sizeof(struct nodo_lista));
    NodoPtr n2 = (NodoPtr)malloc(sizeof(struct nodo_lista));
    NodoPtr n3 = (NodoPtr)malloc(sizeof(struct nodo_lista));

    n1->dato = 10; n1->siguiente = n2;
    n2->dato = 20; n2->siguiente = n3;
    n3->dato = 30; n3->siguiente = NULL;

    cout << "Lista: ";
    NodoPtr actual = n1;
    while (actual != NULL) {
        cout << actual->dato;
        if (actual->siguiente) cout << " -> ";
        actual = actual->siguiente;
    }
    cout << " -> NULL" << endl;

    free(n1); free(n2); free(n3);
}

// ─────────────────────────────────────────────────────────────────────────────
// 4. STRUCTS ANIDADAS
// ─────────────────────────────────────────────────────────────────────────────
struct Fecha {
    int dia;
    int mes;
    int anio;
};

struct Empleado {
    char nombre[50];
    int  legajo;
    struct Fecha fecha_ingreso;   // struct anidada por valor
    struct Fecha *fecha_baja;     // struct anidada por puntero (puede ser NULL)
};

void ejemplo_anidacion() {
    cout << "\n=== 4. STRUCTS ANIDADAS ===" << endl;

    struct Empleado emp;
    strncpy(emp.nombre, "Carlos Lopez", 49);
    emp.nombre[49] = '\0';
    emp.legajo = 1001;
    emp.fecha_ingreso.dia  = 15;
    emp.fecha_ingreso.mes  = 3;
    emp.fecha_ingreso.anio = 2020;
    emp.fecha_baja = NULL;   // sigue activo

    cout << "Empleado: " << emp.nombre << " (legajo " << emp.legajo << ")" << endl;
    cout << "Ingreso : " << emp.fecha_ingreso.dia << "/"
                         << emp.fecha_ingreso.mes << "/"
                         << emp.fecha_ingreso.anio << endl;
    cout << "Baja    : " << (emp.fecha_baja == NULL ? "activo" : "dado de baja") << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 5. PASAR STRUCTS A FUNCIONES
// ─────────────────────────────────────────────────────────────────────────────
struct Rectangulo {
    double ancho;
    double alto;
};

// Por valor: recibe una COPIA (no modifica el original)
double area_por_valor(struct Rectangulo r) {
    return r.ancho * r.alto;
}

// Por puntero: recibe la dirección (puede modificar el original)
void escalar_por_puntero(struct Rectangulo *r, double factor) {
    r->ancho *= factor;
    r->alto  *= factor;
}

void ejemplo_paso_funciones() {
    cout << "\n=== 5. PASAR STRUCTS A FUNCIONES ===" << endl;

    struct Rectangulo rect;
    rect.ancho = 5.0;
    rect.alto  = 3.0;

    cout << "Area (por valor)   = " << area_por_valor(rect) << endl;
    escalar_por_puntero(&rect, 2.0);
    cout << "Despues de escalar x2: " << rect.ancho << " x " << rect.alto << endl;
    cout << "Area nueva         = " << area_por_valor(rect) << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
// 6. STRUCT DE NODO DE ÁRBOL BINARIO (anticipo del Ejercicio 01)
// ─────────────────────────────────────────────────────────────────────────────
struct tnode_preview {
    int dato;
    struct tnode_preview *padre;
    struct tnode_preview *izquierdo;
    struct tnode_preview *derecho;
};
typedef struct tnode_preview *TNODEPTR_P;

TNODEPTR_P crearNodoPreview(int valor) {
    TNODEPTR_P p = (TNODEPTR_P)malloc(sizeof(struct tnode_preview));
    p->dato   = valor;
    p->padre = NULL;
    p->izquierdo   = NULL;
    p->derecho  = NULL;
    return p;
}

void ejemplo_nodo_arbol() {
    cout << "\n=== 6. NODO DE ARBOL BINARIO (preview) ===" << endl;

    //        2
    //       / \  .
    //      1   3
    TNODEPTR_P raiz = crearNodoPreview(2);
    raiz->izquierdo  = crearNodoPreview(1);
    raiz->derecho = crearNodoPreview(3);
    raiz->izquierdo->padre  = raiz;
    raiz->derecho->padre = raiz;

    cout << "Raiz       : " << raiz->dato        << endl;
    cout << "Hijo izq   : " << raiz->izquierdo->dato  << endl;
    cout << "Hijo der   : " << raiz->derecho->dato << endl;
    cout << "Padre del 1: " << raiz->izquierdo->padre->dato << endl;

    free(raiz->izquierdo);
    free(raiz->derecho);
    free(raiz);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  REPASO DE ESTRUCTURAS (STRUCTS) EN C++98" << endl;
    cout << "============================================" << endl;

    ejemplo_struct_basica();
    ejemplo_typedef();
    ejemplo_autorreferencia();
    ejemplo_anidacion();
    ejemplo_paso_funciones();
    ejemplo_nodo_arbol();

    cout << "\n============================================" << endl;
    cout << "  FIN DEL REPASO DE STRUCTS" << endl;
    cout << "============================================" << endl;

    return 0;
}

