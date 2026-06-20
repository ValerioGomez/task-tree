# Ejercicio 07 — Árboles B, B+ y B*

## 🎯 Objetivo

Entender las **tres variantes** del árbol B, sus diferencias estructurales y cuándo usar cada uno.

---

## 📊 CUADRO COMPARATIVO: ÁRBOLES B, B+ Y B*

| CARACTERÍSTICA | ÁRBOL B | ÁRBOL B+ | ÁRBOL B* |
|----------------|---------|----------|----------|
| 📌 **Definición** | Árbol balanceado donde cada nodo (interno y hoja) contiene llaves y datos completos | Árbol balanceado donde los nodos internos SOLO contienen llaves guía; los datos están en las hojas | Variante del B+ que mantiene los nodos al 66% de ocupación (vs 50%) mediante redistribución entre hermanos |
| 🏗️ **Estructura Nodos Internos** | Contienen: Llaves + Datos + Punteros a hijos | Contienen: SOLO Llaves guía + Punteros a hijos | Igual que B+ pero con mayor densidad (≈66% ocupación) |
| 🌿 **Estructura Hojas** | Contienen: Llaves + Datos completos | Contienen: Llaves + Datos completos + Punteros siguiente y anterior | Igual que B+ pero con mayor ocupación |
| 🔗 **Enlace entre Hojas** | ❌ No | ✅ Sí (lista doblemente enlazada) | ✅ Sí (lista doblemente enlazada) |
| 📈 **Factor de Ocupación** | 50% (mínimo) | 50% (mínimo) | 66% (mínimo, ≈2/3) |
| 🗄️ **Uso de Memoria/Disco** | Medio | Medio | Alto (más compacto) |
| 🔍 **Búsqueda por Clave** | O(logₘ n) | O(logₘ n) | O(logₘ n) |
| 📊 **Búsqueda por Rango** | O(k · logₘ n) — necesita buscar cada elemento | O(logₘ n + k) — encuentra inicio y recorre hojas | O(logₘ n + k) — igual que B+ |
| ⚡ **Velocidad de Lectura** | Media | Alta (en rangos) | Alta (en rangos) |
| ✍️ **Velocidad de Escritura** | Media | Media | Lenta (por redistribución) |
| 🔄 **Costo de Inserción** | Medio (split simple) | Medio (split simple) | Alto (redistribución + split) |
| 🗑️ **Costo de Eliminación** | Medio (fusión/redistribución) | Medio (fusión/redistribución) | Alto (requiere mantener 66%) |
| 📦 **Número de Niveles** | 3-4 para 1M registros | 3-4 para 1M registros | 2-3 para 1M registros (más compacto) |
| 💾 **Espacio en Disco** | 200 MB (1M reg, 100B c/u) | 200 MB + punteros extra | 150 MB (mayor densidad) |
| 🎯 **Caso de Uso Principal** | Sistemas de archivos, índices simples | Bases de datos relacionales (PostgreSQL, MySQL) | Sistemas de archivos con alta compresión |
| 🏢 **Usado en** | Sistemas de archivos antiguos, algunos NoSQL | PostgreSQL, MySQL, Oracle, SQLite | Sistemas de archivos modernos (ej: HFS+) |
| 🔧 **Complejidad Implementación**| Media | Media-Alta | Alta |
| 📖 **Acceso Secuencial** | ❌ No optimizado | ✅ Excelente (punteros entre hojas) | ✅ Excelente (punteros entre hojas) |
| 🧩 **Altura del Árbol** | h = logₘ N | h = logₘ N (menor en hojas) | h < logₘ N (por mayor densidad) |
| 🏷️ **Llaves Duplicadas** | ❌ No permitidas | ✅ Permitidas (en hojas) | ✅ Permitidas (en hojas) |
| 🎭 **Rol de Nodos Internos** | Contienen datos, participan en búsqueda | Solo señales de tráfico (guías) | Solo señales de tráfico (guías) |
| 🔄 **Redistribución** | Solo en eliminación | Solo en eliminación | Antes de cada inserción (prestamismo) |

---

## 🎯 DECISIONES DE DISEÑO: ¿CUÁL USAR?

### Elige ÁRBOL B si:
- ✅ Necesitas acceso aleatorio y búsquedas puntuales
- ✅ No necesitas búsquedas por rango frecuentes
- ✅ Tienes pocas inserciones/eliminaciones
- ✅ Sistema de archivos simple
- ✅ Implementación más sencilla

### Elige ÁRBOL B+ si:
- ✅ Queries BETWEEN son comunes (PostgreSQL)
- ✅ SELECT con ORDER BY frecuentes
- ✅ Escaneo secuencial de datos (full table scan)
- ✅ Necesitas múltiples índices en la misma tabla
- ✅ Motor de base de datos relacional

### Elige ÁRBOL B* si:
- ✅ Espacio en disco es crítico (embebidos, IoT)
- ✅ Pocas escrituras, muchas lecturas
- ✅ Sistema de archivos con alta compresión
- ✅ Necesitas máxima densidad de datos
- ✅ Hardware con almacenamiento limitado

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
