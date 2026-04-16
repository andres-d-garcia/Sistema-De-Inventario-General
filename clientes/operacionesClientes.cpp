#include "operacionesClientes.hpp"
#include "Cliente.hpp"
#include "../tienda/Tienda.hpp"
#include "../transacciones/Transaccion.hpp"
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

bool identificacionClienteExiste(const char* identificacion, int idExcluir) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_CLIENTES);
    Cliente c;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, i, c)
            && !c.getEliminado() && c.getId() != idExcluir) {
            char a[20], b[20];
            Formatos::convertirMinusculas(a, c.getIdentificacion());
            Formatos::convertirMinusculas(b, identificacion);
            if (strcmp(a, b) == 0) return true;
        }
    }
    return false;
}

// =============================================================================
// CRUD de Clientes
// =============================================================================

void registrarCliente() {
    Formatos::limpiarPantalla();
    cout << "=== REGISTRAR NUEVO CLIENTE ===" << endl;
    cout << "Ingresa 'cancelar' en cualquier campo para cancelar." << endl;

    Cliente c;
    char temp[200];

    if (!Validaciones::leerCadena("Nombre: ", temp, 100)) return;
    c.setNombre(temp);

    cout << "Cedula (V-12345678 o E-12345678) o 'cancelar': ";
    cin.getline(temp, 20);
    if (strcmp(temp, "cancelar") == 0) { cout << "Cancelado.\n"; return; }
    if (!Validaciones::validarCedula(temp)) return;
    if (identificacionClienteExiste(temp)) {
        cout << "ERROR: Ya existe un cliente con esa cedula." << endl;
        return;
    }
    c.setIdentificacion(temp);

    if (!Validaciones::leerCadena("Telefono: ", temp, 20)) return;
    c.setTelefono(temp);

    if (!Validaciones::leerCadena("Email: ", temp, 100)) return;
    if (!Validaciones::validarEmail(temp)) return;
    c.setEmail(temp);

    if (!Validaciones::leerCadena("Direccion: ", temp, 200)) return;
    c.setDireccion(temp);

    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_CLIENTES);
    c.setId(h.proximoId);
    c.setEliminado(false);
    c.setFechaRegistro(time(nullptr));
    c.actualizarFechaModificacion();

    Formatos::imprimirLinea();
    cout << "RESUMEN:" << endl;
    c.mostrarInformacionCompleta();
    Formatos::imprimirLinea();

    char conf[5];
    if (!Validaciones::leerCadena("Confirmar? (s/cancelar): ", conf, 5)) return;
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }

    int idAsignado = GestorArchivos::guardarRegistro<Cliente>(RUTA_CLIENTES, c);
    if (idAsignado > 0) {
        Tienda t;
        if (GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)) {
            t.incrementarClientes();
            GestorArchivos::actualizarRegistro<Tienda>(RUTA_TIENDA, 0, t);
        }
        setColor(COLOR_VERDE);
        cout << "Cliente creado con ID: " << idAsignado << endl;
        setColor(COLOR_RESET);
    }
}

void listarClientes() {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_CLIENTES);
    if (h.registrosActivos == 0) {
        cout << "No hay clientes registrados." << endl;
        return;
    }
    Formatos::imprimirLinea(70, '=');
    cout << "LISTADO DE CLIENTES (" << h.registrosActivos << " activos)" << endl;
    Formatos::imprimirLinea();
    cout << "ID  | Nombre                    | CI           | Telefono" << endl;
    Formatos::imprimirLinea();

    Cliente c;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, i, c)
            && !c.getEliminado()) {
            c.mostrarInformacionBasica();
        }
    }
    Formatos::imprimirLinea();
}

void buscarClientePorId(int id) {
    int idx = GestorArchivos::buscarIndicePorId<Cliente>(RUTA_CLIENTES, id);
    if (idx == -1) { cout << "Cliente con ID " << id << " no encontrado." << endl; return; }
    Cliente c;
    GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, idx, c);
    c.mostrarInformacionCompleta();
}

void buscarClientePorNombre(const char* nombre) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_CLIENTES);
    int encontrados = 0;
    Cliente c;
    Formatos::imprimirLinea();
    cout << "Resultados para '" << nombre << "':" << endl;
    Formatos::imprimirLinea();
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, i, c)
            && !c.getEliminado()) {
            if (Formatos::contieneSubstring(c.getNombre(), nombre)) {
                c.mostrarInformacionBasica();
                encontrados++;
            }
        }
    }
    Formatos::imprimirLinea();
    if (encontrados == 0) cout << "Sin resultados." << endl;
    else cout << "Total: " << encontrados << endl;
}

