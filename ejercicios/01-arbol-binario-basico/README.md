# Ejercicio 01 — Árbol Binario Básico

## 🎯 Objetivo

Implementar las **operaciones primitivas** de un árbol binario usando la estructura `tnode`
con punteros a padre, hijo izquierdo e hijo derecho.

## 📐 Estructura del Nodo

```
        [info]
       /      \
   [left]   [right]
      ↑
   [father]
```

En C++98:

```cpp
struct tnode {
    int info;
    struct tnode *father;
    struct tnode *left;
    struct tnode *right;
};
typedef struct tnode *TNODEPTR;
```

## 🔧 Funciones Implementadas

| Función | Descripción |
|---------|-------------|
| `createNode()` | Reserva memoria para un nodo (sin inicializar) |
| `freeNode(p)` | Libera la memoria de un nodo |
| `makeTree(x)` | Crea un árbol de un solo nodo con valor `x` |
| `father(p)` | Retorna el puntero al padre |
| `leftChild(p)` | Retorna el puntero al hijo izquierdo |
| `rightChild(p)` | Retorna el puntero al hijo derecho |
| `setLeftChild(p, x)` | Crea y enlaza un hijo izquierdo con valor `x` |
| `setRightChild(p, x)` | Crea y enlaza un hijo derecho con valor `x` |
| `isLeft(p)` | ¿Es `p` el hijo izquierdo de su padre? |
| `isRight(p)` | ¿Es `p` el hijo derecho de su padre? |
| `sibling(p)` | Retorna el hermano del nodo |
| `insert(p, x)` | Inserta `x` en orden BST |
| `freeTree(p)` | Libera toda la memoria del árbol (postorden) |
| `imprimirArbol(p, n)` | Visualiza el árbol rotado 90° |

## 🏗️ Build123 — Tres Formas

Las tres formas construyen este árbol:

```
    2
   / \
  1   3
```

**Forma 1** — makeTree + asignación directa de punteros:
```cpp
TNODEPTR two = makeTree(2);
two->left  = makeTree(1);  two->left->father  = two;
two->right = makeTree(3);  two->right->father = two;
```

**Forma 2** — Tres variables puntero nombradas:
```cpp
TNODEPTR one = makeTree(1), two = makeTree(2), three = makeTree(3);
two->left = one;   one->father   = two;
two->right = three; three->father = two;
```

**Forma 3** — Usando `insert()` recursivo (BST):
```cpp
TNODEPTR root = NULL;
root = insert(root, 2);
root = insert(root, 1);
root = insert(root, 3);
```

## 🚀 Compilar y Ejecutar

```bash
make
./arbol_binario

# O directamente:
g++ -std=c++98 -Wall -Wextra -pedantic arbol_binario_basico.cpp -o arbol_binario
./arbol_binario
```

## 📋 Salida Esperada

```
=============================================
  EJERCICIO 01: ARBOL BINARIO BASICO
=============================================

=== BUILD123 — TRES FORMAS ===

--- Forma 1: makeTree + asignacion directa ---
Estructura del arbol:
        [3]
[2]
        [1]

Raiz         : 2
Hijo izq     : 1
Hijo der     : 3
...
```

## ⚠️ Notas

- `makeTree()` crea un nodo con todos los punteros en `NULL`.
- `setLeftChild()` y `setRightChild()` actualizan el puntero `father` del nuevo nodo.
- `freeTree()` usa recorrido **postorden** para liberar correctamente (hijos antes que padre).
- La visualización imprime el árbol **girado 90°**: el hijo derecho aparece arriba y el izquierdo abajo.
