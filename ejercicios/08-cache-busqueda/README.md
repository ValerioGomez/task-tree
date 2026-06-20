# Ejercicio 08 — Caché de Búsqueda (Memoización)

## 🎯 Objetivo

Implementar un **caché de búsqueda** sobre un BST para que la segunda vez que se busque
un valor, la respuesta sea **inmediata** sin recorrer el árbol.

---

## 🧠 El Problema

```
Árbol con 1000 nodos. Búsqueda de 15 → recorre ~10 nodos (log₂1000 ≈ 10)
Si se busca 15 otra vez → vuelve a recorrer los mismos 10 nodos
Si se busca 15 mil veces → recorre 10.000 nodos innecesariamente
```

**Solución**: guardar el resultado de la primera búsqueda en una tabla de acceso rápido.

---

## 💡 El Concepto: Memoización

> **Memoización** = recordar el resultado de una operación costosa para reutilizarlo.

```
Primera búsqueda de 15:
  [15] no está en caché → recorre el árbol → encuentra nodo
  → GUARDA en caché: {15 → puntero al nodo}

Segunda búsqueda de 15:
  [15] ESTÁ en caché → retorna puntero inmediatamente → O(1)
  ✅ Sin recorrer el árbol
```

---

## 🔧 Tres Implementaciones

### 1️⃣ Tabla Hash Simple — O(1)

```
hash(clave) = clave % 64       ← función de dispersión

Tabla:  [0]→NULL  [7]→{7, nodo7}  [15]→{15, nodo15}  ...

Ventaja:  lookup O(1) — inmediato
Limitación: sin límite de tamaño (puede crecer indefinidamente)
```

### 2️⃣ std::map de C++98 — O(log n)

```cpp
map<int, TNODEPTR> cache;

// Guardar:
cache[15] = nodo15;

// Buscar:
if (cache.count(15)) return cache[15];  // HIT
```

Más simple de implementar, usa un árbol rojo-negro internamente.

### 3️⃣ Caché LRU — O(n) con descarte automático

```
Capacidad máxima: 4 entradas
Estado inicial:  []

Buscar 7:  MISS → [7]
Buscar 19: MISS → [19, 7]
Buscar 15: MISS → [15, 19, 7]
Buscar 4:  MISS → [4, 15, 19, 7]  ← lleno
Buscar 8:  MISS → DESCARTA 7 (el menos usado)
                → [8, 4, 15, 19]
Buscar 7:  MISS → DESCARTA 19
                → [7, 8, 4, 15]
Buscar 15: HIT  → mueve al frente
                → [15, 7, 8, 4]
```

La lista enlazada mantiene el **orden de uso** (frente = más reciente).

---

## 📊 Comparativa de Políticas de Caché

| Política | Descarta | Caso de uso |
|----------|----------|-------------|
| **LRU** (Least Recently Used) | Entrada más antigua sin usar | MySQL, CPU cache |
| **LFU** (Least Frequently Used) | Entrada menos accedida | Sistemas operativos |
| **MRU** (Most Recently Used) | Entrada más reciente | Acceso secuencial |
| **FIFO** | Primera en entrar | Simple, predecible |
| **Sin límite** | Nunca descarta | Memoización pura |

---

## 🌐 Aplicaciones Reales

| Sistema | Qué cachea | Política |
|---------|-----------|----------|
| **MySQL/PostgreSQL** | Páginas de disco (buffer pool) | LRU |
| **CPU L1/L2/L3** | Líneas de memoria RAM | LRU |
| **DNS resolver** | Resultados de consultas DNS | TTL + LRU |
| **Navegadores** | Páginas web, imágenes | LRU + tamaño |
| **Git** | Objetos de repositorio | Sin límite |
| **Programación dinámica** | Subproblemas calculados | Sin límite |

---

## 📈 Salida Esperada

```
DEMO 1: CACHE CON TABLA HASH
  [CACHE MISS] buscar(7)  → recorrio el arbol, guardado en cache
  [CACHE MISS] buscar(19) → recorrio el arbol, guardado en cache
  [CACHE MISS] buscar(100)→ recorrio el arbol, guardado en cache
  [CACHE HIT ] buscar(7)  → directo, sin recorrer arbol       ← ¡instantáneo!
  [CACHE MISS] buscar(15) → recorrio el arbol, guardado en cache
  [CACHE HIT ] buscar(19) → directo, sin recorrer arbol
  [CACHE HIT ] buscar(7)  → directo, sin recorrer arbol
  ...

Estadisticas:
  Cache HITS  : 5 (sin recorrer el arbol)
  Cache MISSES: 5 (recorrio el arbol)
  Ahorro      : 50% de busquedas evitadas
```

---

## 🚀 Compilar y Ejecutar

```bash
make && ./cache_busqueda
```
