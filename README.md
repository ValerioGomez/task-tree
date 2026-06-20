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

Esta sección explica detalladamente cada uno de los ejercicios incluidos en el proyecto. Puedes tratarlos como un tutorial secuencial para entender desde lo más básico hasta los conceptos más avanzados.

### Ejercicio 01: Árbol Binario Básico
- **Ruta:** `ejercicios/01-arbol-binario-basico/`
- **¿Cómo funciona?** Introduce la estructura fundamental de cualquier árbol binario: `NodoArbol`. La función `crearArbol(int x)` se encarga de instanciar un nodo en memoria dinámica usando `malloc`, inicializando sus punteros en `NULL`. En este nivel inicial, **no hay algoritmos automáticos**; la construcción del árbol se hace de forma manual enlazando punteros explícitamente mediante funciones como `asignarHijoIzquierdo()`.
- **Estructura y Funciones Clave:**
  ```cpp
  struct NodoArbol {
      int dato;
      struct NodoArbol *padre;
      struct NodoArbol *izquierdo;
      struct NodoArbol *derecho;
  };
  ```
  La clase sirve como el "ladrillo" base. Las funciones accesorias como `padre()`, `hijoIzquierdo()`, y booleanas como `esHijoIzquierdo()` sirven para navegar y clasificar la relación entre nodos una vez enlazados manualmente.
- **¿Qué se espera?** Entender cómo se relacionan los nodos en memoria. Al ejecutar el archivo, verás la estructura del árbol impresa en pantalla mediante la recursividad de `imprimirArbol()` (mostrado girado 90 grados) y cómo se verifican las relaciones familiares (como el `hermano()` de un nodo).

### Ejercicio 02: Árbol de Búsqueda (BST)
- **Ruta:** `ejercicios/02-arbol-busqueda/`
- **¿Cómo funciona?** Implementa la regla de oro de los Árboles Binarios de Búsqueda: *Todo nodo a la izquierda es menor que la raíz, y todo nodo a la derecha es mayor o igual.* Aquí entran en juego algoritmos recursivos verdaderos. La función `insertar(NodoPtr pNode, int data)` navega recursivamente decidiendo el camino a tomar, y reasignando el puntero resultante a su padre.
- **Partes de Código Destacadas:**
  La eliminación en `deleteNode(NodoPtr root, int x)` es la operación maestra, manejando rigurosamente 3 escenarios:
  1. **Nodo Hoja:** `root->izquierdo == NULL && root->derecho == NULL`. Se libera directo con `freeNode()`.
  2. **Un solo hijo:** El hijo "sube" a tomar el lugar de su padre.
  3. **Dos hijos:** Se busca el "sucesor inorden" (el nodo más pequeño del subárbol derecho, hallado mediante `minNode()`), se copia su valor en el nodo a eliminar, y luego la recursión elimina al sucesor original en la profundidad.
- **¿Qué se espera?** Comprenderás cómo el árbol mantiene su estado globalmente ordenado tras cada operación. El programa validará el comportamiento borrando nodos con 0, 1 y 2 hijos, mostrando gráficamente el resultado.

### Ejercicio 03: Recorridos de Árboles
- **Ruta:** `ejercicios/03-recorridos/`
- **¿Cómo funciona?** Explora las diferentes estrategias para visitar (o "atravesar") la totalidad de los nodos. Se implementan tanto recorridos en profundidad (DFS) de forma recursiva, como en anchura (BFS) usando iteración y colas.
- **Funciones de Recorrido:**
  - **Inorden (`recorridoInorden`):** Visita `Izquierda → Raíz → Derecha`. Es crítico porque en un BST garantiza que visites los nodos en **orden ascendente estricto**.
  - **Preorden (`recorridoPreorden`):** Visita `Raíz → Izquierda → Derecha`. Muy útil para "clonar" o serializar la estructura topológica del árbol.
  - **Postorden (`recorridoPostorden`):** Visita `Izquierda → Derecha → Raíz`. Vital para funciones como `liberarArbol()`, ya que asegura destruir los sub-árboles hijos antes de destruir al nodo padre.
  - **Anchura (`recorridoAnchura`):** Visita nivel por nivel utilizando una estructura auxiliar nativa de C++ (`std::queue`).
    ```cpp
    queue<NodoPtr> cola;
    cola.push(root);
    // ... dentro de un ciclo while(!cola.empty()):
    NodoPtr actual = cola.front(); 
    cola.pop();
    if (actual->izquierdo != NULL) cola.push(actual->izquierdo);
    if (actual->derecho != NULL) cola.push(actual->derecho);
    ```
- **¿Qué se espera?** Visualizar el impacto de colocar la instrucción `cout` antes, durante o después de las llamadas recursivas y entender para qué algoritmos reales sirve cada enfoque.

