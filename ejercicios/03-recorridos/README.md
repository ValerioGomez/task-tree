# Ejercicio 03 — Recorridos de Árboles Binarios

## 🎯 Objetivo

Implementar y comparar los **4 recorridos** de un árbol binario, entendiendo cuándo y por qué
se usa cada uno.

## 📊 Los 4 Recorridos

Para el árbol de prueba:
```
          15
         /  \
        4    19
       / \   /
      3   8 16
         / \  \
        5   9  17
```

| Recorrido | Orden | Resultado | Uso principal |
|-----------|-------|-----------|---------------|
| **Preorden** | Nodo → Izq → Der | `15 4 3 8 5 9 19 16 17` | Serializar, prefijo |
| **Inorden** | Izq → Nodo → Der | `3 4 5 8 9 15 16 17 19` | Orden ascendente en BST |
| **Postorden** | Izq → Der → Nodo | `3 5 9 8 4 17 16 19 15` | Liberar, posfijo |
| **Anchura (BFS)** | Nivel por nivel | `15 4 19 3 8 16 5 9 17` | Camino más corto |

## 🔧 Funciones Implementadas

```cpp
void recorridoPreorden(TNODEPTR pNode);
void recorridoInorden(TNODEPTR pNode);
void recorridoPostorden(TNODEPTR pNode);
void recorridoAnchura(TNODEPTR root);          // BFS con std::queue
void recorridoAnchuraNiveles(TNODEPTR root);   // BFS con nivel visible
```

### Funciones adicionales que usan recorridos:
```cpp
int contarNodos(TNODEPTR p);       // O(n) — cuenta todos los nodos
int altura(TNODEPTR p);            // O(n) — altura máxima
int contarHojas(TNODEPTR p);       // O(n) — nodos sin hijos
int sumaValores(TNODEPTR p);       // O(n) — suma de todos los info
int profundidad(TNODEPTR p, int valor, int nivel);  // nivel de un valor
```

## 💡 Implementación de BFS con Cola

El BFS usa `std::queue<TNODEPTR>` (disponible en C++98):

```cpp
void recorridoAnchura(TNODEPTR root) {
    if (root == NULL) return;
    queue<TNODEPTR> cola;
    cola.push(root);                         // 1. Inicializar cola con raíz

    while (!cola.empty()) {
        TNODEPTR actual = cola.front();      // 2. Tomar el frente
        cola.pop();
        cout << actual->info << " ";         // 3. Visitar
        if (actual->left)  cola.push(actual->left);   // 4. Encolar hijos
        if (actual->right) cola.push(actual->right);
    }
}
```

## 🚀 Compilar y Ejecutar

```bash
make && ./recorridos
```

## 📋 Salida Esperada

```
=== 1. PREORDEN (Nodo - Izq - Der) ===
Resultado: 15 4 3 8 5 9 19 16 17

=== 2. INORDEN (Izq - Nodo - Der) ===
Resultado: 3 4 5 8 9 15 16 17 19

=== 3. POSTORDEN (Izq - Der - Nodo) ===
Resultado: 3 5 9 8 4 17 16 19 15

=== 4. ANCHURA - BFS (nivel por nivel) ===
Resultado: 15 4 19 3 8 16 5 9 17

=== 4B. ANCHURA CON NIVELES VISIBLES ===
  Nivel 0: 15
  Nivel 1: 4 19
  Nivel 2: 3 8 16
  Nivel 3: 5 9 17
```
