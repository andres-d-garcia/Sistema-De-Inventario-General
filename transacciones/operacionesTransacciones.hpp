#ifndef OPERACIONES_TRANSACCIONES_HPP
#define OPERACIONES_TRANSACCIONES_HPP

// =============================================================================
// operacionesTransacciones - Logica de negocio del modulo de transacciones.
// =============================================================================

void registrarVenta();
void registrarCompra();
void listarTransacciones();
void buscarTransaccionPorId(int id);
void cancelarTransaccion();

void reporteResumenGeneral();
void reporteStockCritico();
void verificarIntegridadReferencial();

#endif
