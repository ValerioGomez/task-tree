# Ejercicio 05 — Árbol Multicamino (B-Tree Orden 4)

## 🎯 Objetivo

Implementar un **Árbol B de orden 4** (árbol 2-3-4), que es el árbol de búsqueda multicamino
más utilizado en sistemas de bases de datos y sistemas de archivos.

## 📐 Propiedades del Árbol B Orden 4

| Propiedad | Valor |
|-----------|-------|
| Orden | 4 |
| Máximo de hijos por nodo | 4 |
| Máximo de llaves por nodo | 3 (`MAX_LLAVES = ORDEN - 1`) |
| Mínimo de llaves (no raíz) | 1 |
| Todas las hojas al mismo | nivel |

### Estructura de un nodo:
```
       [k0 | k1 | k2]
      /    |    |    \
    h0    h1   h2    h3
  (<k0) (k0≤·<k1) (k1≤·<k2) (≥k2)
```

## 🔧 Funciones Implementadas

| Función | Descripción |
|---------|-------------|
| `numTrees(p)` | Número de hijos (0 si es hoja, `num_llaves+1` si es interno) |
| `child(p, i)` | Retorna el i-ésimo hijo |
| `key(p, j)` | Retorna la j-ésima llave |
| `traverse(node)` | Recorrido ascendente (inorden generalizado) |
| `next(node, i)` | Nodo con la siguiente llave en inorden |
| `successor(node, i)` | Nodo sucesor de `llave[i]` |
| `buscar(node, k)` | Búsqueda de la llave k |
| `splitChild(node, i, hijo)` | Divide un hijo lleno |
| `insertarBTree(raiz, k)` | Inserción con split automático |

## 🔀 Inserción con Split

Cuando un nodo tiene `MAX_LLAVES` llaves y debemos insertar en él,
se realiza un **split**:

```
Nodo lleno: [10|20|30]
                ↓ split
        [20]           ← llave del medio sube al padre
       /    \
    [10]    [30]       ← dos nodos con las llaves extremas
```

## 📊 Comparativa de Árboles de Búsqueda

| Árbol | Complejidad | Característica |
|-------|-------------|----------------|
| BST | O(log n) promedio, O(n) peor | Sin balance garantizado |
| AVL | O(log n) siempre | Balance estricto (FE ∈ {-1,0,1}) |
| B-Tree | O(log_t n) | Múltiples llaves/hijos, mínima E/S de disco |

El árbol B es preferido en **bases de datos** porque minimiza accesos a disco:
cada nodo cabe en un bloque de disco.

## 🚀 Compilar y Ejecutar

```bash
make && ./arbol_multicamino
```

## 📋 Salida Esperada (parcial)

```
Estructura del Arbol B:
[15|25]
  [5|8|10]
    [1|3]
    [6|7]
    [8]
    [10|12]
  [17|20]
    ...
```
