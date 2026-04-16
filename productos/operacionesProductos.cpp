#include "operacionesProductos.hpp"
#include "Producto.hpp"
#include "../proveedores/Proveedor.hpp"
#include "../tienda/Tienda.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

// =============================================================================
// Utilidades internas
// =============================================================================

bool codigoProductoExiste(const char* codigo, int idExcluir) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PRODUCTOS);
    Producto p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, i, p)
            && !p.getEliminado() && p.getId() != idExcluir) {
            if (Formatos::contieneSubstring(p.getCodigo(), codigo)
                && strlen(p.getCodigo()) == strlen(codigo)) return true;
        }
    }
    return false;
}

void mostrarInfoProveedor(int idProveedor) {
    int idx = GestorArchivos::buscarIndicePorId<Proveedor>(RUTA_PROVEEDORES, idProveedor);
    if (idx == -1) {
        cout << "  Proveedor: [ID:" << idProveedor << " - No encontrado]" << endl;
        return;
    }
    Proveedor prov;
    GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, idx, prov);
    cout << "  Proveedor: " << prov.getNombre()
         << " | RIF: " << prov.getIdentificacion()
         << " | Tel: " << prov.getTelefono() << endl;
}

// =============================================================================
// CRUD de Productos
// =============================================================================

void registrarProducto() {
    Formatos::limpiarPantalla();
    ArchivoHeader hProv = GestorArchivos::leerHeader(RUTA_PROVEEDORES);
    if (hProv.registrosActivos == 0) {
        cout << "ERROR: Debe registrar al menos un proveedor primero." << endl;
        return;
    }

    cout << "=== REGISTRAR NUEVO PRODUCTO ===" << endl;
    cout << "Ingresa 'cancelar' en cualquier campo para cancelar." << endl;

    Producto p;
    char temp[200];

    if (!Validaciones::leerCadena("Codigo: ", temp, 20)) return;
    if (codigoProductoExiste(temp)) {
        cout << "ERROR: Ya existe un producto con ese codigo." << endl;
        return;
    }
    p.setCodigo(temp);

    if (!Validaciones::leerCadena("Nombre: ", temp, 100)) return;
    p.setNombre(temp);

    if (!Validaciones::leerCadena("Descripcion: ", temp, 200)) return;
    p.setDescripcion(temp);

    // Seleccionar proveedor con validacion
    // Listamos proveedores disponibles
    ArchivoHeader hP = GestorArchivos::leerHeader(RUTA_PROVEEDORES);
    Formatos::imprimirLinea();
    cout << "Proveedores disponibles:" << endl;
    Proveedor prov;
    for (int i = 0; i < hP.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, i, prov)
            && !prov.getEliminado()) {
            prov.mostrarInformacionBasica();
        }
    }
    Formatos::imprimirLinea();

    int idProv;
    do {
        if (!Validaciones::leerEntero("ID del proveedor: ", idProv)) return;
        if (GestorArchivos::buscarIndicePorId<Proveedor>(RUTA_PROVEEDORES, idProv) != -1) break;
        cout << "ERROR: Proveedor con ID " << idProv << " no existe." << endl;
    } while (true);
    p.setIdProveedor(idProv);

    float precio;
    if (!Validaciones::leerFlotante("Precio: ", precio)) return;
    if (!p.setPrecio(precio)) return;

    int stock;
    if (!Validaciones::leerEntero("Stock inicial: ", stock)) return;
    p.setStock(stock);

    int stockMin;
    if (!Validaciones::leerEntero("Stock minimo (alerta): ", stockMin)) return;
    p.setStockMinimo(stockMin);

    // Asignar ID y timestamps
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PRODUCTOS);
    p.setId(h.proximoId);
    p.setEliminado(false);
    p.setFechaCreacion(time(nullptr));
    p.actualizarFechaModificacion();

    // Resumen y confirmacion
    Formatos::imprimirLinea();
    cout << "RESUMEN:" << endl;
    p.mostrarInformacionCompleta();
    mostrarInfoProveedor(idProv);
    Formatos::imprimirLinea();

    char conf[5];
    if (!Validaciones::leerCadena("Confirmar? (s/cancelar): ", conf, 5)) return;
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }

    int idAsignado = GestorArchivos::guardarRegistro<Producto>(RUTA_PRODUCTOS, p);
    if (idAsignado > 0) {
        // Actualizar lista de productos del proveedor
        int idxProv = GestorArchivos::buscarIndicePorId<Proveedor>(RUTA_PROVEEDORES, idProv);
        if (idxProv != -1) {
            Proveedor provActual;
            GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, idxProv, provActual);
            provActual.agregarProductoId(idAsignado);
            GestorArchivos::actualizarRegistro<Proveedor>(RUTA_PROVEEDORES, idxProv, provActual);
        }
        // Actualizar contador de la tienda
        Tienda t;
        if (GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)) {
            t.incrementarProductos();
            GestorArchivos::actualizarRegistro<Tienda>(RUTA_TIENDA, 0, t);
        }
        setColor(COLOR_VERDE);
        cout << "Producto creado con ID: " << idAsignado << endl;
        setColor(COLOR_RESET);
    }
}

