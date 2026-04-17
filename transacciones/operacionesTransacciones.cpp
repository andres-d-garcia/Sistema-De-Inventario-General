#include "operacionesTransacciones.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include "Transaccion.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../productos/Producto.hpp"
#include "../clientes/Cliente.hpp"
#include "../proveedores/Proveedor.hpp"

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

void reporteStockCritico() {
    std::cout << "\n===========================================================\n";
    std::cout << "REPORTE DE PRODUCTOS CON STOCK CRITICO\n";
    std::cout << "===========================================================\n";
    
    auto header = GestorArchivos::leerHeader(RUTA_PRODUCTOS);
    int productosCriticos = 0;
    int productosSinStock = 0;
    int productosBajoMinimo = 0;
    
    std::cout << "ID\tNombre\t\t\tStock\tStock Minimo\tEstado\n";
    std::cout << "----------------------------------------------------------------\n";
    
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Producto p;
        if (!GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, i, p)) continue;
        if (p.getEliminado()) continue;
        
        int stockActual = p.getStock();
        int stockMinimo = p.getStockMinimo();
        
        if (stockActual <= 0) {
            std::cout << p.getId() << "\t" << std::left << std::setw(20) << p.getNombre() 
                      << "\t" << stockActual << "\t" << stockMinimo << "\tSIN STOCK\n";
            productosCriticos++;
            productosSinStock++;
        } else if (stockActual <= stockMinimo) {
            std::cout << p.getId() << "\t" << std::left << std::setw(20) << p.getNombre() 
                      << "\t" << stockActual << "\t" << stockMinimo << "\tSTOCK BAJO MINIMO\n";
            productosCriticos++;
            productosBajoMinimo++;
        }
    }
    
    if (productosCriticos == 0) {
        std::cout << "No hay productos con stock critico.\n";
    } else {
        std::cout << "----------------------------------------------------------------\n";
        std::cout << "RESUMEN:\n";
        std::cout << "  Total productos criticos: " << productosCriticos << "\n";
        std::cout << "  Productos sin stock: " << productosSinStock << "\n";
        std::cout << "  Productos bajo minimo: " << productosBajoMinimo << "\n";
    }
    std::cout << "===========================================================\n";
}

