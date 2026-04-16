#include "Cliente.hpp"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

Cliente::Cliente() {
    id = 0;
    memset(nombre,         0, sizeof(nombre));
    memset(direccion,      0, sizeof(direccion));
    memset(telefono,       0, sizeof(telefono));
    memset(email,          0, sizeof(email));
    memset(identificacion, 0, sizeof(identificacion));
    memset(comprasIDs,     0, sizeof(comprasIDs));
    cantidadCompras         = 0;
    totalGastado            = 0.0f;
    eliminado               = false;
    fechaRegistro           = 0;
    fechaUltimaModificacion = 0;
}

Cliente::Cliente(const char* nombre_, const char* identificacion_,
                 const char* telefono_, const char* email_,
                 const char* direccion_) : Cliente() {
    setNombre(nombre_);
    setIdentificacion(identificacion_);
    setTelefono(telefono_);
    setEmail(email_);
    setDireccion(direccion_);
    fechaRegistro = time(nullptr);
}

Cliente::Cliente(const Cliente& otro) {
    id = otro.id;
    strncpy(nombre,         otro.nombre,         sizeof(nombre)         - 1);
    strncpy(direccion,      otro.direccion,      sizeof(direccion)      - 1);
    strncpy(telefono,       otro.telefono,       sizeof(telefono)       - 1);
    strncpy(email,          otro.email,          sizeof(email)          - 1);
    strncpy(identificacion, otro.identificacion, sizeof(identificacion) - 1);
    memcpy(comprasIDs, otro.comprasIDs, sizeof(comprasIDs));
    cantidadCompras         = otro.cantidadCompras;
    totalGastado            = otro.totalGastado;
    eliminado               = otro.eliminado;
    fechaRegistro           = otro.fechaRegistro;
    fechaUltimaModificacion = otro.fechaUltimaModificacion;
}

Cliente::~Cliente() {}


int         Cliente::getId()             const { return id; }
const char* Cliente::getNombre()         const { return nombre; }
const char* Cliente::getDireccion()      const { return direccion; }
const char* Cliente::getTelefono()       const { return telefono; }
const char* Cliente::getEmail()          const { return email; }
const char* Cliente::getIdentificacion() const { return identificacion; }
int         Cliente::getCantidadCompras() const { return cantidadCompras; }
float       Cliente::getTotalGastado()   const { return totalGastado; }
bool        Cliente::getEliminado()      const { return eliminado; }
time_t      Cliente::getFechaRegistro()  const { return fechaRegistro; }
time_t      Cliente::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

int Cliente::getCompraId(int posicion) const {
    if (posicion < 0 || posicion >= cantidadCompras) return -1;
    return comprasIDs[posicion];
}


bool Cliente::setId(int id_) {
    if (id_ <= 0) return false;
    id = id_;
    return true;
}
bool Cliente::setNombre(const char* n) {
    if (!n || strlen(n) == 0) return false;
    strncpy(nombre, n, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    return true;
}
bool Cliente::setDireccion(const char* d) {
    if (!d) return false;
    strncpy(direccion, d, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    return true;
}
bool Cliente::setTelefono(const char* t) {
    if (!t || strlen(t) == 0) return false;
    strncpy(telefono, t, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    return true;
}
bool Cliente::setEmail(const char* e) {
    if (!e || strlen(e) == 0) return false;
    strncpy(email, e, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    return true;
}
bool Cliente::setIdentificacion(const char* ident) {
    if (!ident || strlen(ident) == 0) return false;
    strncpy(identificacion, ident, sizeof(identificacion) - 1);
    identificacion[sizeof(identificacion) - 1] = '\0';
    return true;
}
void Cliente::setTotalGastado(float t)          { if (t >= 0) totalGastado = t; }
void Cliente::setEliminado(bool e)              { eliminado = e; }
void Cliente::setFechaRegistro(time_t f)        { fechaRegistro = f; }
void Cliente::actualizarFechaModificacion()     { fechaUltimaModificacion = time(nullptr); }


bool Cliente::agregarCompraId(int idTransaccion) {
    if (cantidadCompras >= MAX_TRANSACCIONES_POR_ENTIDAD) return false;
    comprasIDs[cantidadCompras++] = idTransaccion;
    return true;
}

void Cliente::registrarCompra(int idTransaccion, float monto) {
    agregarCompraId(idTransaccion);
    totalGastado += monto;
    actualizarFechaModificacion();
}

void Cliente::revertirCompra(float monto) {
    totalGastado -= monto;
    if (totalGastado < 0) totalGastado = 0;
    actualizarFechaModificacion();
}


void Cliente::mostrarInformacionBasica() const {
    printf("ID: %-3d | %-25s | CI: %-12s | Tel: %s\n",
           id, nombre, identificacion, telefono);
}

void Cliente::mostrarInformacionCompleta() const {
    mostrarInformacionBasica();
    cout << "  Email:         " << email     << endl;
    cout << "  Direccion:     " << direccion << endl;
    cout << "  Compras:       " << cantidadCompras << endl;
    printf("  Total gastado: $%.2f\n", totalGastado);
}

int Cliente::obtenerTamano() { return sizeof(Cliente); }
