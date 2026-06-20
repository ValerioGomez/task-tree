// ============================================================================
// EJERCICIO 05: ÁRBOL MULTICAMINO (B-TREE ORDEN 4)
// Archivo: ejercicios/05-multicamino/arbol_multicamino.cpp
// Compilar: g++ -std=c++98 -Wall -Wextra -pedantic arbol_multicamino.cpp -o arbol_multicamino
// ============================================================================
//
// Un árbol B de ORDEN 4 (también llamado árbol 2-3-4) tiene:
//   - Cada nodo tiene como máximo 3 llaves y 4 hijos
//   - Cada nodo (excepto la raíz) tiene al menos 2 hijos (si es nodo interno)
//   - Todas las hojas están al mismo nivel
//   - Las llaves en cada nodo están en orden ascendente
//
// Estructura de un nodo:
//   llaves  : [k0] [k1] [k2]          (hasta MAX_LLAVES = ORDEN-1 = 3)
//   hijos   : [h0] [h1] [h2] [h3]     (hasta ORDEN = 4 hijos)
//
// Para una llave k_i: h_i contiene valores < k_i, h_{i+1} contiene valores >= k_i
//
// Operaciones:
//   - numTrees     : número de hijos de un nodo
//   - child        : i-ésimo hijo
//   - key          : j-ésima llave
//   - traverse     : recorrido ascendente (inorden generalizado)
//   - buscar       : encontrar una llave
//   - insertarBTree: inserción con split de nodos
// ============================================================================

#include <iostream>
#include <cstdlib>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// CONSTANTES DEL ÁRBOL B
// ─────────────────────────────────────────────────────────────────────────────
#define ORDEN       4              // Orden del árbol (máximo de hijos por nodo)
#define MAX_LLAVES  (ORDEN - 1)    // Máximo de llaves por nodo (3)
#define MIN_LLAVES  (ORDEN/2 - 1) // Mínimo de llaves (excluyendo raíz) (1)

// ─────────────────────────────────────────────────────────────────────────────
// ESTRUCTURA DEL NODO MULTICAMINO
// ─────────────────────────────────────────────────────────────────────────────
struct multicamino_node {
    int num_llaves;                              // llaves actualmente en uso
    int llaves[MAX_LLAVES];                      // arreglo de llaves
    struct multicamino_node *hijos[ORDEN];       // arreglo de hijos
    struct multicamino_node *padre;              // puntero al padre
    bool es_hoja;                                // ¿es nodo hoja?
};

typedef struct multicamino_node *MNODEPTR;

// ─────────────────────────────────────────────────────────────────────────────
// CREACIÓN DE NODO
// ─────────────────────────────────────────────────────────────────────────────
MNODEPTR crearNodo(bool es_hoja) {
    MNODEPTR p = (MNODEPTR)malloc(sizeof(struct multicamino_node));
    p->num_llaves = 0;
    p->padre      = NULL;
    p->es_hoja    = es_hoja;
    for (int i = 0; i < ORDEN; i++)
        p->hijos[i] = NULL;
    for (int i = 0; i < MAX_LLAVES; i++)
        p->llaves[i] = 0;
    return p;
}

// ─────────────────────────────────────────────────────────────────────────────
// OPERACIONES PRIMITIVAS
// ─────────────────────────────────────────────────────────────────────────────

// Número de hijos de un nodo (= num_llaves + 1 si no es hoja, 0 si es hoja)
int numTrees(MNODEPTR p) {
    if (p == NULL) return 0;
    if (p->es_hoja) return 0;
    return p->num_llaves + 1;
}

// Retorna el i-ésimo hijo (0-indexado)
MNODEPTR child(MNODEPTR p, int i) {
    if (p == NULL || i < 0 || i >= ORDEN) return NULL;
    return p->hijos[i];
}

// Retorna la j-ésima llave (0-indexada)
int key(MNODEPTR p, int j) {
    if (p == NULL || j < 0 || j >= p->num_llaves) return -1;
    return p->llaves[j];
}

