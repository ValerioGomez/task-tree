// ============================================================================
// EJERCICIO 06 — APLICACIÓN 2: ÁRBOL DE EXPRESIONES
// Archivo: ejercicios/06-aplicaciones/expresiones.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic expresiones.cpp -o expresiones
// ============================================================================
//
// Un árbol de expresiones representa una expresión aritmética:
//   - Las HOJAS contienen OPERANDOS (valores o variables)
//   - Los NODOS INTERNOS contienen OPERADORES (+, -, *, /)
//
// Expresiones a representar:
//   1. A + B * C        (la multiplicación tiene precedencia)
//   2. (A + B) * C      (el paréntesis fuerza la suma primero)
//
// Para expr. 1: A + B * C
//          +
//         / \  .
//        A   *
//           / \  .
//          B   C
//
// Para expr. 2: (A + B) * C
//          *
//         / \  .
//        +   C
//       / \  .
//      A   B
//
// Recorridos sobre árboles de expresión:
//   - Inorden:    notación infija   (A + B * C)
//   - Preorden:   notación prefija  (+ A * B C)
//   - Postorden:  notación posfija  (A B C * +)
//
// ============================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// ESTRUCTURA DEL NODO DE EXPRESIÓN
// ─────────────────────────────────────────────────────────────────────────────
struct NodoExpr {
    char     token[4];              // operador (+,-,*,/) o nombre de operando
    bool     es_operador;           // true si es +,-,*,/
    double   valor;                 // valor numérico del operando
    struct NodoExpr *padre;
    struct NodoExpr *izquierdo;
    struct NodoExpr *derecho;
};

typedef struct NodoExpr *NodoExprPtr;

// ─────────────────────────────────────────────────────────────────────────────
// CREACIÓN DE NODOS
// ─────────────────────────────────────────────────────────────────────────────
NodoExprPtr crearOperador(const char *op) {
    NodoExprPtr p = (NodoExprPtr)malloc(sizeof(struct NodoExpr));
    strncpy(p->token, op, 3);
    p->token[3]    = '\0';
    p->es_operador = true;
    p->valor       = 0.0;
    p->padre      = NULL;
    p->izquierdo        = NULL;
    p->derecho       = NULL;
    return p;
}

NodoExprPtr crearOperando(const char *nombre, double val) {
    NodoExprPtr p = (NodoExprPtr)malloc(sizeof(struct NodoExpr));
    strncpy(p->token, nombre, 3);
    p->token[3]    = '\0';
    p->es_operador = false;
    p->valor       = val;
    p->padre      = NULL;
    p->izquierdo        = NULL;
    p->derecho       = NULL;
    return p;
}

void liberarExprTree(NodoExprPtr p) {
    if (p == NULL) return;
    liberarExprTree(p->izquierdo);
    liberarExprTree(p->derecho);
    free(p);
}

// ─────────────────────────────────────────────────────────────────────────────
// EVALUACIÓN DEL ÁRBOL (recorrido postorden)
// ─────────────────────────────────────────────────────────────────────────────
double evaluar(NodoExprPtr p) {
    if (p == NULL) return 0.0;

    // Caso base: es una hoja (operando)
    if (!p->es_operador) {
        return p->valor;
    }

    // Evaluar recursivamente los subárboles
    double izq = evaluar(p->izquierdo);
    double der = evaluar(p->derecho);

    // Aplicar el operador
    if (strcmp(p->token, "+") == 0) return izq + der;
    if (strcmp(p->token, "-") == 0) return izq - der;
    if (strcmp(p->token, "*") == 0) return izq * der;
    if (strcmp(p->token, "/") == 0) {
        if (der == 0.0) {
            cout << "ERROR: division por cero" << endl;
            return 0.0;
        }
        return izq / der;
    }

    return 0.0;
}

// ─────────────────────────────────────────────────────────────────────────────
// RECORRIDOS PARA NOTACIONES
// ─────────────────────────────────────────────────────────────────────────────

// INORDEN → notación infija (con paréntesis para claridad)
void infija(NodoExprPtr p) {
    if (p == NULL) return;
    if (p->es_operador) cout << "(";
    infija(p->izquierdo);
    cout << p->token;
    infija(p->derecho);
    if (p->es_operador) cout << ")";
}

