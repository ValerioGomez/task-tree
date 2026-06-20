# 🌳 Proyecto de Estructuras de Datos — Árboles en C++98

## 📋 Descripción General

Este proyecto contiene la implementación desde cero de todas las estructuras de datos basadas en árboles utilizando **C++98 estricto**, desarrollado para el curso de **Estructuras de Datos y Algoritmos Avanzados** del Doctorado UNAP. 

Está diseñado con fines didácticos (incluyendo nombres de variables, métodos y estructuras como `NodoArbol`, `hijoIzquierdo`, `buscar`, etc.) para máxima comprensión.

Incluye repaso de C++ básico, 8 ejercicios progresivos (desde Árboles Binarios hasta Árboles B*, B+ y Caché LRU), aplicaciones prácticas y un entorno reproducible con Docker y VS Code.

---

## 📁 Estructura del Proyecto

```text
task-tree/
├── README.md                   ← Este archivo
├── Makefile                    ← Compilación global
├── docker/
│   ├── Dockerfile              ← Imagen GCC con herramientas para compilar
│   └── docker-compose.yml      ← Entorno de desarrollo aislado
├── repaso/
│   ├── punteros/               ← Repaso de punteros en C++
│   ├── estructuras/            ← Repaso de structs y typedefs
│   └── recursividad/           ← Repaso de funciones recursivas
├── ejercicios/                 ← Carpeta principal con la guía de estudio (ver abajo)
└── docs/
    ├── teoria_arboles.md         ← Teoría completa y fórmulas matemáticas
    └── diagramas/                ← Carpeta para diagramas de referencia
```

---

## 🚀 Guía de Reproducibilidad (Paso a Paso)

