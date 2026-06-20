# Ejercicio 07 — Árboles B, B+ y B*

## 🎯 Objetivo

Entender las **tres variantes** del árbol B, sus diferencias estructurales y cuándo usar cada uno.

---

## 📊 Comparativa Rápida

| Propiedad | Árbol B | Árbol B+ | Árbol B* |
|-----------|---------|----------|----------|
| **Datos en** | Todos los nodos | **Solo hojas** | Todos los nodos |
| **Nodos internos** | Llaves + datos | **Solo llaves** (guías) | Llaves + datos |
| **Hojas enlazadas** | No | **Sí** (lista enlazada) | No |
| **Mínimo ocupación** | 1/2 | 1/2 | **2/3** |
| **Split** | 1 nodo → 2 | 1 nodo → 2 | Redistribuye primero; split 2→3 |
| **Búsqueda** | O(log n) | O(log n) | O(log n) |
| **Recorrido secuencial** | O(n) lento | **O(n) muy rápido** | O(n) lento |
| **Uso real** | General | Índices de BD (MySQL, PostgreSQL) | NTFS, HFS+ |

---

## 🌳 Árbol B (clásico)

```
         [15|25]
        /   |   \
    [5|10] [20] [30|35]
```

- Los datos (registros) se encuentran en **cualquier nivel**
- Buscar puede terminar en un nodo interno → más rápido en ese caso
- Recorrido inorden requiere bajar y subir constantemente

---

## 🌳 Árbol B+

```
         [15]          ← nodo INTERNO: solo guía, sin datos reales
        /     \
   [5|10|15]  [20|25|30]   ← HOJAS: aquí están los datos
       ↔                    ← enlazadas entre sí
```

**Ventaja clave — recorrido secuencial**: para hacer `SELECT * ORDER BY id` en una base de datos,
solo hay que recorrer la **lista enlazada de hojas** sin subir/bajar por el árbol.

**Por eso MySQL InnoDB, PostgreSQL y SQLite usan B+ para sus índices.**

```
Búsqueda de rango [10..25]:
  1. Bajar por el árbol hasta la hoja con 10  → O(log n)
  2. Recorrer la lista: 10 → 15 → 20 → 25    → O(k) donde k = resultados
```

---

## 🌳 Árbol B*

```
Insertar en nodo lleno:
  B  → split inmediato (1 nodo lleno → 2 nodos al 50%)
  B* → primero intenta mover una llave al hermano (redistribución)
       solo si ambos hermanos están llenos → split de 2 nodos en 3
```

**Consecuencia**: los nodos del B* tienen en promedio **más llaves** → árbol más bajo →
menos accesos a disco.

---

## 💡 ¿Cuándo usar cada uno?

```
¿Necesitas recorridos de rango o secuenciales? → B+  (bases de datos)
¿Sistema de archivos con espacio limitado?      → B*  (NTFS, HFS+)
¿Uso general, código simple?                    → B   (clásico)
```

---

## 📚 TEORÍA DETALLADA

### 1. ÁRBOL B CLÁSICO
**Analogía:** Un archivador donde cada cajón contiene documentos completos.

**Características:**
- Los nodos internos SÍ contienen datos/registros completos.
- Las hojas también contienen datos.
- **Búsqueda:** O(log n) en promedio.
- **Ideal para:** Acceso aleatorio y búsquedas puntuales.
- **Práctica:** Ver carpeta `01-b-clasico`.

### 2. ÁRBOL B+
**Analogía:** Un mapa de carreteras donde las señales (nodos internos) te guían, pero solo las ciudades (hojas) tienen información real.

**Características:**
- **Nodos internos:** SOLO contienen llaves guía (no datos).
- **Hojas:** Contienen TODOS los datos (registros completos).
- **Hojas enlazadas:** Punteros siguiente para recorridos secuenciales.
- **Ideal para:** Búsquedas por rango y queries `BETWEEN`.

**Ventaja clave:**
Para un `SELECT * FROM tabla WHERE id BETWEEN 100 AND 200`:
- **Árbol B:** Recorre nodos internos, encuentra cada registro individualmente.
- **Árbol B+:** Encuentra la hoja inicial (100), luego recorre hojas enlazadas hasta 200 de forma altamente optimizada.
- **Práctica:** Ver carpeta `02-b-plus`.

### 3. ÁRBOL B*
**Analogía:** Un sistema de biblioteca donde si un estante está lleno, primero prestas libros a un estante vecino antes de comprar un estante nuevo.

**Características:**
- Mínimo **2/3 de ocupación** (vs 1/2 en B/B+).
- Redistribución de llaves entre hermanos antes de dividir.
- Mayor densidad → menos altura → menos lecturas de disco.
- **Ideal para:** Sistemas de archivos con alta compresión.

**Algoritmo de redistribución:**
```text
Si nodo N está lleno al insertar:
    1. Buscar hermano H (izquierdo o derecho)
    2. Si H tiene espacio libre (no está lleno):
       - Mover llaves de N a H (redistribuir)
       - Actualizar padres
       - FIN (sin split)
    3. Si H también está lleno:
       - Crear nuevo nodo N2
       - Dividir N + H en 3 nodos (cada uno 2/3 lleno)
       - Actualizar punteros
```
- **Práctica:** Ver carpeta `03-b-star`.

---

## 🚀 Práctica y Ejecución

El código en este ejercicio ha sido separado en subcarpetas para aislar las estructuras:
- `01-b-clasico/`
- `02-b-plus/`
- `03-b-star/`
