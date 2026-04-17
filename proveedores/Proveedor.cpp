#include "Proveedor.hpp"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

Proveedor::Proveedor() {
    id = 0;
    memset(nombre,         0, sizeof(nombre));
    memset(direccion,      0, sizeof(direccion));
    memset(telefono,       0, sizeof(telefono));
    memset(email,          0, sizeof(email));
    memset(identificacion, 0, sizeof(identificacion));
    memset(productosIDs,   0, sizeof(productosIDs));
    cantidadProductos       = 0;
    eliminado               = false;
    fechaRegistro           = 0;
    fechaUltimaModificacion = 0;
}

Proveedor::Proveedor(const char* nombre_, const char* identificacion_,
                     const char* telefono_, const char* email_,
                     const char* direccion_) : Proveedor() {
    setNombre(nombre_);
    setIdentificacion(identificacion_);
    setTelefono(telefono_);
    setEmail(email_);
    setDireccion(direccion_);
    fechaRegistro = time(nullptr);
}

Proveedor::Proveedor(const Proveedor& otro) {
    id = otro.id;
    strncpy(nombre,         otro.nombre,         sizeof(nombre)         - 1);
    strncpy(direccion,      otro.direccion,      sizeof(direccion)      - 1);
    strncpy(telefono,       otro.telefono,       sizeof(telefono)       - 1);
    strncpy(email,          otro.email,          sizeof(email)          - 1);
    strncpy(identificacion, otro.identificacion, sizeof(identificacion) - 1);
    memcpy(productosIDs, otro.productosIDs, sizeof(productosIDs));
    cantidadProductos       = otro.cantidadProductos;
    eliminado               = otro.eliminado;
    fechaRegistro           = otro.fechaRegistro;
    fechaUltimaModificacion = otro.fechaUltimaModificacion;
}

Proveedor::~Proveedor() {}

// --- Getters ---
int         Proveedor::getId()             const { return id; }
const char* Proveedor::getNombre()         const { return nombre; }
const char* Proveedor::getDireccion()      const { return direccion; }
const char* Proveedor::getTelefono()       const { return telefono; }
const char* Proveedor::getEmail()          const { return email; }
const char* Proveedor::getIdentificacion() const { return identificacion; }
int         Proveedor::getCantidadProductos() const { return cantidadProductos; }
bool        Proveedor::getEliminado()      const { return eliminado; }
time_t      Proveedor::getFechaRegistro()  const { return fechaRegistro; }
time_t      Proveedor::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

int Proveedor::getProductoId(int posicion) const {
    if (posicion < 0 || posicion >= cantidadProductos) return -1;
    return productosIDs[posicion];
}

// --- Setters ---
bool Proveedor::setId(int id_) {
    if (id_ <= 0) return false;
    id = id_;
    return true;
}
bool Proveedor::setNombre(const char* n) {
    if (!n || strlen(n) == 0) return false;
    strncpy(nombre, n, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    return true;
}
bool Proveedor::setDireccion(const char* d) {
    if (!d) return false;
    strncpy(direccion, d, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    return true;
}
bool Proveedor::setTelefono(const char* t) {
    if (!t || strlen(t) == 0) return false;
    strncpy(telefono, t, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    return true;
}
bool Proveedor::setEmail(const char* e) {
    if (!e || strlen(e) == 0) return false;
    strncpy(email, e, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    return true;
}
bool Proveedor::setIdentificacion(const char* ident) {
    if (!ident || strlen(ident) == 0) return false;
    strncpy(identificacion, ident, sizeof(identificacion) - 1);
    identificacion[sizeof(identificacion) - 1] = '\0';
    return true;
}
void Proveedor::setEliminado(bool e)           { eliminado = e; }
void Proveedor::setFechaRegistro(time_t f)     { fechaRegistro = f; }
void Proveedor::actualizarFechaModificacion()  { fechaUltimaModificacion = time(nullptr); }

// --- Gestion de relaciones ---
bool Proveedor::agregarProductoId(int idProducto) {
    if (cantidadProductos >= MAX_PRODUCTOS_POR_PROVEEDOR) return false;
    // Verificar que no este duplicado
    for (int i = 0; i < cantidadProductos; i++) {
        if (productosIDs[i] == idProducto) return false;
    }
    productosIDs[cantidadProductos++] = idProducto;
    actualizarFechaModificacion();
    return true;
}

bool Proveedor::eliminarProductoId(int idProducto) {
    for (int i = 0; i < cantidadProductos; i++) {
        if (productosIDs[i] == idProducto) {
            // Compactar el arreglo desplazando elementos
            for (int j = i; j < cantidadProductos - 1; j++) {
                productosIDs[j] = productosIDs[j + 1];
            }
            cantidadProductos--;
            actualizarFechaModificacion();
            return true;
        }
    }
    return false;
}

// --- Presentacion ---
void Proveedor::mostrarInformacionBasica() const {
    printf("ID: %-3d | %-25s | RIF: %-12s | Tel: %s\n",
           id, nombre, identificacion, telefono);
}

void Proveedor::mostrarInformacionCompleta() const {
    mostrarInformacionBasica();
    cout << "  Email:     " << email     << endl;
    cout << "  Direccion: " << direccion << endl;
    cout << "  Productos asociados: " << cantidadProductos << endl;
}

int Proveedor::obtenerTamano() { return sizeof(Proveedor); }
