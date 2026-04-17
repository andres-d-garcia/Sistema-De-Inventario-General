#include "operacionesTransacciones.hpp"
#include <iostream>
#include "Transaccion.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <ctime>

void registrarVenta() {
	std::cout << "Registrar Venta - entrada minima\n";
	Transaccion t("VENTA", 0, 0);

	int idProducto = 0;
	int cantidad = 0;
	float precio = 0.0f;

	std::cout << "Ingrese id producto: "; std::cin >> idProducto;
	std::cout << "Ingrese cantidad: "; std::cin >> cantidad;
	std::cout << "Ingrese precio unitario: "; std::cin >> precio;

	ItemTransaccion item(idProducto, cantidad, precio);
	t.agregarItem(item);
	t.setFechaRegistro(std::time(nullptr));

	int idAsignado = GestorArchivos::guardarRegistro<Transaccion>(RUTA_TRANSACCIONES, t);
	if (idAsignado < 0) std::cout << "Error guardando la transaccion\n";
	else std::cout << "Venta registrada id=" << idAsignado << " total=" << t.getTotal() << "\n";
}
void registrarCompra() {
	std::cout << "Registrar Compra - entrada minima\n";
	Transaccion t("COMPRA", 0, 0);

	int idProducto = 0;
	int cantidad = 0;
	float precio = 0.0f;

	std::cout << "Ingrese id producto: "; std::cin >> idProducto;
	std::cout << "Ingrese cantidad: "; std::cin >> cantidad;
	std::cout << "Ingrese precio unitario: "; std::cin >> precio;

	ItemTransaccion item(idProducto, cantidad, precio);
	t.agregarItem(item);
	t.setFechaRegistro(std::time(nullptr));

	int idAsignado = GestorArchivos::guardarRegistro<Transaccion>(RUTA_TRANSACCIONES, t);
	if (idAsignado < 0) std::cout << "Error guardando la transaccion\n";
	else std::cout << "Compra registrada id=" << idAsignado << " total=" << t.getTotal() << "\n";
}

void listarTransacciones() {
	auto header = GestorArchivos::leerHeader(RUTA_TRANSACCIONES);
	std::cout << "Listado de transacciones (totales fisicos=" << header.cantidadRegistros << ")\n";
	for (int i = 0; i < header.cantidadRegistros; ++i) {
		Transaccion t;
		if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, i, t)) continue;
		if (!t.getEliminado()) t.mostrarInformacionBasica();
	}
}

void buscarTransaccionPorId(int id) {
	int idx = GestorArchivos::buscarIndicePorId<Transaccion>(RUTA_TRANSACCIONES, id);
	if (idx < 0) { std::cout << "Transaccion id="<<id<<" no encontrada\n"; return; }
	Transaccion t;
	if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, idx, t)) {
		std::cout << "Error leyendo transaccion\n"; return;
	}
	t.mostrarInformacionCompleta();
}

void cancelarTransaccion() {
	int id = 0;
	std::cout << "Ingrese id de transaccion a cancelar: "; std::cin >> id;
	int idx = GestorArchivos::buscarIndicePorId<Transaccion>(RUTA_TRANSACCIONES, id);
	if (idx < 0) { std::cout << "Transaccion no encontrada\n"; return; }
	if (GestorArchivos::borradoLogico<Transaccion>(RUTA_TRANSACCIONES, idx))
		std::cout << "Transaccion "<<id<<" cancelada (borrado logico)\n";
	else
		std::cout << "Fallo al cancelar transaccion\n";
}

void reporteResumenGeneral() {
	auto header = GestorArchivos::leerHeader(RUTA_TRANSACCIONES);
	double totalVentas = 0.0, totalCompras = 0.0;
	int countVentas = 0, countCompras = 0;
	for (int i = 0; i < header.cantidadRegistros; ++i) {
		Transaccion t;
		if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, i, t)) continue;
		if (t.getEliminado()) continue;
		if (t.esVenta()) { totalVentas += t.getTotal(); ++countVentas; }
		if (t.esCompra()) { totalCompras += t.getTotal(); ++countCompras; }
	}
	std::cout << "Resumen general:\n";
	std::cout << "  Ventas: "<<countVentas<<" total="<<totalVentas<<"\n";
	std::cout << "  Compras: "<<countCompras<<" total="<<totalCompras<<"\n";
}

void reporteStockCritico() { std::cout << "Reporte stock critico: no implementado\n"; }
void verificarIntegridadReferencial() { std::cout << "Verificacion integridad referencial: no implementado\n"; }