void listarProductos() {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PRODUCTOS);
    if (h.registrosActivos == 0) {
        cout << "No hay productos registrados." << endl;
        return;
    }
    Formatos::imprimirLinea(70, '=');
    cout << "LISTADO DE PRODUCTOS (" << h.registrosActivos << " activos)" << endl;
    Formatos::imprimirLinea();
    cout << "ID  | Codigo     | Nombre                    | Precio   | Stock" << endl;
    Formatos::imprimirLinea();

    Producto p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, i, p)
            && !p.getEliminado()) {
            p.mostrarInformacionBasica();
        }
    }
    Formatos::imprimirLinea();
}

void buscarProductoPorId(int id) {
    int idx = GestorArchivos::buscarIndicePorId<Producto>(RUTA_PRODUCTOS, id);
    if (idx == -1) { cout << "Producto con ID " << id << " no encontrado." << endl; return; }
    Producto p;
    GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, idx, p);
    p.mostrarInformacionCompleta();
    mostrarInfoProveedor(p.getIdProveedor());
}

void buscarProductoPorNombre(const char* nombre) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PRODUCTOS);
    int encontrados = 0;
    Producto p;
    Formatos::imprimirLinea();
    cout << "Resultados para '" << nombre << "':" << endl;
    Formatos::imprimirLinea();
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, i, p)
            && !p.getEliminado()) {
            if (Formatos::contieneSubstring(p.getNombre(), nombre)) {
                p.mostrarInformacionBasica();
                encontrados++;
            }
        }
    }
    Formatos::imprimirLinea();
    if (encontrados == 0) cout << "Sin resultados." << endl;
    else cout << "Total: " << encontrados << endl;
}

