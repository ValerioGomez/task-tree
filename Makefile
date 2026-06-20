CXX      = g++
CXXFLAGS = -std=c++98 -Wall -Wextra -pedantic -O2

# ─────────────────────────────────────────────────────────────────────────────
# Ejercicios
# ─────────────────────────────────────────────────────────────────────────────
EXERCISES = \
	ejercicios/01-arbol-binario-basico \
	ejercicios/02-arbol-busqueda \
	ejercicios/03-recorridos \
	ejercicios/04-avl \
	ejercicios/05-multicamino \
	ejercicios/06-aplicaciones

# ─────────────────────────────────────────────────────────────────────────────
# Repaso
# ─────────────────────────────────────────────────────────────────────────────
REPASO_SRCS = \
	repaso/punteros/punteros_ejemplos.cpp \
	repaso/estructuras/structs_ejemplos.cpp \
	repaso/recursividad/recursividad_ejemplos.cpp

.PHONY: all clean repaso ejercicios help

# Objetivo por defecto: compilar todo
all: repaso ejercicios

# Compilar material de repaso
repaso:
	@echo "========================================="
	@echo "  Compilando material de REPASO..."
	@echo "========================================="
	@$(CXX) $(CXXFLAGS) repaso/punteros/punteros_ejemplos.cpp \
		-o repaso/punteros/punteros_ej
	@echo "  [OK] punteros_ejemplos"
	@$(CXX) $(CXXFLAGS) repaso/estructuras/structs_ejemplos.cpp \
		-o repaso/estructuras/structs_ej
	@echo "  [OK] structs_ejemplos"
	@$(CXX) $(CXXFLAGS) repaso/recursividad/recursividad_ejemplos.cpp \
		-o repaso/recursividad/recursividad_ej
	@echo "  [OK] recursividad_ejemplos"
	@echo ""

# Compilar todos los ejercicios
ejercicios:
	@echo "========================================="
	@echo "  Compilando EJERCICIOS..."
	@echo "========================================="
	@for dir in $(EXERCISES); do \
		echo "  --> $$dir"; \
		$(MAKE) -C $$dir; \
	done
	@echo ""
	@echo "  [OK] Todos los ejercicios compilados."

# Limpiar binarios
clean:
	@echo "Limpiando binarios..."
	@rm -f repaso/punteros/punteros_ej
	@rm -f repaso/estructuras/structs_ej
	@rm -f repaso/recursividad/recursividad_ej
	@for dir in $(EXERCISES); do \
		$(MAKE) -C $$dir clean; \
	done
	@echo "  [OK] Limpieza completa."

help:
	@echo ""
	@echo "Uso: make [objetivo]"
	@echo ""
	@echo "  all        - Compilar repaso y todos los ejercicios (por defecto)"
	@echo "  repaso     - Compilar solo los archivos de repaso"
	@echo "  ejercicios - Compilar solo los ejercicios"
	@echo "  clean      - Eliminar todos los binarios generados"
	@echo "  help       - Mostrar esta ayuda"
	@echo ""
