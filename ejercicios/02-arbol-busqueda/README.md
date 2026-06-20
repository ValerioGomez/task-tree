# Ejercicio 02 — Árbol Binario de Búsqueda (BST)

## 🎯 Objetivo

Implementar un **Árbol Binario de Búsqueda (BST)** con todas sus operaciones fundamentales,
incluyendo la eliminación correcta con los tres casos posibles.

## 📐 Propiedad BST

Para todo nodo N del árbol:
```
  subárbol_izquierdo(N) < N.info  ≤  subárbol_derecho(N)
```

Esto garantiza que el **recorrido inorden** devuelve los valores en **orden ascendente**.

## 🔧 Funciones Implementadas

| Función | Complejidad | Descripción |
|---------|-------------|-------------|
| `find(p, data)` | O(log n) promedio | Búsqueda recursiva |
| `insert(p, data)` | O(log n) promedio | Inserción manteniendo BST |
| `size(p)` | O(n) | Contar todos los nodos |
| `minimum(p)` | O(log n) | Valor mínimo (nodo más a la izquierda) |
| `maximum(p)` | O(log n) | Valor máximo (nodo más a la derecha) |
| `deleteNode(p, x)` | O(log n) promedio | Eliminación con 3 casos |

## 🗑️ Los 3 Casos de Eliminación

### Caso 1: Nodo Hoja (sin hijos)
```
Eliminar 9:
    [8]          [8]
       \    →      \
       [9]          (NULL)
```
Simplemente se libera el nodo.

### Caso 2: Nodo con Un Hijo
```
Eliminar 16:
    [16]         [17]
       \    →
       [17]
```
El hijo sube a ocupar el lugar del nodo eliminado.

### Caso 3: Nodo con Dos Hijos
```
Eliminar 4:              Sucesor = min(subárbol derecho) = 5
      [4]                      [5]
     /   \         →          /   \
   [3]   [8]               [3]   [8]
         /                        /
        [5]                      (el 5 fue eliminado de aquí)
```
Se reemplaza el valor del nodo con su **sucesor en inorden** (mínimo del subárbol derecho),
luego se elimina ese sucesor.

## 🧪 Array de Prueba

```cpp
int numbers[] = {15, 4, 8, 7, 4, 3, 19, 5, 7, 9, 16, 5, 17};
```

El árbol resultante tras insertar todos los elementos:
```
                    [19]
                   /
              [16]
                 \
                 [17]
         [15]
              \
              [9]
         [8]
              \
              [7]
         [7]
    [4]
         \
         [5]
         [5]
    [4]
         \
    [3]
```

## 🚀 Compilar y Ejecutar

```bash
make && ./arbol_busqueda
```
