# ─────────────────────────────────────────────────────────────────────────────
# Imagen: gcc:12
# Propósito: Entorno de desarrollo C++98 para el proyecto de árboles
# Compilador: g++ -std=c++98
# ─────────────────────────────────────────────────────────────────────────────
FROM gcc:12

LABEL maintainer="Doctorado UNAP - Estructuras de Datos"
LABEL description="Entorno C++98 para proyecto de árboles"

# Instalar herramientas de desarrollo
RUN apt-get update && apt-get install -y \
    make \
    gdb \
    valgrind \
    vim \
    nano \
    less \
    tree \
    && rm -rf /var/lib/apt/lists/*

# Crear alias para compilación C++98 rápida
RUN echo 'alias cpp98="g++ -std=c++98 -Wall -Wextra -pedantic"' >> /etc/bash.bashrc
RUN echo 'alias cpp98dbg="g++ -std=c++98 -Wall -Wextra -pedantic -g -DDEBUG"' >> /etc/bash.bashrc

# Directorio de trabajo
WORKDIR /app

# Copiar todo el proyecto
COPY . .

# Mensaje de bienvenida al iniciar bash
RUN echo 'echo ""' >> /etc/bash.bashrc && \
    echo 'echo "======================================================"' >> /etc/bash.bashrc && \
    echo 'echo "  Entorno C++98 - Proyecto Arboles - Doctorado UNAP"' >> /etc/bash.bashrc && \
    echo 'echo "======================================================"' >> /etc/bash.bashrc && \
    echo 'echo ""' >> /etc/bash.bashrc && \
    echo 'echo "  Comandos utiles:"' >> /etc/bash.bashrc && \
    echo 'echo "    make              - Compilar todo"' >> /etc/bash.bashrc && \
    echo 'echo "    make clean        - Limpiar binarios"' >> /etc/bash.bashrc && \
    echo 'echo "    cpp98 archivo.cpp - Compilar archivo individual"' >> /etc/bash.bashrc && \
    echo 'echo "    tree -L 3         - Ver estructura de carpetas"' >> /etc/bash.bashrc && \
    echo 'echo ""' >> /etc/bash.bashrc

# Verificar que el compilador funciona correctamente
RUN g++ --version

CMD ["/bin/bash"]
