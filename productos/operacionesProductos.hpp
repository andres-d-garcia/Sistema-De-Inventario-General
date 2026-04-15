#ifndef OPERACIONES_PRODUCTOS_HPP
#define OPERACIONES_PRODUCTOS_HPP

// =============================================================================
// operacionesProductos - Logica de negocio del modulo de productos.
// Estas funciones manejan la interaccion con el usuario, llaman a
// GestorArchivos para persistir, y usan la clase Producto.
// =============================================================================

void registrarProducto();
void listarProductos();
void buscarProductoPorId(int id);
void buscarProductoPorNombre(const char* nombre);
void actualizarProducto();
void eliminarProducto();
void productosStockCritico();

// Muestra informacion del proveedor de un producto (usado en detalle)
void mostrarInfoProveedor(int idProveedor);

// Verifica si un codigo ya existe en productos.bin (excluye idExcluir)
bool codigoProductoExiste(const char* codigo, int idExcluir = -1);

#endif // OPERACIONES_PRODUCTOS_HPP
