# Ejercicio 04 — Árbol AVL (Balanceado)

## 🎯 Objetivo

Implementar un **Árbol AVL** que se mantiene balanceado automáticamente tras cada inserción
mediante rotaciones, garantizando O(log n) en todas las operaciones.

## 📐 Propiedad AVL

Para todo nodo N del árbol:
```
|altura(N.izquierdo) - altura(N.derecho)| ≤ 1
```

El **factor de equilibrio (FE)** de un nodo es:
```
FE = altura(subárbol_derecho) - altura(subárbol_izquierdo)
```
Un árbol AVL válido tiene FE ∈ {-1, 0, +1} en todos sus nodos.

## 🔄 Las 4 Rotaciones

### Caso LL — Rotación Simple Derecha

Ocurre cuando `FE(p) ≤ -2` y `FE(p.izq) ≤ 0` (inserción en el subárbol izquierdo-izquierdo):

```
      p(FE=-2)          q(FE=0)
     /                 / \
    q(FE=-1)   →     A     p(FE=0)
   /
  A
```

### Caso RR — Rotación Simple Izquierda

Ocurre cuando `FE(p) ≥ +2` y `FE(p.der) ≥ 0` (inserción en subárbol derecho-derecho):

```
  p(FE=+2)          q(FE=0)
     \              / \
      q(FE=+1) → p     C
         \
          C
```

### Caso LR — Rotación Doble Izquierda-Derecha

Ocurre cuando `FE(p) ≤ -2` y `FE(p.izq) > 0`:

```
    p             p             r
   /     Rot←   /     Rot→   /   \
  q      ──→   r      ──→  q       p
   \          /
    r        q
```

### Caso RL — Rotación Doble Derecha-Izquierda

Ocurre cuando `FE(p) ≥ +2` y `FE(p.der) < 0`:

```
  p               p               r
   \      Rot→     \      Rot←   / \
    q      ──→      r     ──→  p     q
   /                 \
  r                    q
```

## 🔧 Funciones Implementadas

| Función | Descripción |
|---------|-------------|
| `getAltura(p)` | Retorna la altura almacenada (-1 para NULL) |
| `actualizarAltura(p)` | Recalcula y almacena la altura |
| `factorEquilibrio(p)` | Calcula FE = h(der) - h(izq) |
| `rotacionDerecha(p)` | Rotación simple LL |
| `rotacionIzquierda(p)` | Rotación simple RR |
| `rotacionDobleIzqDer(p)` | Rotación doble LR |
| `rotacionDobleDerIzq(p)` | Rotación doble RL |
| `balancear(p)` | Aplica la rotación correcta según el FE |
| `insertarAVL(p, x)` | Inserta y rebalancea automáticamente |
| `esAVLValido(p)` | Verifica que todos los FE ∈ {-1, 0, 1} |

## 📊 Comparativa BST vs AVL

| Operación | BST (promedio) | BST (peor caso) | AVL (siempre) |
|-----------|----------------|-----------------|---------------|
| Búsqueda | O(log n) | O(n) | O(log n) |
| Inserción | O(log n) | O(n) | O(log n) |
| Eliminación | O(log n) | O(n) | O(log n) |

El peor caso del BST ocurre cuando se insertan elementos **ordenados** (el árbol degenera en lista).

## 🚀 Compilar y Ejecutar

```bash
make && ./arbol_avl
```