void actualizarCliente() {
    Formatos::limpiarPantalla();
    int id;
    if (!Validaciones::leerEntero("ID del cliente a actualizar: ", id)) return;

    int idx = GestorArchivos::buscarIndicePorId<Cliente>(RUTA_CLIENTES, id);
    if (idx == -1) { cout << "Cliente no encontrado." << endl; return; }

    Cliente c;
    GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, idx, c);
    cout << "Cliente actual:" << endl;
    c.mostrarInformacionCompleta();

    int op;
    do {
        cout << "\n=== ACTUALIZAR CLIENTE ===" << endl;
        cout << "1. Nombre    2. Direccion  3. Telefono" << endl;
        cout << "4. Email     5. Cedula     9. Guardar" << endl;
        cout << "0. Cancelar" << endl;
        cout << "Opcion: ";
        cin >> op;
        Formatos::limpiarBuffer();

        char temp[200];

        switch (op) {
            case 1:
                if (Validaciones::leerCadena("Nuevo nombre: ", temp, 100)) c.setNombre(temp);
                break;
            case 2:
                if (Validaciones::leerCadena("Nueva direccion: ", temp, 200)) c.setDireccion(temp);
                break;
            case 3:
                if (Validaciones::leerCadena("Nuevo telefono: ", temp, 20)) c.setTelefono(temp);
                break;
            case 4:
                if (Validaciones::leerCadena("Nuevo email: ", temp, 100)
                    && Validaciones::validarEmail(temp))
                    c.setEmail(temp);
                break;
            case 5:
                cout << "Nueva cedula: ";
                cin.getline(temp, 20);
                if (Validaciones::validarCedula(temp)) {
                    if (identificacionClienteExiste(temp, c.getId()))
                        cout << "ERROR: Ya existe esa cedula." << endl;
                    else c.setIdentificacion(temp);
                }
                break;
            case 9:
                c.actualizarFechaModificacion();
                if (GestorArchivos::actualizarRegistro<Cliente>(RUTA_CLIENTES, idx, c)) {
                    setColor(COLOR_VERDE);
                    cout << "Cliente actualizado exitosamente." << endl;
                    setColor(COLOR_RESET);
                }
                break;
            case 0: cout << "Cancelado.\n"; break;
            default: cout << "Opcion invalida.\n";
        }
    } while (op != 0 && op != 9);
}

void eliminarCliente() {
    int id;
    if (!Validaciones::leerEntero("ID del cliente a eliminar: ", id)) return;

    int idx = GestorArchivos::buscarIndicePorId<Cliente>(RUTA_CLIENTES, id);
    if (idx == -1) { cout << "Cliente no encontrado." << endl; return; }

    Cliente c;
    GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, idx, c);
    c.mostrarInformacionCompleta();

    if (c.getCantidadCompras() > 0) {
        setColor(COLOR_AMARILLO);
        cout << "ADVERTENCIA: Este cliente tiene "
             << c.getCantidadCompras() << " compra(s) registrada(s)." << endl;
        setColor(COLOR_RESET);
    }

    char conf[5];
    if (!Validaciones::leerCadena("Confirmar eliminacion? (s/cancelar): ", conf, 5)) return;
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }

    if (GestorArchivos::borradoLogico<Cliente>(RUTA_CLIENTES, idx)) {
        Tienda t;
        if (GestorArchivos::leerRegistroPorIndice<Tienda>(RUTA_TIENDA, 0, t)) {
            t.decrementarClientes();
            GestorArchivos::actualizarRegistro<Tienda>(RUTA_TIENDA, 0, t);
        }
        setColor(COLOR_AMARILLO);
        cout << "Cliente eliminado (borrado logico)." << endl;
        setColor(COLOR_RESET);
    }
}

void historialCliente(int idCliente) {
    int idxCli = GestorArchivos::buscarIndicePorId<Cliente>(RUTA_CLIENTES, idCliente);
    if (idxCli == -1) { cout << "Cliente no encontrado." << endl; return; }

    Cliente c;
    GestorArchivos::leerRegistroPorIndice<Cliente>(RUTA_CLIENTES, idxCli, c);

    Formatos::imprimirLinea(70, '=');
    cout << "HISTORIAL DE: " << c.getNombre() << endl;
    Formatos::imprimirLinea(70, '=');
    printf("Total gastado: $%.2f | Compras: %d\n",
           c.getTotalGastado(), c.getCantidadCompras());
    Formatos::imprimirLinea();

    float totalAcum = 0;
    for (int i = 0; i < c.getCantidadCompras(); i++) {
        int idxTrans = GestorArchivos::buscarIndicePorId<Transaccion>(
            RUTA_TRANSACCIONES, c.getCompraId(i));
        if (idxTrans != -1) {
            Transaccion t;
            GestorArchivos::leerRegistroPorIndice<Transaccion>(
                RUTA_TRANSACCIONES, idxTrans, t);
            t.mostrarInformacionCompleta();
            totalAcum += t.getTotal();
        }
    }
    Formatos::imprimirLinea();
    printf("Total acumulado: $%.2f\n", totalAcum);
}