### 1. Clonar el Repositorio
Abre tu terminal (PowerShell, CMD, o Bash) y clona el proyecto:
```bash
git clone https://github.com/ValerioGomez/task-tree.git
cd task-tree
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

1. Abre VS Code y ve a `Archivo -> Abrir Carpeta`. Selecciona **exactamente** la carpeta `task-tree`.
2. Instala la extensión **C/C++ Extension Pack** de Microsoft.
3. Abre cualquier archivo `.cpp` (ej. `arbol_busqueda.cpp` dentro de los ejercicios).
4. Presiona **`Ctrl + Shift + B`** y selecciona **`C++98: Compilar y Ejecutar`**.
5. Verás el resultado en el terminal integrado de VS Code.

---

## 📖 Guía de Estudio y Ejercicios (Tutorial)

Esta sección explica paso a paso cada uno de los ejercicios incluidos en el proyecto. Puedes tratarlos como un tutorial secuencial para entender desde lo más básico hasta los conceptos más avanzados.

### Ejercicio 01: Árbol Binario Básico
- **Ruta:** `ejercicios/01-arbol-binario-basico/`
- **¿Cómo funciona?** Introduce la estructura fundamental `NodoArbol`. Un nodo contiene un valor y punteros a sus hijos izquierdo y derecho. La construcción del árbol se hace de forma manual enlazando punteros, sin algoritmos automáticos.
- **¿Qué se espera?** Entender cómo se relacionan los nodos en memoria mediante punteros y cómo instanciar un árbol simple de forma directa. Al compilar y ejecutar, verás la estructura del árbol impresa, lo que confirma que los enlaces se realizaron correctamente.

### Ejercicio 02: Árbol de Búsqueda (BST)
- **Ruta:** `ejercicios/02-arbol-busqueda/`
- **¿Cómo funciona?** Implementa un Árbol Binario de Búsqueda (BST), donde cada nodo a la izquierda es menor que el nodo raíz y cada nodo a la derecha es mayor. Se incluye la lógica de inserción ordenada, búsqueda rápida y eliminación.
- **¿Qué se espera?** Comprenderás a fondo los 3 casos de eliminación: nodo hoja (sin hijos), nodo con un solo hijo, y nodo con dos hijos (donde se busca el sucesor inorden para reemplazar). El programa de prueba validará la inserción de números y su correcta eliminación mostrando el estado final.

### Ejercicio 03: Recorridos de Árboles
- **Ruta:** `ejercicios/03-recorridos/`
- **¿Cómo funciona?** Implementa diferentes estrategias para visitar los nodos. Incluye recorridos en profundidad (DFS) de forma recursiva: Preorden (Raíz-Izq-Der), Inorden (Izq-Raíz-Der) y Postorden (Izq-Der-Raíz). También implementa el recorrido en anchura (BFS) utilizando una cola (`std::queue`).
- **¿Qué se espera?** Lograrás visualizar cómo la recursividad baja y sube por las ramas del árbol, y cómo una estructura de cola permite explorar el árbol nivel por nivel. Al ejecutarlo, el programa mostrará el orden exacto de visita de los nodos en cada estrategia.

### Ejercicio 04: Árbol AVL (Auto-balanceado)
- **Ruta:** `ejercicios/04-avl/`
- **¿Cómo funciona?** Extiende el concepto del BST agregando un "factor de equilibrio" en cada nodo. Esto evita la degeneración (que el árbol se vuelva una lista enlazada asimétrica). Si un subárbol difiere del otro en más de 1 nivel de altura, se aplican rotaciones para balancearlo.
- **¿Qué se espera?** Entender y poner a prueba las 4 rotaciones: Simples (LL, RR) y Dobles (LR, RL). Al añadir elementos secuenciales que normalmente desbalancearían el árbol (como insertar 1, 2, 3...), el AVL corregirá su estructura automáticamente, manteniendo la complejidad de búsqueda en O(log n).

### Ejercicio 05: Árbol Multicamino (B-Tree Clásico)
- **Ruta:** `ejercicios/05-multicamino/`
- **¿Cómo funciona?** Implementa un Árbol B de orden 4. A diferencia de los árboles binarios, un nodo aquí puede almacenar múltiples claves y apuntar a múltiples hijos. Cuando un nodo se llena, ocurre un evento llamado "split" (división), empujando la clave mediana hacia el padre.
- **¿Qué se espera?** Observar un concepto fundamental de bases de datos: cómo el árbol crece "hacia arriba" al dividirse la raíz, en lugar de crecer hacia abajo. Al ejecutar el código e insertar un gran volumen de datos, verás cómo se dividen las páginas y se mantiene un balance perfecto.

### Ejercicio 06: Aplicaciones de Árboles
- **Ruta:** `ejercicios/06-aplicaciones/`
- **¿Cómo funciona?** Aplica la teoría a problemas algorítmicos concretos, como la detección rápida de elementos duplicados en un flujo de datos y la evaluación de expresiones matemáticas (donde los operadores son nodos internos y los operandos son hojas).
- **¿Qué se espera?** Consolidar los conocimientos previos viendo cómo las estructuras en árbol resuelven problemas reales de forma elegante y altamente optimizada.

### Ejercicio 07: Variantes de Árboles B (B, B+, B*)
- **Ruta:** `ejercicios/07-arboles-b-variantes/`
- **¿Cómo funciona?** Un módulo enfocado en resaltar las diferencias estratégicas (políticas de split y almacenamiento) entre el Árbol B clásico, el B+ (donde los datos reales residen únicamente en las hojas, que están enlazadas como una lista) y el B* (que busca mayor densidad de almacenamiento redistribuyendo datos con nodos hermanos antes de hacer un split).
- **¿Qué se espera?** Aprender a elegir la variante arquitectónica correcta según el caso de uso (como indexación en bases de datos relacionales vs. sistemas de archivos). El código demostrará las diferencias estructurales y de comportamiento en inserciones críticas.

### Ejercicio 08: Caché de Búsqueda (Memoización y LRU)
- **Ruta:** `ejercicios/08-cache-busqueda/`
- **¿Cómo funciona?** Integra una capa de memoria caché utilizando tablas Hash/Map para acelerar las búsquedas en el árbol. Adicionalmente, implementa una política de desalojo LRU (Least Recently Used) para gestionar el tamaño de la caché y descartar los datos más antiguos cuando esta se llena.
- **¿Qué se espera?** Comprobar empíricamente las mejoras de rendimiento. Al buscar el mismo dato varias veces, la primera búsqueda recorrerá el árbol (costoso), pero las subsecuentes recuperarán el dato directamente de la caché (en tiempo O(1)). La terminal mostrará métricas y el historial de desalojo.

---

## 🛠️ Compilación Global

Si quieres verificar que todo tu entorno está sano, en Linux, Mac, o MSYS2 corre:

```bash
# Desde la raíz del proyecto (task-tree/)
make all

# Si quieres limpiar los ejecutables generados
make clean
```

---

## 📚 Documentación Adicional

Se recomienda encarecidamente leer [`docs/teoria_arboles.md`](docs/teoria_arboles.md) para profundizar en:
- **Complejidades Algorítmicas (Big-O)** de todas las operaciones y fórmulas estructurales de los diferentes tipos de árboles.
- **Matemática detrás de las rotaciones** de árboles AVL y los factores de carga en Árboles B*.

---
*Desarrollado para la clase de Estructuras de Datos y Algoritmos Avanzados. Todos los archivos compilan de forma limpia bajo `g++ -std=c++98 -Wall -Wextra -pedantic`.*
