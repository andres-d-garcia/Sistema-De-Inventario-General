#include "operacionesTransacciones.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include "Transaccion.hpp"
#include "../persistencia/GestorArchivos.hpp"

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
    int activos = 0;
    
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Transaccion t;
        if (GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, i, t)) {
            if (!t.getEliminado()) activos++;
        }
    }
    
    std::cout << "===========================================================\n";
    std::cout << "LISTADO DE TRANSACCIONES (" << activos << " activos)\n";
    std::cout << "---\n";
    std::cout << "ID | Tipo | Total | Fecha\n";
    std::cout << "---\n";
    
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Transaccion t;
        if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, i, t)) continue;
        if (!t.getEliminado()) {
            char fechaStr[20];
            time_t fr = t.getFechaRegistro();
            struct tm* timeinfo = localtime(&fr);
            if (timeinfo) strftime(fechaStr, sizeof(fechaStr), "%d/%m/%Y", timeinfo);
            else fechaStr[0] = '\0';
            
            std::cout << "ID: " << t.getId() << " | " << t.getTipo() 
                      << " | Total: " << t.getTotal() 
                      << " | Fecha: " << fechaStr << "\n";
        }
    }
    std::cout << "---\n";
}

void buscarTransaccionPorId(int id) {
    int idx = GestorArchivos::buscarIndicePorId<Transaccion>(RUTA_TRANSACCIONES, id);
    if (idx < 0) { 
        std::cout << "Transaccion con ID " << id << " no encontrada.\n"; 
        return; 
    }
    
    Transaccion t;
    if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, idx, t)) {
        std::cout << "Error leyendo transaccion\n"; 
        return;
    }
    
    if (t.getEliminado()) {
        std::cout << "Transaccion con ID " << id << " esta cancelada (eliminada logicamente).\n";
        return;
    }
    
    char fechaStr[20];
    time_t fr = t.getFechaRegistro();
    struct tm* timeinfo = localtime(&fr);
    if (timeinfo) strftime(fechaStr, sizeof(fechaStr), "%d/%m/%Y", timeinfo);
    else fechaStr[0] = '\0';
    
    std::cout << "\n=== TRANSACCION ENCONTRADA ===\n";
    std::cout << "ID: " << t.getId() << std::endl;
    std::cout << "Tipo: " << t.getTipo() << std::endl;
    std::cout << "Total: $" << t.getTotal() << std::endl;
    std::cout << "Fecha: " << fechaStr << std::endl;
    std::cout << "Descripcion: " << t.getDescripcion() << std::endl;
    std::cout << "Cantidad de items: " << t.getCantidadItems() << std::endl;
    std::cout << "\n--- Items de la transaccion ---\n";
    
    for (int i = 0; i < t.getCantidadItems(); ++i) {
        const ItemTransaccion& item = t.getItem(i);
        std::cout << "Producto ID: " << item.getIdProducto() 
                  << " | Cantidad: " << item.getCantidad()
                  << " | Precio Unitario: $" << item.getPrecioUnitario()
                  << " | Subtotal: $" << item.getSubtotal() << std::endl;
    }
    std::cout << "================================\n";
}

void cancelarTransaccion() {
    int id = 0;
    std::cout << "Ingrese id de transaccion a cancelar: "; std::cin >> id;
    int idx = GestorArchivos::buscarIndicePorId<Transaccion>(RUTA_TRANSACCIONES, id);
    if (idx < 0) { std::cout << "Transaccion no encontrada\n"; return; }
    if (GestorArchivos::borradoLogico<Transaccion>(RUTA_TRANSACCIONES, idx))
        std::cout << "Transaccion " << id << " cancelada (borrado logico)\n";
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
    std::cout << "  Ventas: " << countVentas << " total=" << totalVentas << "\n";
    std::cout << "  Compras: " << countCompras << " total=" << totalCompras << "\n";
}

void reporteStockCritico() { std::cout << "Reporte stock critico: no implementado\n"; }
void verificarIntegridadReferencial() { std::cout << "Verificacion integridad referencial: no implementado\n"; }