### Ejercicio 04: Árbol AVL (Auto-balanceado)
- **Ruta:** `ejercicios/04-avl/`
- **¿Cómo funciona?** Soluciona el peor caso del BST: la degeneración en una lista enlazada (por ej. si insertas `1, 2, 3, 4, 5...`). La estructura del nodo muta a `avl_node` añadiendo un campo `int altura`. Tras cada inserción, la función `balancear()` revisa el **Factor de Equilibrio (FE)** de los nodos:
  `FE = getAltura(p->derecho) - getAltura(p->izquierdo)`
  Si el FE se sale del rango permisible `[-1, 0, 1]`, se gatilla una rotación matemática de punteros.
- **Partes de Código Destacadas (Rotaciones):**
  Existen 4 subrutinas complejas de ajuste de punteros:
  - `rotacionDerecha(AVLNODEPTR p)`: Aplica para desbalances a la izquierda (Left-Left).
  - `rotacionIzquierda(AVLNODEPTR p)`: Para casos de desbalance a la derecha (Right-Right).
  - Y las rotaciones dobles: `rotacionDobleIzqDer` (Left-Right) y `rotacionDobleDerIzq` (Right-Left).
- **¿Qué se espera?** Al insertar números puramente ascendentes en la terminal, el programa alertará automáticamente `[Rotacion Simple IZQUIERDA en nodo X]`, y podrás ver cómo, como por arte de magia, la raíz cambia y la altura permanece en tiempo logarítmico `O(log n)`.

### Ejercicio 05: Árbol Multicamino (B-Tree Clásico)
- **Ruta:** `ejercicios/05-multicamino/`
- **¿Cómo funciona?** Cambia por completo el paradigma del nodo binario. Introducimos un **Árbol B de Orden 4** (2-3-4 tree). La estructura principal, `multicamino_node`, abandona el concepto de un solo dato y adopta arreglos de llaves y punteros.
- **Estructura y Partes Clave:**
  ```cpp
  #define ORDEN 4
  #define MAX_LLAVES (ORDEN - 1)  // 3 llaves como máximo
  struct multicamino_node {
      int num_llaves;             // Contador actual
      int llaves[MAX_LLAVES];     // Arreglo de datos ordenados
      struct multicamino_node *hijos[ORDEN]; // Hasta 4 ramas
      bool es_hoja;
  };
  ```
  La función más trascendental es `splitChild()`. Cuando la función `insertarNoLleno()` detecta que está bajando hacia un nodo que ya tiene `MAX_LLAVES` (3 claves), invoca a `splitChild()`. Este procedimiento divide físicamente el arreglo en dos nodos independientes y **empuja la clave mediana hacia el padre**, previniendo desbordamientos sin romper el ordenamiento.
- **¿Qué se espera?** Dominar la base teórica de cómo se indexan las grandes Bases de Datos Relacionales. A diferencia de un BST que crece extendiendo ramas hacia abajo, observarás que el Árbol B engorda y **crece hacia arriba** (añadiendo niveles desde la nueva raíz producto de un split de la raíz anterior).

### Ejercicio 06: Aplicaciones de Árboles
- **Ruta:** `ejercicios/06-aplicaciones/`
- **¿Cómo funciona?** Archivos dedicados a aplicar la teoría pura en resolver problemas comunes. El ejemplo principal (archivo `duplicados.cpp`) consiste en detectar todos los elementos repetidos de un flujo de entrada. La función `encontrarDuplicados()` intenta insertar en un BST; sin embargo, al recorrer el camino si encuentra que `dato_a_insertar == p->dato`, bloquea la inserción y gatilla una alerta de duplicidad.
- En una variante extendida, el árbol se equipa con un `NodoConteo`:
  ```cpp
  struct NodoConteo {
      int dato;
      int count;   // <--- Acumulador de frecuencias
      // ...
  };
  ```
- **¿Qué se espera?** Apreciarás que usar un árbol binario para buscar duplicados y contar frecuencias evita la necesidad de bucles anidados `for` de `O(n²)`, haciéndolo inmensamente superior al lograr un rendimiento cercano a `O(n log n)`.

### Ejercicio 07: Variantes de Árboles B (B, B+, B*)
- **Ruta:** `ejercicios/07-arboles-b-variantes/`
- **¿Cómo funciona?** Un módulo analítico/teórico enfocado en las sutiles pero enormes diferencias de diseño que marcan a la industria del almacenamiento físico (Discos y SSD):
  - **Árbol B Clásico:** Guarda registros completos en cualquier nodo (interno o hoja).
  - **Árbol B+:** Diseñado para rangos rápidos. Los nodos internos no guardan "datos", solo actúan como "señales de tráfico" (`llaves guía`). Los datos enteros yacen apretados en las **hojas**, las cuales a su vez mantienen un puntero `siguiente` conformando una lista enlazada a nivel de suelo.
  - **Árbol B*:** Su obsesión es la densidad (espacio en disco). Retrasa las costosas divisiones (`splits`) aplicando una técnica de "prestamismo": si un nodo se llena, traslada parte de sus claves a un nodo `hermano` vecino; solo si ambos están llenos se decide hacer un split que envuelva a 2 nodos para crear 3.
