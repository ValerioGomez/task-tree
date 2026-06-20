# Teoría de Árboles — Estructuras de Datos y Algoritmos Avanzados

## 1. Definiciones Fundamentales

### 1.1 Árbol

Un **árbol** es una estructura de datos no lineal compuesta por nodos enlazados mediante aristas,
que satisface las siguientes propiedades:

- Existe exactamente **un nodo raíz** (sin padre)
- Cada nodo (excepto la raíz) tiene **exactamente un padre**
- No existen ciclos
- El árbol es **conectado** (existe un camino entre cualquier par de nodos)

### 1.2 Terminología

| Término | Definición |
|---------|------------|
| **Raíz** | Nodo sin padre |
| **Hoja** | Nodo sin hijos |
| **Nodo interno** | Nodo con al menos un hijo |
| **Padre** | Nodo que precede a otro en el camino desde la raíz |
| **Hijo** | Nodo que sigue a otro (sucesor directo) |
| **Hermano** | Nodos con el mismo padre |
| **Nivel** | Distancia desde la raíz (raíz está en nivel 0) |
| **Altura** | Longitud del camino más largo desde el nodo hasta una hoja |
| **Profundidad** | Longitud del camino desde la raíz hasta el nodo |
| **Grado** | Número de hijos de un nodo |

### 1.3 Árbol Binario

Un árbol en el que cada nodo tiene **como máximo 2 hijos**: izquierdo y derecho.

**Propiedades:**
- Un árbol binario con $h$ niveles tiene como máximo $2^h - 1$ nodos
- Un árbol binario con $n$ nodos tiene altura $h \geq \lceil \log_2(n+1) \rceil$

---

## 2. Árbol Binario de Búsqueda (BST)

### 2.1 Definición

Para todo nodo $N$ en un BST:
$$\text{todos los valores en } T_{izq}(N) < N.info \leq \text{todos los valores en } T_{der}(N)$$

### 2.2 Estructura en C++98

```cpp
struct tnode {
    int info;
    struct tnode *father;
    struct tnode *left;
    struct tnode *right;
};
typedef struct tnode *TNODEPTR;
```

### 2.3 Complejidad de Operaciones

| Operación | Promedio | Peor Caso |
|-----------|----------|-----------|
| Búsqueda | $O(\log n)$ | $O(n)$ |
| Inserción | $O(\log n)$ | $O(n)$ |
| Eliminación | $O(\log n)$ | $O(n)$ |

> El peor caso $O(n)$ ocurre cuando el árbol degenera en lista (inserción de datos ordenados).

### 2.4 Eliminación — Tres Casos

**Caso 1**: El nodo a eliminar es una **hoja** → se elimina directamente.

**Caso 2**: El nodo tiene **un hijo** → el hijo sube a ocupar su lugar.

**Caso 3**: El nodo tiene **dos hijos** → se reemplaza con su **sucesor en inorden**
(el nodo más pequeño del subárbol derecho), luego se elimina ese sucesor.

---

## 3. Recorridos de Árboles

Los recorridos visitan **todos los nodos exactamente una vez**.

### 3.1 Recorridos en Profundidad (DFS)

#### Preorden (NLR): Nodo → Izquierda → Derecha
```cpp
void preorden(TNODEPTR p) {
    if (p != NULL) {
        visitar(p);
        preorden(p->left);
        preorden(p->right);
    }
}
```
**Uso**: serializar el árbol, notación prefija de expresiones.

#### Inorden (LNR): Izquierda → Nodo → Derecha
```cpp
void inorden(TNODEPTR p) {
    if (p != NULL) {
        inorden(p->left);
        visitar(p);
        inorden(p->right);
    }
}
```
**Uso**: recorrer BST en orden ascendente, notación infija.

#### Postorden (LRN): Izquierda → Derecha → Nodo
```cpp
void postorden(TNODEPTR p) {
    if (p != NULL) {
        postorden(p->left);
        postorden(p->right);
        visitar(p);
    }
}
```
**Uso**: liberar memoria, evaluar expresiones, notación posfija.

### 3.2 Recorrido en Anchura (BFS)

Visita nodo por nodo, **nivel por nivel**, usando una cola auxiliar.

```cpp
void BFS(TNODEPTR root) {
    queue<TNODEPTR> cola;
    cola.push(root);
    while (!cola.empty()) {
        TNODEPTR actual = cola.front(); cola.pop();
        visitar(actual);
        if (actual->left)  cola.push(actual->left);
        if (actual->right) cola.push(actual->right);
    }
}
```

**Complejidad**: $O(n)$ para todos los recorridos.

---

## 4. Árbol AVL

### 4.1 Definición

Un **árbol AVL** (Adelson-Velsky & Landis, 1962) es un BST en el que el
**factor de equilibrio (FE)** de cada nodo satisface:

$$FE(N) = h(N_{der}) - h(N_{izq}) \in \{-1, 0, +1\}$$

### 4.2 Estructura en C++98

```cpp
struct avl_node {
    int info;
    int altura;           // altura del subárbol
    struct avl_node *father;
    struct avl_node *left;
    struct avl_node *right;
};
```

### 4.3 Las 4 Rotaciones

