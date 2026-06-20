CXX      = g++
CXXFLAGS = -std=c++98 -Wall -Wextra -pedantic -O2
TARGET   = arbol_avl
SRC      = arbol_avl.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
	@echo "[OK] Compilado: $(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
