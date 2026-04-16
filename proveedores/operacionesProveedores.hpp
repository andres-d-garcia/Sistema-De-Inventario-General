#ifndef OPERACIONES_PROVEEDORES_HPP
#define OPERACIONES_PROVEEDORES_HPP

// =============================================================================
// operacionesProveedores - Logica de negocio del modulo de proveedores.
// =============================================================================

void registrarProveedor();
void listarProveedores();
void buscarProveedorPorId(int id);
void buscarProveedorPorNombre(const char* nombre);
void actualizarProveedor();
void eliminarProveedor();

bool identificacionProveedorExiste(const char* identificacion, int idExcluir = -1);

#endif