void actualizarProducto() {
    Formatos::limpiarPantalla();
    int id;
    if (!Validaciones::leerEntero("ID del producto a actualizar: ", id)) return;

    int idx = GestorArchivos::buscarIndicePorId<Producto>(RUTA_PRODUCTOS, id);
    if (idx == -1) { cout << "Producto no encontrado." << endl; return; }

    Producto p;
    GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, idx, p);
    cout << "Producto actual:" << endl;
    p.mostrarInformacionCompleta();
    mostrarInfoProveedor(p.getIdProveedor());

    int op;
    do {
        cout << "\n=== ACTUALIZAR PRODUCTO ===" << endl;
        cout << "1. Codigo    2. Nombre    3. Descripcion" << endl;
        cout << "4. Proveedor 5. Precio    6. Stock" << endl;
        cout << "7. StockMin  8. Ajustar Stock (+/-)" << endl;
        cout << "9. Guardar   0. Cancelar" << endl;
        cout << "Opcion: ";
        cin >> op;
        Formatos::limpiarBuffer();

        char temp[200];
        int  tempInt;
        float tempFloat;

        switch (op) {
            case 1:
                if (Validaciones::leerCadena("Nuevo codigo: ", temp, 20)) {
                    if (codigoProductoExiste(temp, p.getId()))
                        cout << "ERROR: Codigo ya existe.\n";
                    else p.setCodigo(temp);
                }
                break;
            case 2:
                if (Validaciones::leerCadena("Nuevo nombre: ", temp, 100)) p.setNombre(temp);
                break;
            case 3:
                if (Validaciones::leerCadena("Nueva descripcion: ", temp, 200)) p.setDescripcion(temp);
                break;
            case 4:
                if (Validaciones::leerEntero("Nuevo ID proveedor: ", tempInt)) {
                    if (GestorArchivos::buscarIndicePorId<Proveedor>(RUTA_PROVEEDORES, tempInt) != -1)
                        p.setIdProveedor(tempInt);
                    else cout << "ERROR: Proveedor no existe.\n";
                }
                break;
            case 5:
                if (Validaciones::leerFlotante("Nuevo precio: ", tempFloat)) p.setPrecio(tempFloat);
                break;
            case 6:
                cout << "Stock actual: " << p.getStock() << endl;
                if (Validaciones::leerEntero("Nuevo stock: ", tempInt)) p.setStock(tempInt);
                break;
            case 7:
                if (Validaciones::leerEntero("Nuevo stock minimo: ", tempInt)) p.setStockMinimo(tempInt);
                break;
            case 8:
                cout << "Stock actual: " << p.getStock() << endl;
                if (Validaciones::leerEntero("Cantidad (+/-): ", tempInt, true)) {
                    p.setStock(p.getStock() + tempInt);
                    cout << "Nuevo stock: " << p.getStock() << endl;
                }
                break;
            case 9:
                p.actualizarFechaModificacion();
                if (GestorArchivos::actualizarRegistro<Producto>(RUTA_PRODUCTOS, idx, p)) {
                    setColor(COLOR_VERDE);
                    cout << "Producto actualizado exitosamente." << endl;
                    setColor(COLOR_RESET);
                }
                break;
            case 0: cout << "Cancelado.\n"; break;
            default: cout << "Opcion invalida.\n";
        }
    } while (op != 0 && op != 9);
}

void eliminarProducto() {
    int id;
    if (!Validaciones::leerEntero("ID del producto a eliminar: ", id)) return;

    int idx = GestorArchivos::buscarIndicePorId<Producto>(RUTA_PRODUCTOS, id);
    if (idx == -1) { cout << "Producto no encontrado." << endl; return; }

    Producto p;
    GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, idx, p);
    p.mostrarInformacionCompleta();

    char conf[5];
    if (!Validaciones::leerCadena("Confirmar eliminacion? (s/cancelar): ", conf, 5)) return;
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }

    if (GestorArchivos::borradoLogico<Producto>(RUTA_PRODUCTOS, idx)) {
        Tienda t;
        if (GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)) {
            t.decrementarProductos();
            GestorArchivos::actualizarRegistro<Tienda>(RUTA_TIENDA, 0, t);
        }
        setColor(COLOR_AMARILLO);
        cout << "Producto eliminado (borrado logico)." << endl;
        setColor(COLOR_RESET);
    }
}

void productosStockCritico() {
    Formatos::limpiarPantalla();
    Formatos::imprimirLinea(70, '=');
    cout << "     PRODUCTOS CON STOCK CRITICO (stock <= stock minimo)" << endl;
    Formatos::imprimirLinea(70, '=');

    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PRODUCTOS);
    Producto p;
    int encontrados = 0;

    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Producto>(RUTA_PRODUCTOS, i, p)
            && !p.getEliminado() && p.tieneStockCritico()) {
            setColor(COLOR_ROJO);
            printf("ID: %-3d | %-25s | Stock: %-5d | Min: %-5d | Deficit: %d\n",
                   p.getId(), p.getNombre(), p.getStock(), p.getStockMinimo(),
                   p.getStockMinimo() - p.getStock());
            setColor(COLOR_RESET);
            mostrarInfoProveedor(p.getIdProveedor());
            encontrados++;
        }
    }
    Formatos::imprimirLinea();
    if (encontrados == 0) cout << "No hay productos con stock critico." << endl;
    else cout << "Total en estado critico: " << encontrados << endl;
}
