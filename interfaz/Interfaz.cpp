#include "Interfaz.hpp"
#include "../productos/operacionesProductos.hpp"
#include "../proveedores/operacionesProveedores.hpp"
#include "../clientes/operacionesClientes.hpp"
#include "../transacciones/operacionesTransacciones.hpp"
#include "../tienda/Tienda.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <limits>

using namespace std;

// =============================================================================
// Utilidades internas
// =============================================================================

int Interfaz::leerOpcion() {
    int op = -1;
    string input;
    cin >> input;
    Formatos::limpiarBuffer();
    try { op = stoi(input); } catch (...) { op = -1; }
    return op;
}

void Interfaz::mostrarEncabezado() {
    Tienda t;
    Formatos::imprimirLinea(70, '=');
    setColor(COLOR_CYAN);
    if (GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)) {
        cout << "   SISTEMA DE INVENTARIO | "
             << t.getNombre() << " | RIF: " << t.getRif() << endl;
    } else {
        cout << "   SISTEMA DE INVENTARIO" << endl;
    }
    setColor(COLOR_RESET);
    Formatos::imprimirLinea(70, '=');
}

// =============================================================================
// Punto de entrada
// =============================================================================

void Interfaz::ejecutar() {
    // Si no hay tienda configurada, pedirla antes de entrar al loop
    Tienda t;
    if (!GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)
        || strlen(t.getNombre()) == 0) {
        cout << "Bienvenido. Configure su tienda antes de continuar." << endl;
        menuConfigurarTienda();
    }

    int op;
    do {
        Formatos::limpiarPantalla();
        mostrarEncabezado();
        setColor(COLOR_BLANCO);
        cout << "  1. Gestion de Productos"    << endl;
        cout << "  2. Gestion de Proveedores"  << endl;
        cout << "  3. Gestion de Clientes"     << endl;
        cout << "  4. Gestion de Transacciones"<< endl;
        cout << "  5. Reportes y Mantenimiento"<< endl;
        cout << "  6. Configurar Tienda"        << endl;
        cout << "  0. Salir"                   << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea();
        setColor(COLOR_AMARILLO);
        cout << "  Ingresa una opcion: ";
        setColor(COLOR_RESET);
        op = leerOpcion();

        switch (op) {
            case 1: menuProductos();        break;
            case 2: menuProveedores();      break;
            case 3: menuClientes();         break;
            case 4: menuTransacciones();    break;
            case 5: menuReportes();         break;
            case 6: menuConfigurarTienda(); break;
            case 0: cout << "Saliendo del sistema...\n"; break;
            default: cout << "Opcion invalida.\n"; Formatos::pausar();
        }
    } while (op != 0);
}

// =============================================================================
// Submenu Productos
// =============================================================================

void Interfaz::menuProductos() {
    int op;
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_CYAN);
        cout << "            GESTION DE PRODUCTOS" << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_BLANCO);
        cout << "  1. Nuevo producto"    << endl;
        cout << "  2. Listar productos"  << endl;
        cout << "  3. Buscar por ID"     << endl;
        cout << "  4. Buscar por nombre" << endl;
        cout << "  5. Actualizar"        << endl;
        cout << "  6. Eliminar"          << endl;
        cout << "  0. Volver"            << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea();
        setColor(COLOR_AMARILLO);
        cout << "  Ingresa una opcion: ";
        setColor(COLOR_RESET);
        op = leerOpcion();

        switch (op) {
            case 1: registrarProducto(); break;
            case 2: listarProductos();   break;
            case 3: {
                int id;
                if (Validaciones::leerEntero("ID: ", id)) buscarProductoPorId(id);
                break;
            }
            case 4: {
                char n[100];
                if (Validaciones::leerCadena("Nombre: ", n, 100)) buscarProductoPorNombre(n);
                break;
            }
            case 5: actualizarProducto(); break;
            case 6: eliminarProducto();   break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) Formatos::pausar();
    } while (op != 0);
}

// =============================================================================
// Submenu Proveedores
// =============================================================================

void Interfaz::menuProveedores() {
    int op;
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_CYAN);
        cout << "           GESTION DE PROVEEDORES" << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_BLANCO);
        cout << "  1. Nuevo proveedor"   << endl;
        cout << "  2. Listar"            << endl;
        cout << "  3. Buscar por ID"     << endl;
        cout << "  4. Buscar por nombre" << endl;
        cout << "  5. Actualizar"        << endl;
        cout << "  6. Eliminar"          << endl;
        cout << "  0. Volver"            << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea();
        setColor(COLOR_AMARILLO);
        cout << "  Ingresa una opcion: ";
        setColor(COLOR_RESET);
        op = leerOpcion();

        switch (op) {
            case 1: registrarProveedor(); break;
            case 2: listarProveedores();  break;
            case 3: {
                int id;
                if (Validaciones::leerEntero("ID: ", id)) buscarProveedorPorId(id);
                break;
            }
            case 4: {
                char n[100];
                if (Validaciones::leerCadena("Nombre: ", n, 100)) buscarProveedorPorNombre(n);
                break;
            }
            case 5: actualizarProveedor(); break;
            case 6: eliminarProveedor();   break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) Formatos::pausar();
    } while (op != 0);
}

// =============================================================================
// Submenu Clientes
// =============================================================================

