CXX      = g++
CXXFLAGS = -std=c++98 -Wall -Wextra -pedantic -O2
TARGET1  = duplicados
TARGET2  = expresiones

.PHONY: all clean run1 run2

all: $(TARGET1) $(TARGET2)

$(TARGET1): duplicados.cpp
	$(CXX) $(CXXFLAGS) duplicados.cpp -o $(TARGET1)
	@echo "[OK] Compilado: $(TARGET1)"

$(TARGET2): expresiones.cpp
	$(CXX) $(CXXFLAGS) expresiones.cpp -o $(TARGET2)
	@echo "[OK] Compilado: $(TARGET2)"

run1: $(TARGET1)
	./$(TARGET1)

run2: $(TARGET2)
	./$(TARGET2)

clean:
	rm -f $(TARGET1) $(TARGET2)