// ─────────────────────────────────────────────────────────────────────────────
// RECORRIDO ASCENDENTE (inorden generalizado)
// Para cada nodo: recorre hijo[i], luego imprime llave[i], luego hijo[i+1]...
// ─────────────────────────────────────────────────────────────────────────────
void traverse(MNODEPTR node) {
    if (node == NULL) return;

    int i;
    for (i = 0; i < node->num_llaves; i++) {
        // Recorrer el hijo i (si existe)
        if (!node->es_hoja) {
            traverse(node->hijos[i]);
        }
        // Imprimir la llave i
        cout << node->llaves[i] << " ";
    }
    // Recorrer el último hijo
    if (!node->es_hoja) {
        traverse(node->hijos[i]);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// NEXT — siguiente llave en el recorrido inorden
// Dado un nodo y el índice de una llave, retorna el nodo con la siguiente llave
// ─────────────────────────────────────────────────────────────────────────────
MNODEPTR next(MNODEPTR node, int index) {
    if (node == NULL) return NULL;

    // Si tiene hijo derecho a esa llave, el sucesor está en ese subárbol
    if (!node->es_hoja && node->hijos[index + 1] != NULL) {
        MNODEPTR p = node->hijos[index + 1];
        while (!p->es_hoja) {
            p = p->hijos[0];   // ir al nodo más a la izquierda
        }
        return p;
    }

    // Si no tiene hijo derecho, subir hacia el padre
    MNODEPTR actual = node;
    MNODEPTR padre  = node->padre;
    while (padre != NULL) {
        // Buscar qué hijo soy
        for (int i = 0; i <= padre->num_llaves; i++) {
            if (padre->hijos[i] == actual) {
                if (i < padre->num_llaves) {
                    return padre;    // padre tiene llave sucesora
                }
                break;
            }
        }
        actual = padre;
        padre  = padre->padre;
    }
    return NULL;    // es el último elemento
}

// ─────────────────────────────────────────────────────────────────────────────
// SUCCESSOR — sucesor en inorden de la llave[index] de node
// ─────────────────────────────────────────────────────────────────────────────
MNODEPTR successor(MNODEPTR node, int index) {
    if (node == NULL) return NULL;

    // El sucesor inmediato es el mínimo del subárbol derecho
    if (!node->es_hoja && node->hijos[index + 1] != NULL) {
        MNODEPTR p = node->hijos[index + 1];
        while (!p->es_hoja) {
            p = p->hijos[0];
        }
        return p;   // p->llaves[0] es el sucesor
    }
    return NULL;
}

// ─────────────────────────────────────────────────────────────────────────────
// BÚSQUEDA
// ─────────────────────────────────────────────────────────────────────────────
bool buscar(MNODEPTR node, int k) {
    if (node == NULL) return false;

    // Encontrar el primer índice donde llaves[i] >= k
    int i = 0;
    while (i < node->num_llaves && k > node->llaves[i])
        i++;

    // ¿Encontramos la llave exacta?
    if (i < node->num_llaves && k == node->llaves[i])
        return true;

    // Si es hoja y no está aquí, no existe
    if (node->es_hoja)
        return false;

    // Buscar en el hijo apropiado
    return buscar(node->hijos[i], k);
}

// ─────────────────────────────────────────────────────────────────────────────
// SPLIT — dividir un hijo lleno
// Cuando hijos[i] tiene MAX_LLAVES llaves, se divide en dos y la llave del
// medio sube al padre (node)
// ─────────────────────────────────────────────────────────────────────────────
void splitChild(MNODEPTR node, int i, MNODEPTR hijoLleno) {
    int medio = MAX_LLAVES / 2;    // índice de la llave que sube

    // Crear nuevo nodo con las llaves derechas del hijo lleno
    MNODEPTR nuevoNodo = crearNodo(hijoLleno->es_hoja);
    nuevoNodo->num_llaves = MAX_LLAVES - medio - 1;
    nuevoNodo->padre      = node;

    // Copiar llaves derechas
    for (int j = 0; j < nuevoNodo->num_llaves; j++)
        nuevoNodo->llaves[j] = hijoLleno->llaves[j + medio + 1];

    // Copiar hijos derechos (si no es hoja)
    if (!hijoLleno->es_hoja) {
        for (int j = 0; j <= nuevoNodo->num_llaves; j++) {
            nuevoNodo->hijos[j] = hijoLleno->hijos[j + medio + 1];
            if (nuevoNodo->hijos[j] != NULL)
                nuevoNodo->hijos[j]->padre = nuevoNodo;
        }
    }

    // Ajustar el hijo original (conserva las llaves izquierdas)
    hijoLleno->num_llaves = medio;

    // Hacer espacio en node para insertar el nuevo hijo
    for (int j = node->num_llaves; j >= i + 1; j--)
        node->hijos[j + 1] = node->hijos[j];

    // Insertar el nuevo nodo en los hijos de node
    node->hijos[i + 1] = nuevoNodo;

    // Hacer espacio en node para insertar la llave que sube
    for (int j = node->num_llaves - 1; j >= i; j--)
        node->llaves[j + 1] = node->llaves[j];

    // La llave del medio sube al padre
    node->llaves[i]   = hijoLleno->llaves[medio];
    node->num_llaves += 1;
}

// ─────────────────────────────────────────────────────────────────────────────
// INSERTAR EN NODO NO LLENO
// ─────────────────────────────────────────────────────────────────────────────
void insertarNoLleno(MNODEPTR node, int k) {
    int i = node->num_llaves - 1;

    if (node->es_hoja) {
        // Insertar directamente en la hoja, desplazando llaves mayores
        while (i >= 0 && k < node->llaves[i]) {
            node->llaves[i + 1] = node->llaves[i];
            i--;
        }
        node->llaves[i + 1]  = k;
        node->num_llaves    += 1;
    } else {
        // Encontrar el hijo donde debe ir k
        while (i >= 0 && k < node->llaves[i])
            i--;
        i++;    // hijo correcto

        // Si el hijo está lleno, hay que dividirlo
        if (node->hijos[i]->num_llaves == MAX_LLAVES) {
            splitChild(node, i, node->hijos[i]);
            // Después del split, decidir en cuál de los dos hijos insertar
            if (k > node->llaves[i])
                i++;
        }
        insertarNoLleno(node->hijos[i], k);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// INSERTAR EN EL ÁRBOL B
// ─────────────────────────────────────────────────────────────────────────────
MNODEPTR insertarBTree(MNODEPTR raiz, int k) {
    if (raiz == NULL) {
        // Árbol vacío: crear raíz
        MNODEPTR r = crearNodo(true);
        r->llaves[0]   = k;
        r->num_llaves  = 1;
        return r;
    }

    // Si la raíz está llena, hay que dividirla y crear nueva raíz
    if (raiz->num_llaves == MAX_LLAVES) {
        MNODEPTR nuevaRaiz = crearNodo(false);
        nuevaRaiz->hijos[0] = raiz;
        raiz->padre = nuevaRaiz;
        splitChild(nuevaRaiz, 0, raiz);
        insertarNoLleno(nuevaRaiz, k);
        return nuevaRaiz;
    }

    insertarNoLleno(raiz, k);
    return raiz;
}

// ─────────────────────────────────────────────────────────────────────────────
// VISUALIZACIÓN DEL ÁRBOL B (por niveles)
// ─────────────────────────────────────────────────────────────────────────────
void imprimirNivel(MNODEPTR node, int nivel) {
    if (node == NULL) return;

    // Indentación
    for (int i = 0; i < nivel; i++) cout << "  ";

    // Imprimir llaves del nodo entre corchetes
    cout << "[";
    for (int i = 0; i < node->num_llaves; i++) {
        cout << node->llaves[i];
        if (i < node->num_llaves - 1) cout << "|";
    }
    cout << "]" << endl;

    // Imprimir hijos recursivamente
    if (!node->es_hoja) {
        for (int i = 0; i <= node->num_llaves; i++) {
            imprimirNivel(node->hijos[i], nivel + 1);
        }
    }
}

void liberarBTree(MNODEPTR p) {
    if (p == NULL) return;
    if (!p->es_hoja) {
        for (int i = 0; i <= p->num_llaves; i++)
            liberarBTree(p->hijos[i]);
    }
    free(p);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "============================================" << endl;
    cout << "  EJERCICIO 05: ARBOL MULTICAMINO (B-TREE)" << endl;
    cout << "  Orden = " << ORDEN << " | Max llaves = " << MAX_LLAVES << endl;
    cout << "============================================" << endl;

    MNODEPTR raiz = NULL;

    // ─── Insertar valores ────────────────────────────────────────────────
    int valores[] = {10, 20, 5, 6, 12, 30, 7, 17, 3, 25, 15, 8, 35, 40, 1};
    int n = 15;

    cout << "\nInsertando valores: ";
    for (int i = 0; i < n; i++) {
        cout << valores[i];
        if (i < n - 1) cout << ", ";
        raiz = insertarBTree(raiz, valores[i]);
    }
    cout << endl;

    // ─── Visualización ───────────────────────────────────────────────────
    cout << "\nEstructura del Arbol B (indenado por nivel):" << endl;
    imprimirNivel(raiz, 0);

    // ─── Recorrido ascendente ────────────────────────────────────────────
    cout << "\nRecorrido ascendente (inorden generalizado):" << endl;
    traverse(raiz);
    cout << endl;

    // ─── Búsquedas ───────────────────────────────────────────────────────
    cout << "\n--- Busqueda ---" << endl;
    int buscar_vals[] = {12, 17, 99, 1, 40};
    for (int i = 0; i < 5; i++) {
        cout << "Buscar " << buscar_vals[i] << ": "
             << (buscar(raiz, buscar_vals[i]) ? "ENCONTRADO" : "NO encontrado")
             << endl;
    }

    // ─── numTrees y key ──────────────────────────────────────────────────
    cout << "\n--- Funciones primitivas sobre la raiz ---" << endl;
    cout << "numTrees(raiz) = " << numTrees(raiz) << " (numero de hijos)" << endl;
    cout << "Llaves de la raiz: ";
    for (int j = 0; j < raiz->num_llaves; j++) {
        cout << key(raiz, j) << " ";
    }
    cout << endl;
    cout << "child(raiz, 0) -> llaves: [";
    MNODEPTR h0 = child(raiz, 0);
    if (h0 != NULL) {
        for (int j = 0; j < h0->num_llaves; j++) {
            cout << h0->llaves[j];
            if (j < h0->num_llaves - 1) cout << "|";
        }
    }
    cout << "]" << endl;

    // ─── Successor ───────────────────────────────────────────────────────
    cout << "\n--- Sucesor ---" << endl;
    if (raiz->num_llaves > 0) {
        MNODEPTR suc = successor(raiz, 0);
        if (suc != NULL)
            cout << "Sucesor de raiz->llaves[0]=" << raiz->llaves[0]
                 << " es el nodo con llaves[0]=" << suc->llaves[0] << endl;
    }

    liberarBTree(raiz);

    cout << "\n============================================" << endl;
    cout << "  FIN DEL EJERCICIO 05" << endl;
    cout << "============================================" << endl;

    return 0;
}

