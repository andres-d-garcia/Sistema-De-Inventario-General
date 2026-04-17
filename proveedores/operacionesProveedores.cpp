#include "operacionesProveedores.hpp"
#include "Proveedor.hpp"
#include "../tienda/Tienda.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <cstring>

using namespace std;

// =============================================================================
// Utilidades internas
// =============================================================================

bool identificacionProveedorExiste(const char* identificacion, int idExcluir) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PROVEEDORES);
    Proveedor p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, i, p)
            && !p.getEliminado() && p.getId() != idExcluir) {
            char a[20], b[20];
            Formatos::convertirMinusculas(a, p.getIdentificacion());
            Formatos::convertirMinusculas(b, identificacion);
            if (strcmp(a, b) == 0) return true;
        }
    }
    return false;
}

// =============================================================================
// CRUD de Proveedores
// =============================================================================

void registrarProveedor() {
    Formatos::limpiarPantalla();
    cout << "=== REGISTRAR NUEVO PROVEEDOR ===" << endl;
    cout << "Ingresa 'cancelar' en cualquier campo para cancelar." << endl;

    Proveedor p;
    char temp[200];

    if (!Validaciones::leerCadena("Nombre: ", temp, 100)) return;
    p.setNombre(temp);

    cout << "RIF (J-12345678-9) o 'cancelar': ";
    cin.getline(temp, 20);
    if (strcmp(temp, "cancelar") == 0) { cout << "Cancelado.\n"; return; }
    if (!Validaciones::validarRIF(temp)) return;
    if (identificacionProveedorExiste(temp)) {
        cout << "ERROR: Ya existe un proveedor con ese RIF." << endl;
        return;
    }
    p.setIdentificacion(temp);

    if (!Validaciones::leerCadena("Telefono: ", temp, 20)) return;
    p.setTelefono(temp);

    if (!Validaciones::leerCadena("Email: ", temp, 100)) return;
    if (!Validaciones::validarEmail(temp)) return;
    p.setEmail(temp);

    if (!Validaciones::leerCadena("Direccion: ", temp, 200)) return;
    p.setDireccion(temp);

    // Asignar ID y timestamps
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PROVEEDORES);
    p.setId(h.proximoId);
    p.setEliminado(false);
    p.setFechaRegistro(time(nullptr));
    p.actualizarFechaModificacion();

    Formatos::imprimirLinea();
    cout << "RESUMEN:" << endl;
    p.mostrarInformacionCompleta();
    Formatos::imprimirLinea();

    char conf[5];
    if (!Validaciones::leerCadena("Confirmar? (s/cancelar): ", conf, 5)) return;
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }

    int idAsignado = GestorArchivos::guardarRegistro<Proveedor>(RUTA_PROVEEDORES, p);
    if (idAsignado > 0) {
        Tienda t;
        if (GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)) {
            t.incrementarProveedores();
            GestorArchivos::actualizarRegistro<Tienda>(RUTA_TIENDA, 0, t);
        }
        setColor(COLOR_VERDE);
        cout << "Proveedor creado con ID: " << idAsignado << endl;
        setColor(COLOR_RESET);
    }
}

void listarProveedores() {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PROVEEDORES);
    if (h.registrosActivos == 0) {
        cout << "No hay proveedores registrados." << endl;
        return;
    }
    Formatos::imprimirLinea(70, '=');
    cout << "LISTADO DE PROVEEDORES (" << h.registrosActivos << " activos)" << endl;
    Formatos::imprimirLinea();
    cout << "ID  | Nombre                      | RIF          | Telefono" << endl;
    Formatos::imprimirLinea();

    Proveedor p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, i, p)
            && !p.getEliminado()) {
            p.mostrarInformacionBasica();
        }
    }
    Formatos::imprimirLinea();
}

void buscarProveedorPorId(int id) {
    int idx = GestorArchivos::buscarIndicePorId<Proveedor>(RUTA_PROVEEDORES, id);
    if (idx == -1) { cout << "Proveedor con ID " << id << " no encontrado." << endl; return; }
    Proveedor p;
    GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, idx, p);
    p.mostrarInformacionCompleta();
}

