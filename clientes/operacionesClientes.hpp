#ifndef OPERACIONES_CLIENTES_HPP
#define OPERACIONES_CLIENTES_HPP

// =============================================================================
// operacionesClientes - Logica de negocio del modulo de clientes.
// =============================================================================

void registrarCliente();
void listarClientes();
void buscarClientePorId(int id);
void buscarClientePorNombre(const char* nombre);
void actualizarCliente();
void eliminarCliente();
void historialCliente(int idCliente);

bool identificacionClienteExiste(const char* identificacion, int idExcluir = -1);

#endif