// PREORDEN → notación prefija (notación polaca)
void prefija(NodoExprPtr p) {
    if (p == NULL) return;
    cout << p->token << " ";
    prefija(p->izquierdo);
    prefija(p->derecho);
}

// POSTORDEN → notación posfija (notación polaca inversa)
void posfija(NodoExprPtr p) {
    if (p == NULL) return;
    posfija(p->izquierdo);
    posfija(p->derecho);
    cout << p->token << " ";
}

// ─────────────────────────────────────────────────────────────────────────────
// VISUALIZACIÓN (árbol girado 90°)
// ─────────────────────────────────────────────────────────────────────────────
void imprimirExprTree(NodoExprPtr p, int nivel) {
    if (p == NULL) return;
    imprimirExprTree(p->derecho, nivel + 1);
    for (int i = 0; i < nivel; i++) cout << "   ";
    cout << "(" << p->token << ")" << endl;
    imprimirExprTree(p->izquierdo, nivel + 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// CONSTRUIR ÁRBOL 1: A + B * C
//   A=2, B=3, C=4 → resultado = 2 + 3*4 = 14
//
//        +
//       / \  .
//      A   *
//         / \  .
//        B   C
// ─────────────────────────────────────────────────────────────────────────────
NodoExprPtr construirExpresion1(double valA, double valB, double valC) {
    // Raíz: operador +
    NodoExprPtr raiz = crearOperador("+");

    // Hijo izquierdo: operando A
    NodoExprPtr nA = crearOperando("A", valA);
    nA->padre = raiz;
    raiz->izquierdo = nA;

    // Hijo derecho: operador *
    NodoExprPtr mult = crearOperador("*");
    mult->padre = raiz;
    raiz->derecho  = mult;

    // Hijos del operador *: B y C
    NodoExprPtr nB = crearOperando("B", valB);
    NodoExprPtr nC = crearOperando("C", valC);
    nB->padre  = mult;
    nC->padre  = mult;
    mult->izquierdo  = nB;
    mult->derecho = nC;

    return raiz;
}

// ─────────────────────────────────────────────────────────────────────────────
// CONSTRUIR ÁRBOL 2: (A + B) * C
//   A=2, B=3, C=4 → resultado = (2+3)*4 = 20
//
//        *
//       / \  .
//      +   C
//     / \  .
//    A   B
// ─────────────────────────────────────────────────────────────────────────────
NodoExprPtr construirExpresion2(double valA, double valB, double valC) {
    // Raíz: operador *
    NodoExprPtr raiz = crearOperador("*");

    // Hijo izquierdo: operador +
    NodoExprPtr suma = crearOperador("+");
    suma->padre = raiz;
    raiz->izquierdo   = suma;

    // Hijos del operador +: A y B
    NodoExprPtr nA = crearOperando("A", valA);
    NodoExprPtr nB = crearOperando("B", valB);
    nA->padre = suma;
    nB->padre = suma;
    suma->izquierdo  = nA;
    suma->derecho = nB;

    // Hijo derecho de raíz: operando C
    NodoExprPtr nC = crearOperando("C", valC);
    nC->padre  = raiz;
    raiz->derecho = nC;

    return raiz;
}

// ─────────────────────────────────────────────────────────────────────────────
// CONSTRUIR ÁRBOL 3: Expresión compleja (A - B) / (C + D) * E
// ─────────────────────────────────────────────────────────────────────────────
NodoExprPtr construirExpresion3(double vA, double vB, double vC, double vD, double vE) {
    //         *
    //        / \  .
    //       /   E
    //      /
    //    (A-B)/(C+D)
    //      /
    //     raiz: /
    //    / \  .
    //  (A-B) (C+D)

    NodoExprPtr raizMult = crearOperador("*");
    NodoExprPtr div_     = crearOperador("/");
    NodoExprPtr rest     = crearOperador("-");
    NodoExprPtr suma     = crearOperador("+");

    NodoExprPtr nA = crearOperando("A", vA);
    NodoExprPtr nB = crearOperando("B", vB);
    NodoExprPtr nC = crearOperando("C", vC);
    NodoExprPtr nD = crearOperando("D", vD);
    NodoExprPtr nE = crearOperando("E", vE);

    rest->izquierdo  = nA; nA->padre = rest;
    rest->derecho = nB; nB->padre = rest;

    suma->izquierdo  = nC; nC->padre = suma;
    suma->derecho = nD; nD->padre = suma;

    div_->izquierdo  = rest; rest->padre = div_;
    div_->derecho = suma; suma->padre = div_;

    raizMult->izquierdo  = div_; div_->padre = raizMult;
    raizMult->derecho = nE;   nE->padre   = raizMult;

    return raizMult;
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  APLICACION 2: ARBOL DE EXPRESIONES" << endl;
    cout << "============================================" << endl;

    double A = 2.0, B = 3.0, C = 4.0;

    // ─── Expresión 1: A + B * C ───────────────────────────────────────────
    cout << "\n=== EXPRESION 1: A + B * C ===" << endl;
    cout << "A=" << A << ", B=" << B << ", C=" << C << endl;
    cout << "(la multiplicacion tiene mayor precedencia)" << endl;

    NodoExprPtr expr1 = construirExpresion1(A, B, C);

    cout << "\nEstructura del arbol (girado 90 grados):" << endl;
    imprimirExprTree(expr1, 0);

    cout << "\nNotacion infija   : "; infija(expr1);  cout << endl;
    cout << "Notacion prefija  : "; prefija(expr1); cout << endl;
    cout << "Notacion posfija  : "; posfija(expr1); cout << endl;
    cout << "Resultado = " << A << " + " << B << " * " << C
         << " = " << evaluar(expr1) << endl;

    liberarExprTree(expr1);

    // ─── Expresión 2: (A + B) * C ─────────────────────────────────────────
    cout << "\n=== EXPRESION 2: (A + B) * C ===" << endl;
    cout << "A=" << A << ", B=" << B << ", C=" << C << endl;
    cout << "(el parentesis fuerza la suma primero)" << endl;

    NodoExprPtr expr2 = construirExpresion2(A, B, C);

    cout << "\nEstructura del arbol (girado 90 grados):" << endl;
    imprimirExprTree(expr2, 0);

    cout << "\nNotacion infija   : "; infija(expr2);  cout << endl;
    cout << "Notacion prefija  : "; prefija(expr2); cout << endl;
    cout << "Notacion posfija  : "; posfija(expr2); cout << endl;
    cout << "Resultado = (" << A << " + " << B << ") * " << C
         << " = " << evaluar(expr2) << endl;

    liberarExprTree(expr2);

    // ─── Comparación ──────────────────────────────────────────────────────
    cout << "\n=== COMPARACION ===" << endl;
    cout << "A + B * C   = " << A + B * C   << " (sin parentesis, * primero)" << endl;
    cout << "(A + B) * C = " << (A + B) * C << " (con parentesis, + primero)" << endl;
    cout << "El ARBOL codifica la precedencia mediante su ESTRUCTURA." << endl;

    // ─── Expresión 3: (A - B) / (C + D) * E ──────────────────────────────
    cout << "\n=== EXPRESION 3: (A - B) / (C + D) * E ===" << endl;
    double D = 1.0, E = 5.0;
    cout << "A=" << A << " B=" << B << " C=" << C << " D=" << D << " E=" << E << endl;

    NodoExprPtr expr3 = construirExpresion3(A, B, C, D, E);

    cout << "\nArbol:" << endl;
    imprimirExprTree(expr3, 0);

    cout << "\nNotacion infija   : "; infija(expr3);  cout << endl;
    cout << "Notacion prefija  : "; prefija(expr3); cout << endl;
    cout << "Notacion posfija  : "; posfija(expr3); cout << endl;
    cout << "Resultado = (" << A << "-" << B << ")/(" << C << "+" << D << ")*" << E
         << " = " << evaluar(expr3) << endl;

    liberarExprTree(expr3);

    cout << "\n============================================" << endl;
    cout << "  FIN DE APLICACION 2" << endl;
    cout << "============================================" << endl;

    return 0;
}