void verificarIntegridadReferencial() {
    std::cout << "\n===========================================================\n";
    std::cout << "VERIFICACION DE INTEGRIDAD REFERENCIAL\n";
    std::cout << "===========================================================\n";
    
    int erroresEncontrados = 0;
    
    auto headerTransacciones = GestorArchivos::leerHeader(RUTA_TRANSACCIONES);
    auto headerProductos = GestorArchivos::leerHeader(RUTA_PRODUCTOS);
    auto headerClientes = GestorArchivos::leerHeader(RUTA_CLIENTES);
    auto headerProveedores = GestorArchivos::leerHeader(RUTA_PROVEEDORES);
    
    std::cout << "\n--- VERIFICANDO TRANSACCIONES VS PRODUCTOS ---\n";
    
    for (int i = 0; i < headerTransacciones.cantidadRegistros; ++i) {
        Transaccion t;
        if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, i, t)) continue;
        if (t.getEliminado()) continue;
        
        for (int j = 0; j < t.getCantidadItems(); ++j) {
            const ItemTransaccion& item = t.getItem(j);
            int idProducto = item.getIdProducto();
            
            bool productoExiste = false;
            bool productoEliminado = false;
            
            for (int k = 0; k < headerProductos.cantidadRegistros; ++k) {
                Producto p;
                if (!GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, k, p)) continue;
                if (p.getId() == idProducto) {
                    productoExiste = true;
                    if (p.getEliminado()) productoEliminado = true;
                    break;
                }
            }
            
            if (!productoExiste) {
                std::cout << "ERROR: Transaccion ID " << t.getId() 
                          << " referencia a producto ID " << idProducto 
                          << " que NO EXISTE\n";
                erroresEncontrados++;
            } else if (productoEliminado) {
                std::cout << "ADVERTENCIA: Transaccion ID " << t.getId() 
                          << " referencia a producto ID " << idProducto 
                          << " que esta ELIMINADO LOGICAMENTE\n";
                erroresEncontrados++;
            }
        }
    }
    
    std::cout << "\n--- VERIFICANDO VENTAS VS CLIENTES ---\n";
    
    for (int i = 0; i < headerTransacciones.cantidadRegistros; ++i) {
        Transaccion t;
        if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, i, t)) continue;
        if (t.getEliminado()) continue;
        
        if (t.esVenta() && t.getIdCliente() != 0) {
            int idCliente = t.getIdCliente();
            bool clienteExiste = false;
            bool clienteEliminado = false;
            
            for (int j = 0; j < headerClientes.cantidadRegistros; ++j) {
                Cliente c;
                if (!GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, j, c)) continue;
                if (c.getId() == idCliente) {
                    clienteExiste = true;
                    if (c.getEliminado()) clienteEliminado = true;
                    break;
                }
            }
            
            if (!clienteExiste) {
                std::cout << "ERROR: Venta ID " << t.getId() 
                          << " referencia a cliente ID " << idCliente 
                          << " que NO EXISTE\n";
                erroresEncontrados++;
            } else if (clienteEliminado) {
                std::cout << "ADVERTENCIA: Venta ID " << t.getId() 
                          << " referencia a cliente ID " << idCliente 
                          << " que esta ELIMINADO LOGICAMENTE\n";
                erroresEncontrados++;
            }
        }
    }
    
    std::cout << "\n--- VERIFICANDO COMPRAS VS PROVEEDORES ---\n";
    
    for (int i = 0; i < headerTransacciones.cantidadRegistros; ++i) {
        Transaccion t;
        if (!GestorArchivos::leerRegistroPorIndice<Transaccion>(RUTA_TRANSACCIONES, i, t)) continue;
        if (t.getEliminado()) continue;
        
        if (t.esCompra() && t.getIdProveedor() != 0) {
            int idProveedor = t.getIdProveedor();
            bool proveedorExiste = false;
            bool proveedorEliminado = false;
            
            for (int j = 0; j < headerProveedores.cantidadRegistros; ++j) {
                Proveedor prov;
                if (!GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, j, prov)) continue;
                if (prov.getId() == idProveedor) {
                    proveedorExiste = true;
                    if (prov.getEliminado()) proveedorEliminado = true;
                    break;
                }
            }
            
            if (!proveedorExiste) {
                std::cout << "ERROR: Compra ID " << t.getId() 
                          << " referencia a proveedor ID " << idProveedor 
                          << " que NO EXISTE\n";
                erroresEncontrados++;
            } else if (proveedorEliminado) {
                std::cout << "ADVERTENCIA: Compra ID " << t.getId() 
                          << " referencia a proveedor ID " << idProveedor 
                          << " que esta ELIMINADO LOGICAMENTE\n";
                erroresEncontrados++;
            }
        }
    }
    
    std::cout << "\n--- VERIFICANDO PRODUCTOS VS PROVEEDORES ---\n";
    
    for (int i = 0; i < headerProductos.cantidadRegistros; ++i) {
        Producto p;
        if (!GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, i, p)) continue;
        if (p.getEliminado()) continue;
        
        int idProveedor = p.getIdProveedor();
        if (idProveedor != 0) {
            bool proveedorExiste = false;
            
            for (int j = 0; j < headerProveedores.cantidadRegistros; ++j) {
                Proveedor prov;
                if (!GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, j, prov)) continue;
                if (prov.getId() == idProveedor && !prov.getEliminado()) {
                    proveedorExiste = true;
                    break;
                }
            }
            
            if (!proveedorExiste) {
                std::cout << "ERROR: Producto ID " << p.getId() 
                          << " (" << p.getNombre() << ")"
                          << " referencia a proveedor ID " << idProveedor 
                          << " que NO EXISTE o esta ELIMINADO\n";
                erroresEncontrados++;
            }
        }
    }
    
    std::cout << "\n===========================================================\n";
    if (erroresEncontrados == 0) {
        std::cout << "VERIFICACION COMPLETADA: No se encontraron problemas de integridad.\n";
    } else {
        std::cout << "VERIFICACION COMPLETADA: Se encontraron " << erroresEncontrados 
                  << " problema(s) de integridad referencial.\n";
    }
    std::cout << "===========================================================\n";
}