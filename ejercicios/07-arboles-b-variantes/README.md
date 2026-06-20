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

## 🚀 Compilar y Ejecutar

```bash
make && ./b_variantes
```