void buscarProveedorPorNombre(const char* nombre) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PROVEEDORES);
    int encontrados = 0;
    Proveedor p;
    Formatos::imprimirLinea();
    cout << "Resultados para '" << nombre << "':" << endl;
    Formatos::imprimirLinea();
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, i, p)
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

void actualizarProveedor() {
    Formatos::limpiarPantalla();
    int id;
    if (!Validaciones::leerEntero("ID del proveedor a actualizar: ", id)) return;

    int idx = GestorArchivos::buscarIndicePorId<Proveedor>(RUTA_PROVEEDORES, id);
    if (idx == -1) { cout << "Proveedor no encontrado." << endl; return; }

    Proveedor p;
    GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, idx, p);
    cout << "Proveedor actual:" << endl;
    p.mostrarInformacionCompleta();

    int op;
    do {
        cout << "\n=== ACTUALIZAR PROVEEDOR ===" << endl;
        cout << "1. Nombre    2. Direccion  3. Telefono" << endl;
        cout << "4. Email     5. RIF        9. Guardar" << endl;
        cout << "0. Cancelar" << endl;
        cout << "Opcion: ";
        cin >> op;
        Formatos::limpiarBuffer();

        char temp[200];

        switch (op) {
            case 1:
                if (Validaciones::leerCadena("Nuevo nombre: ", temp, 100)) p.setNombre(temp);
                break;
            case 2:
                if (Validaciones::leerCadena("Nueva direccion: ", temp, 200)) p.setDireccion(temp);
                break;
            case 3:
                if (Validaciones::leerCadena("Nuevo telefono: ", temp, 20)) p.setTelefono(temp);
                break;
            case 4:
                if (Validaciones::leerCadena("Nuevo email: ", temp, 100)
                    && Validaciones::validarEmail(temp))
                    p.setEmail(temp);
                break;
            case 5:
                cout << "Nuevo RIF: ";
                cin.getline(temp, 20);
                if (Validaciones::validarRIF(temp)) {
                    if (identificacionProveedorExiste(temp, p.getId()))
                        cout << "ERROR: Ya existe ese RIF." << endl;
                    else p.setIdentificacion(temp);
                }
                break;
            case 9:
                p.actualizarFechaModificacion();
                if (GestorArchivos::actualizarRegistro<Proveedor>(RUTA_PROVEEDORES, idx, p)) {
                    setColor(COLOR_VERDE);
                    cout << "Proveedor actualizado exitosamente." << endl;
                    setColor(COLOR_RESET);
                }
                break;
            case 0: cout << "Cancelado.\n"; break;
            default: cout << "Opcion invalida.\n";
        }
    } while (op != 0 && op != 9);
}

void eliminarProveedor() {
    int id;
    if (!Validaciones::leerEntero("ID del proveedor a eliminar: ", id)) return;

    int idx = GestorArchivos::buscarIndicePorId<Proveedor>(RUTA_PROVEEDORES, id);
    if (idx == -1) { cout << "Proveedor no encontrado." << endl; return; }

    Proveedor p;
    GestorArchivos::leerRegistroPorIndice<Proveedor>(RUTA_PROVEEDORES, idx, p);
    p.mostrarInformacionCompleta();

    if (p.getCantidadProductos() > 0) {
        setColor(COLOR_AMARILLO);
        cout << "ADVERTENCIA: Este proveedor tiene "
             << p.getCantidadProductos() << " producto(s) asociado(s)." << endl;
        setColor(COLOR_RESET);
    }

    char conf[5];
    if (!Validaciones::leerCadena("Confirmar eliminacion? (s/cancelar): ", conf, 5)) return;
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }

    if (GestorArchivos::borradoLogico<Proveedor>(RUTA_PROVEEDORES, idx)) {
        Tienda t;
        if (GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)) {
            t.decrementarProveedores();
            GestorArchivos::actualizarRegistro<Tienda>(RUTA_TIENDA, 0, t);
        }
        setColor(COLOR_AMARILLO);
        cout << "Proveedor eliminado (borrado logico)." << endl;
        setColor(COLOR_RESET);
    }
}
