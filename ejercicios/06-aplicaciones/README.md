# Ejercicio 06 — Aplicaciones de Árboles

## 🎯 Objetivo

Resolver dos problemas prácticos usando árboles binarios como estructura de datos de soporte.

---

## 📌 Aplicación 1: Encontrar Duplicados (`duplicados.cpp`)

### Problema
Dado el array `{15,4,8,7,4,3,19,5,7,9,16,5,17}`, detectar qué valores se repiten.

### Algoritmo (del enunciado)

```cpp
TNODEPTR tree = makeTree(numbers[0]);

for (int i = 1; i < 13; i++) {
    TNODEPTR p = tree;
    TNODEPTR q = tree;

    while (numbers[i] != p->info && q != NULL) {
        p = q;
        if (numbers[i] < p->info)
            q = p->left;
        else
            q = p->right;
    }

    if (numbers[i] == p->info)
        cout << "Número repetido: " << numbers[i] << endl;
    else if (numbers[i] < p->info)
        setLeftChild(p, numbers[i]);
    else
        setRightChild(p, numbers[i]);
}
```

### Duplicados detectados
```
Número 4  → repetido (pos 1 y 4)
Número 7  → repetido (pos 3 y 8)
Número 5  → repetido (pos 7 y 11)
```

### Compilar y ejecutar
```bash
make duplicados
./duplicados
```

---

## 📌 Aplicación 2: Árbol de Expresiones (`expresiones.cpp`)

### Estructura del árbol
- **Hojas**: operandos (valores numéricos o nombres de variables)
- **Nodos internos**: operadores (`+`, `-`, `*`, `/`)

### Expresión 1: `A + B * C` (A=2, B=3, C=4 → resultado=14)
```
      +
     / \
    A   *
       / \
      B   C
```

### Expresión 2: `(A + B) * C` (A=2, B=3, C=4 → resultado=20)
```
      *
     / \
    +   C
   / \
  A   B
```

### Por qué los árboles representan correctamente la precedencia

La **estructura del árbol** codifica la precedencia de operadores.
En `A + B * C`, el `*` está más profundo que `+`, por lo que se evalúa primero.

### Los 3 recorridos generan las 3 notaciones

| Recorrido | Notación | `A + B * C` |
|-----------|----------|-------------|
| Inorden | Infija | `(A+(B*C))` |
| Preorden | Prefija | `+ A * B C` |
| Postorden | Posfija | `A B C * +` |

La **notación posfija** (Reverse Polish Notation) es la que usan las calculadoras HP
y muchos compiladores, porque no requiere paréntesis y es fácil de evaluar con una pila.

### Compilar y ejecutar
```bash
make expresiones
./expresiones
```

---

## 🚀 Compilar ambas aplicaciones

```bash
make all

# Ejecutar
./duplicados
./expresiones
```
