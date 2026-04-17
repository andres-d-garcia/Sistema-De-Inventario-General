# =============================================================================
# Makefile - Sistema de Gestion de Inventario (Proyecto 3)
# Compilacion multi-archivo con g++ C++11
# Compatible con Windows (MinGW) y Linux/macOS
# =============================================================================

CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Nombre del ejecutable final
TARGET   = inventario

# Extensión ejecutable en Windows
EXE :=
ifeq ($(OS), Windows_NT)
    EXE := .exe
endif
TARGET := inventario$(EXE)

# Comandos portables (funcionan en entornos MinGW/MSYS/Unix)
MKDIR := mkdir -p
RM    := rm -f
RMDIR := rm -rf

# =============================================================================
# Archivos fuente (.cpp) - uno por modulo
# =============================================================================
SRCS = main.cpp \
       persistencia/GestorArchivos.cpp \
       tienda/Tienda.cpp \
       productos/Producto.cpp \
       productos/operacionesProductos.cpp \
       proveedores/Proveedor.cpp \
       proveedores/operacionesProveedores.cpp \
       clientes/Cliente.cpp \
       clientes/operacionesClientes.cpp \
       transacciones/Transaccion.cpp \
       transacciones/operacionesTransacciones.cpp \
       interfaz/Interfaz.cpp \
       utilidades/Validaciones.cpp \
       utilidades/Formatos.cpp

# Convertir .cpp a .o automaticamente
OBJS = $(SRCS:.cpp=.o)

# =============================================================================
# Regla principal
# =============================================================================
all: datos $(TARGET)

# Crear carpeta datos/ si no existe
datos:
	$(MKDIR)

# Enlazar todos los objetos en el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "======================================"
	@echo " Compilacion exitosa: $(TARGET)"
	@echo "======================================"

# Compilar cada .cpp en su .o correspondiente
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# =============================================================================
# Reglas de utilidad
# =============================================================================

# Limpiar archivos objeto y ejecutable
clean:
	$(RM) $(OBJS) $(TARGET)

# Limpiar todo incluyendo datos binarios generados
cleanall: clean
	$(RMDIR) datos

# Ejecutar directamente despues de compilar
run: all
	./$(TARGET)

# Reconstruir todo desde cero
rebuild: clean all

# Mostrar todos los archivos del proyecto
info:
	@echo "Fuentes: $(SRCS)"
	@echo "Objetos: $(OBJS)"
	@echo "Target:  $(TARGET)"

.PHONY: all clean cleanall run rebuild info datos