void Interfaz::menuClientes() {
    int op;
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_CYAN);
        cout << "             GESTION DE CLIENTES" << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_BLANCO);
        cout << "  1. Nuevo cliente"     << endl;
        cout << "  2. Listar"            << endl;
        cout << "  3. Buscar por ID"     << endl;
        cout << "  4. Buscar por nombre" << endl;
        cout << "  5. Actualizar"        << endl;
        cout << "  6. Eliminar"          << endl;
        cout << "  7. Ver historial"     << endl;
        cout << "  0. Volver"            << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea();
        setColor(COLOR_AMARILLO);
        cout << "  Ingresa una opcion: ";
        setColor(COLOR_RESET);
        op = leerOpcion();

        switch (op) {
            case 1: registrarCliente(); break;
            case 2: listarClientes();   break;
            case 3: {
                int id;
                if (Validaciones::leerEntero("ID: ", id)) buscarClientePorId(id);
                break;
            }
            case 4: {
                char n[100];
                if (Validaciones::leerCadena("Nombre: ", n, 100)) buscarClientePorNombre(n);
                break;
            }
            case 5: actualizarCliente(); break;
            case 6: eliminarCliente();   break;
            case 7: {
                int id;
                if (Validaciones::leerEntero("ID del cliente: ", id)) historialCliente(id);
                break;
            }
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) Formatos::pausar();
    } while (op != 0);
}

// =============================================================================
// Submenu Transacciones
// =============================================================================

void Interfaz::menuTransacciones() {
    int op;
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_CYAN);
        cout << "          GESTION DE TRANSACCIONES" << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_BLANCO);
        cout << "  1. Registrar venta"       << endl;
        cout << "  2. Registrar compra"      << endl;
        cout << "  3. Listar transacciones"  << endl;
        cout << "  4. Buscar por ID"         << endl;
        cout << "  5. Cancelar transaccion"  << endl;
        cout << "  0. Volver"                << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea();
        setColor(COLOR_AMARILLO);
        cout << "  Ingresa una opcion: ";
        setColor(COLOR_RESET);
        op = leerOpcion();

        switch (op) {
            case 1: registrarVenta();       break;
            case 2: registrarCompra();      break;
            case 3: listarTransacciones();  break;
            case 4: {
                int id;
                if (Validaciones::leerEntero("ID: ", id)) buscarTransaccionPorId(id);
                break;
            }
            case 5: cancelarTransaccion(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) Formatos::pausar();
    } while (op != 0);
}

// =============================================================================
// Submenu Reportes y Mantenimiento
// =============================================================================

void Interfaz::menuReportes() {
    int op;
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_CYAN);
        cout << "          REPORTES Y MANTENIMIENTO" << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea(70, '=');
        setColor(COLOR_BLANCO);
        cout << "  1. Resumen general"              << endl;
        cout << "  2. Productos con stock critico"  << endl;
        cout << "  3. Verificar integridad"         << endl;
        cout << "  4. Crear backup"                 << endl;
        cout << "  0. Volver"                       << endl;
        setColor(COLOR_RESET);
        Formatos::imprimirLinea();
        setColor(COLOR_AMARILLO);
        cout << "  Ingresa una opcion: ";
        setColor(COLOR_RESET);
        op = leerOpcion();

        switch (op) {
            case 1: reporteResumenGeneral();        break;
            case 2: reporteStockCritico();          break;
            case 3: verificarIntegridadReferencial(); break;
            case 4: GestorArchivos::crearBackup();  break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) Formatos::pausar();
    } while (op != 0);
}

// =============================================================================
// Configurar Tienda
// =============================================================================

void Interfaz::menuConfigurarTienda() {
    Formatos::limpiarPantalla();
    Tienda t;
    bool existe = GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)
                  && strlen(t.getNombre()) > 0;

    if (existe) {
        t.mostrarInformacionCompleta();
        cout << "Desea modificar los datos de la tienda? (s/n): ";
        char resp;
        cin >> resp;
        Formatos::limpiarBuffer();
        if (resp != 's' && resp != 'S') return;
    } else {
        cout << "=== CONFIGURACION INICIAL DE LA TIENDA ===" << endl;
        cout << "Ingresa 'cancelar' en cualquier campo para cancelar." << endl;
    }

    char temp[200];

    if (!Validaciones::leerCadena("Nombre de la tienda: ", temp, 100)) return;
    t.setNombre(temp);

    if (!Validaciones::leerCadena("RIF (J-XXXXXXXX-X): ", temp, 20)) return;
    t.setRif(temp);

    if (!Validaciones::leerCadena("Direccion: ", temp, 200)) return;
    t.setDireccion(temp);

    if (!Validaciones::leerCadena("Telefono: ", temp, 20)) return;
    t.setTelefono(temp);

    if (!Validaciones::leerCadena("Email: ", temp, 100)) return;
    if (!Validaciones::validarEmail(temp)) return;
    t.setEmail(temp);

    t.actualizarFechaModificacion();

    bool ok;
    if (!existe) {
        // Primera vez: asignar ID y fecha de creacion
        t.setId(1);
        t.setEliminado(false);
        t.setFechaCreacion(time(nullptr));
        ok = GestorArchivos::guardarRegistro<Tienda>(RUTA_TIENDA, t) > 0;
    } else {
        ok = GestorArchivos::actualizarRegistro<Tienda>(RUTA_TIENDA, 0, t);
    }

    if (ok) {
        setColor(COLOR_VERDE);
        cout << "Tienda configurada exitosamente." << endl;
        setColor(COLOR_RESET);
    }
}