#### Caso LL — Rotación Simple Derecha
```
    p(-2)         q(0)
   /             / \
  q(-1)   →   A     p(0)
 /
A
```

#### Caso RR — Rotación Simple Izquierda
```
  p(+2)           q(0)
     \            / \
      q(+1)  → p(0)   C
         \
          C
```

#### Caso LR — Rotación Doble (Izq luego Der)
```
    p(-2)     p(-2)       r(0)
   /         /           / \
  q(+1) →  r(-1)  →   q     p
     \     /
      r   q
```

#### Caso RL — Rotación Doble (Der luego Izq)
```
  p(+2)    p(+2)        r(0)
     \        \         / \
      q(-1) →  r(+1) → p     q
     /            \
    r               q
```

### 4.4 Complejidad AVL

| Operación | Complejidad | Garantía |
|-----------|-------------|----------|
| Búsqueda | $O(\log n)$ | Siempre |
| Inserción | $O(\log n)$ | Siempre |
| Eliminación | $O(\log n)$ | Siempre |
| Rotación | $O(1)$ | Siempre |

> La altura de un árbol AVL con $n$ nodos es a lo sumo $1.44 \log_2(n+2) - 0.328$

---

## 5. Árbol Multicamino (Árbol B)

### 5.1 Definición

Un **árbol B de orden $m$** es un árbol de búsqueda multicamino que satisface:
1. Cada nodo tiene como máximo $m$ hijos
2. Cada nodo (excepto la raíz) tiene al menos $\lceil m/2 \rceil$ hijos
3. Todos los nodos hoja están al mismo nivel
4. Las llaves en cada nodo están en orden ascendente

### 5.2 Estructura en C++98 (orden 4)

```cpp
#define ORDEN       4
#define MAX_LLAVES  (ORDEN - 1)   // 3

struct multicamino_node {
    int num_llaves;
    int llaves[MAX_LLAVES];
    struct multicamino_node *hijos[ORDEN];
    struct multicamino_node *padre;
    bool es_hoja;
};
```

### 5.3 Invariante de llaves e hijos

Para un nodo con $k$ llaves $(k_0 < k_1 < \ldots < k_{k-1})$ y $k+1$ hijos:
$$\text{valores en } h_i < k_i \leq \text{valores en } h_{i+1}$$

### 5.4 Inserción con Split

Si se intenta insertar en un nodo lleno ($k = $ MAX_LLAVES):
1. La **llave del medio** sube al padre
2. El nodo se divide en dos nodos con las llaves izquierdas y derechas

### 5.5 Complejidad

| Operación | Complejidad |
|-----------|-------------|
| Búsqueda | $O(\log_m n)$ |
| Inserción | $O(\log_m n)$ |
| Eliminación | $O(\log_m n)$ |

El árbol B es $O(\log_m n)$ en lugar de $O(\log_2 n)$, lo que es significativamente
más rápido para $m$ grande (ej. $m = 1000$ en bases de datos).

---

## 6. Árbol de Expresiones

### 6.1 Estructura

- **Hojas**: operandos (literales, variables)
- **Nodos internos**: operadores binarios (`+`, `-`, `*`, `/`)

### 6.2 Equivalencia recorridos-notaciones

| Recorrido | Notación | Ejemplo |
|-----------|----------|---------|
| Inorden | Infija | `(A + (B * C))` |
| Preorden | Prefija (Polaca) | `+ A * B C` |
| Postorden | Posfija (Polaca Inversa) | `A B C * +` |

### 6.3 Evaluación

Se implementa mediante **postorden** recursivo:
```cpp
double evaluar(EXPRNODEPTR p) {
    if (!p->es_operador) return p->valor;     // hoja: retornar valor
    double izq = evaluar(p->left);
    double der = evaluar(p->right);
    // Aplicar operador...
}
```

---

## 7. Comparativa General

| Árbol | Búsqueda | Inserción | Balance | Nodos por rama | Uso típico |
|-------|----------|-----------|---------|----------------|------------|
| BST | $O(\log n)$ prom. | $O(\log n)$ prom. | No | 2 | Diccionarios simples |
| AVL | $O(\log n)$ | $O(\log n)$ | Estricto | 2 | Búsquedas frecuentes |
| B-Tree | $O(\log_m n)$ | $O(\log_m n)$ | Sí | $m$ | Bases de datos, FS |

---

## 8. Casos de Uso Reales

| Estructura | Aplicación |
|-----------|------------|
| BST | Sets y Maps de C++ STL |
| AVL | Bases de datos en memoria (ej. AVL tree en algunos RDBMS) |
| Árbol B | Índices de MySQL/PostgreSQL, sistemas de archivos (NTFS, ext4) |
| Árbol de expresiones | Compiladores, parsers, hojas de cálculo |

---

## 9. Resumen de Fórmulas

$$\text{Altura mínima BST} = \lceil \log_2(n+1) \rceil - 1$$

$$\text{Altura máxima AVL} \leq 1.44 \cdot \log_2 n$$

$$\text{Nodos en árbol completo de altura } h = 2^{h+1} - 1$$

$$\text{FE}(N) = h(N_{der}) - h(N_{izq})$$

$$\text{Número de comparaciones en BST balanceado} \approx \log_2 n$$