- **¿Qué se espera?** Distinguir cuándo un motor de búsqueda como PostgreSQL usa B+ (ideal para queries de tipo `BETWEEN`) vs cuándo un sistema de archivos opta por arquitecturas de alta compresión.

### Ejercicio 08: Caché de Búsqueda (Memoización y LRU)
- **Ruta:** `ejercicios/08-cache-busqueda/`
- **¿Cómo funciona?** Aborda un problema arquitectónico real de servidores: Aunque un árbol AVL es muy rápido `O(log n)`, "saltar" por punteros de RAM incurre en tiempos de búsqueda que, en alta latencia, no son ideales si preguntas por el mismo dato mil veces. La solución acopla un Mapa en Hash frente al Árbol.
  Al buscar un número, primero se busca en la caché (rápido, en `O(1)`). Si no existe (*Cache Miss*), se recorre el árbol y luego se inyecta en la caché. Para evitar que la RAM explote, implementa el algoritmo **LRU (Least Recently Used)**: si la cuota máxima se alcanza, se descarta implacablemente el dato que no haya sido consultado en mayor cantidad de tiempo.
- **¿Qué se espera?** Visualizar mediante analíticas en consola los porcentajes de *Cache Hit* y *Cache Miss*, experimentando con un caso práctico la forma en que arquitecturas modernas (como Memcached o Redis junto a una base SQL) lidian con bases de datos en disco.

---

## ✅ Matriz de Cumplimiento (Requisitos del Ejercicio)

Este proyecto cumple al **100%** con los requisitos y ejercicios propuestos en el material de estudio. A continuación, la ruta de verificación para cada punto solicitado:

### 1. Funciones Primitivas y Árbol Binario (build123)
Todas las operaciones primitivas se desarrollaron con nombres equivalentes en español para facilitar la didáctica, y se encuentran principalmente en el **Ejercicio 01** y **Ejercicio 02**:
- `createNode()` y `freeNode()` ➔ Implementadas en `01/arbol_binario_basico.cpp`.
- `makeTree()` ➔ Implementada como `crearArbol()`.
- `setLeftChild()`, `setRightChild()` ➔ Implementadas como `asignarHijoIzquierdo()` y `asignarHijoDerecho()`.
- `isLeft()`, `isRight()`, `sibling()` ➔ Implementadas como `esHijoIzquierdo()`, `esHijoDerecho()` y `hermano()`.
- `find()`, `insert()`, `deleteNode()` ➔ Implementadas fielmente en `02/arbol_busqueda.cpp`.
- `size()` y `minimum()` ➔ Implementadas en `02/arbol_busqueda.cpp`.
- **Ejercicio `build123`**: Las 3 formas de construir el árbol (asignación directa, variables separadas y función `insert`) están rigurosamente implementadas bajo los nombres `construirEjemplo_1`, `construirEjemplo_2` y `construirEjemplo_3` en el Ejercicio 01.

### 2. Recorridos
- `Preorden`, `Inorden`, `Postorden` ➔ Implementados en `03-recorridos/recorridos.cpp`.
- Se añadió de manera extra el recorrido en `Anchura (BFS)` por niveles usando colas.

### 3. Árbol AVL y Rotaciones
- Implementado en `04-avl/arbol_avl.cpp`.
- Calcula dinámicamente el **Factor de equilibrio**.
- Aplica de forma automática las rotaciones simples (Derecha, Izquierda) y dobles al insertar datos de forma desbalanceada.

### 4. Batería de Pruebas y Diagramas
- **El Array oficial:** El array solicitado `{15,4,8,7,4,3,19,5,7,9,16,5,17}` ha sido incluido por defecto en `02-arbol-busqueda.cpp` y en `06-aplicaciones/duplicados.cpp`.
- **Casos de Eliminación:** El código `02` incluye pruebas aisladas que eliminan explícitamente nodos con 0, 1 y 2 hijos.
- **Diagramas (Impresión):** Para cumplir el requerimiento de diagramación sin dependencias externas, se programaron algoritmos como `imprimirArbol()` e `imprimirAVL()` que grafican de forma elegante y recursiva la estructura de los nodos, incluyendo la impresión del *Factor de Equilibrio* al vuelo en la misma terminal.

### 5. Extensión a Árboles Multicaminos
- Implementado en `05-multicamino/arbol_multicamino.cpp`.
- Usa exactamente la estructura `node` con arreglos de llaves (3) y punteros hijos (4).
- Las funciones requeridas `numTrees()`, `child(i)` y `key(i)` están codificadas y puestas a prueba.
- El recorrido ascendente se ejecuta mediante `traverse()`.

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
