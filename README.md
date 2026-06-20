# 🌳 Proyecto de Estructuras de Datos — Árboles en C++98

## 📋 Descripción General

Este proyecto contiene la implementación desde cero de todas las estructuras de datos basadas en árboles utilizando **C++98 estricto**, desarrollado para el curso de **Estructuras de Datos y Algoritmos Avanzados** del Doctorado UNAP. 

Está diseñado con fines didácticos, con **código documentado 100% en español** (incluyendo nombres de variables, métodos y estructuras como `NodoArbol`, `hijoIzquierdo`, `buscar`, etc.) para máxima comprensión.

Incluye repaso de C++ básico, 8 ejercicios progresivos (desde Árboles Binarios hasta Árboles B*, B+ y Caché LRU), aplicaciones prácticas y un entorno reproducible con Docker y VS Code.

---

## 📁 Estructura del Proyecto

```text
ARBOLES/
├── README.md                   ← Este archivo
├── Makefile                    ← Compilación global
├── docker/
│   ├── Dockerfile              ← Imagen GCC con herramientas para compilar
│   └── docker-compose.yml      ← Entorno de desarrollo aislado
├── repaso/
│   ├── punteros/               ← Repaso de punteros en C++
│   ├── estructuras/            ← Repaso de structs y typedefs
│   └── recursividad/           ← Repaso de funciones recursivas
├── ejercicios/
│   ├── 01-arbol-binario-basico/  ← NodoArbol, crearArbol, construirEjemplo
│   ├── 02-arbol-busqueda/        ← BST: insertar, buscar, eliminar (3 casos)
│   ├── 03-recorridos/            ← Preorden, Inorden, Postorden, BFS
│   ├── 04-avl/                   ← AVL con factor de equilibrio y 4 rotaciones
│   ├── 05-multicamino/           ← Árbol B orden 4 (B-Tree clásico)
│   ├── 06-aplicaciones/          ← Duplicados y árboles de expresiones
│   ├── 07-arboles-b-variantes/   ← Diferencias entre B, B+ y B*
│   └── 08-cache-busqueda/        ← Caché de búsqueda (Memoización, LRU)
└── docs/
    ├── teoria_arboles.md         ← Teoría completa y fórmulas matemáticas
    └── diagramas/                ← Carpeta para diagramas de referencia
```

---

## 🚀 Guía de Reproducibilidad (Paso a Paso)

### 1. Clonar el Repositorio
Abre tu terminal (PowerShell, CMD, o Bash) y clona el proyecto:
```bash
git clone https://github.com/ValerioGomez/doctorado-unap.git
cd "doctorado-unap/PRIMER SEMESTRE/03. ESTRUCTURA DE DATOS Y ALGORITMOS AVANZADOS/TASK 20JUN2026/ARBOLES"
```

### 2. Ejecutar con Docker (El método más seguro y estandarizado)
El proyecto incluye un contenedor con `gcc:12` listo para compilar sin instalar nada en tu sistema anfitrión.

```bash
# Levantar el entorno en segundo plano
docker-compose -f docker/docker-compose.yml up -d

# Entrar al terminal interactivo del contenedor
docker-compose -f docker/docker-compose.yml exec arboles-dev bash

# ¡Listo! Ya estás dentro del contenedor Linux.
# Puedes compilar cualquier ejercicio, por ejemplo el 08:
cd ejercicios/08-cache-busqueda
make
./cache_busqueda
```

### 3. Ejecutar de forma Nativa en VS Code (Windows con MSYS2)
Si prefieres no usar Docker y tienes instalado **MSYS2** (con `g++` en el PATH):

1. Abre VS Code y ve a `Archivo -> Abrir Carpeta`. Selecciona **exactamente** la carpeta `ARBOLES`.
2. Instala la extensión **C/C++ Extension Pack** de Microsoft.
3. Abre cualquier archivo `.cpp` (ej. `arbol_busqueda.cpp`).
4. Presiona **`Ctrl + Shift + B`** y selecciona **`C++98: Compilar y Ejecutar`**.
5. Verás el resultado en el terminal integrado de VS Code.

> **💡 Nota para Windows**: Evita caracteres especiales en el nombre de las carpetas superiores.

---

## 📝 Resumen de los Ejercicios (Todos en C++98)

| # | Ejercicio | Conceptos clave implementados |
|---|-----------|-------------------------------|
| 01 | **Binario Básico** | `struct NodoArbol`, punteros, creación y enlazado manual de nodos. |
| 02 | **BST (Búsqueda)** | Inserción ordenada, búsqueda rápida, y eliminación manejando los 3 casos. |
| 03 | **Recorridos** | Recorridos DFS (Pre/In/Postorden) y BFS (Anchura/Niveles con `std::queue`). |
| 04 | **Árbol AVL** | Auto-balanceo estricto mediante rotaciones simples (LL, RR) y dobles (LR, RL). |
| 05 | **Árbol B (Clásico)**| Multicamino de orden 4, split de nodos internos, crecimiento hacia arriba. |
| 06 | **Aplicaciones** | Detección de duplicados en arrays y evaluación de expresiones matemáticas. |
| 07 | **Árboles B / B+ / B*** | Comparativa teórica y de política de split entre B, B+ (hojas enlazadas) y B*. |
| 08 | **Caché (Memoización)**| Sistema de caché (Hash, Map y LRU) para evitar recorrer el árbol repetidas veces. |

---

## 🛠️ Compilar todo el proyecto de golpe

Si quieres verificar que todo tu entorno está sano, en Linux, Mac, o MSYS2 corre:

```bash
# Desde la raíz del proyecto (ARBOLES/)
make all

# Si quieres limpiar los ejecutables generados
make clean
```

---

## 📚 Documentación y Teoría

Se recomienda encarecidamente leer [`docs/teoria_arboles.md`](docs/teoria_arboles.md) antes de analizar el código. Allí se explica:
- **Diferencias entre B, B+ y B*** (cómo el B* redistribuye antes del split y por qué los B+ se usan en Bases de Datos).
- **Caché y Memoización** (cómo una política LRU descarta la información más antigua).
- **Complejidades Algorítmicas (Big-O)** de todas las operaciones y fórmulas estructurales.

---
*Desarrollado para la clase de Estructuras de Datos Avanzadas. Todos los archivos compilan de forma limpia bajo `g++ -std=c++98 -Wall -Wextra -pedantic`.*
