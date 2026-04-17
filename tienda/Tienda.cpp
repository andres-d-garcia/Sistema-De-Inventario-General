#include "Tienda.hpp"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

// =============================================================================
// Constructores y destructor
// =============================================================================

Tienda::Tienda() {
    id = 0;
    memset(nombre,    0, sizeof(nombre));
    memset(rif,       0, sizeof(rif));
    memset(direccion, 0, sizeof(direccion));
    memset(telefono,  0, sizeof(telefono));
    memset(email,     0, sizeof(email));
    totalProductos     = 0;
    totalProveedores   = 0;
    totalClientes      = 0;
    totalTransacciones = 0;
    totalVentas        = 0.0f;
    totalCompras       = 0.0f;
    eliminado          = false;
    fechaCreacion             = 0;
    fechaUltimaModificacion   = 0;
}

Tienda::Tienda(const char* nombre_, const char* rif_, const char* direccion_,
               const char* telefono_, const char* email_) : Tienda() {
    // Delegar inicializacion base al constructor por defecto
    setNombre(nombre_);
    setRif(rif_);
    setDireccion(direccion_);
    setTelefono(telefono_);
    setEmail(email_);
    fechaCreacion = time(nullptr);
}

Tienda::Tienda(const Tienda& otro) {
    id = otro.id;
    strncpy(nombre,    otro.nombre,    sizeof(nombre)    - 1);
    strncpy(rif,       otro.rif,       sizeof(rif)       - 1);
    strncpy(direccion, otro.direccion, sizeof(direccion) - 1);
    strncpy(telefono,  otro.telefono,  sizeof(telefono)  - 1);
    strncpy(email,     otro.email,     sizeof(email)     - 1);
    totalProductos     = otro.totalProductos;
    totalProveedores   = otro.totalProveedores;
    totalClientes      = otro.totalClientes;
    totalTransacciones = otro.totalTransacciones;
    totalVentas        = otro.totalVentas;
    totalCompras       = otro.totalCompras;
    eliminado          = otro.eliminado;
    fechaCreacion             = otro.fechaCreacion;
    fechaUltimaModificacion   = otro.fechaUltimaModificacion;
}

Tienda::~Tienda() {
    // No hay recursos dinamicos que liberar
}

// =============================================================================
// Getters
// =============================================================================

int         Tienda::getId()           const { return id; }
const char* Tienda::getNombre()       const { return nombre; }
const char* Tienda::getRif()          const { return rif; }
const char* Tienda::getDireccion()    const { return direccion; }
const char* Tienda::getTelefono()     const { return telefono; }
const char* Tienda::getEmail()        const { return email; }
int         Tienda::getTotalProductos()     const { return totalProductos; }
int         Tienda::getTotalProveedores()   const { return totalProveedores; }
int         Tienda::getTotalClientes()      const { return totalClientes; }
int         Tienda::getTotalTransacciones() const { return totalTransacciones; }
float       Tienda::getTotalVentas()   const { return totalVentas; }
float       Tienda::getTotalCompras()  const { return totalCompras; }
bool        Tienda::getEliminado()    const { return eliminado; }
time_t      Tienda::getFechaCreacion() const { return fechaCreacion; }
time_t      Tienda::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// =============================================================================
// Setters con validacion
// =============================================================================

bool Tienda::setId(int id_) {
    if (id_ <= 0) return false;
    id = id_;
    return true;
}

bool Tienda::setNombre(const char* nombre_) {
    if (!nombre_ || strlen(nombre_) == 0) return false;
    strncpy(nombre, nombre_, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    return true;
}

bool Tienda::setRif(const char* rif_) {
    if (!rif_ || strlen(rif_) == 0) return false;
    strncpy(rif, rif_, sizeof(rif) - 1);
    rif[sizeof(rif) - 1] = '\0';
    return true;
}

bool Tienda::setDireccion(const char* direccion_) {
    if (!direccion_) return false;
    strncpy(direccion, direccion_, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    return true;
}

bool Tienda::setTelefono(const char* telefono_) {
    if (!telefono_ || strlen(telefono_) == 0) return false;
    strncpy(telefono, telefono_, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    return true;
}

bool Tienda::setEmail(const char* email_) {
    if (!email_ || strlen(email_) == 0) return false;
    strncpy(email, email_, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    return true;
}

void Tienda::setTotalProductos(int total)     { if (total >= 0) totalProductos = total; }
void Tienda::setTotalProveedores(int total)   { if (total >= 0) totalProveedores = total; }
void Tienda::setTotalClientes(int total)      { if (total >= 0) totalClientes = total; }
void Tienda::setTotalTransacciones(int total) { if (total >= 0) totalTransacciones = total; }
void Tienda::setTotalVentas(float total)      { if (total >= 0) totalVentas = total; }
void Tienda::setTotalCompras(float total)     { if (total >= 0) totalCompras = total; }
void Tienda::setEliminado(bool e)             { eliminado = e; }
void Tienda::setFechaCreacion(time_t fecha)   { fechaCreacion = fecha; }

void Tienda::actualizarFechaModificacion() {
    fechaUltimaModificacion = time(nullptr);
}

// =============================================================================
// Metodos de negocio
// =============================================================================

void Tienda::incrementarProductos()   { totalProductos++;   actualizarFechaModificacion(); }
void Tienda::decrementarProductos()   { if (totalProductos > 0) totalProductos--;  actualizarFechaModificacion(); }
void Tienda::incrementarProveedores() { totalProveedores++; actualizarFechaModificacion(); }
void Tienda::decrementarProveedores() { if (totalProveedores > 0) totalProveedores--; actualizarFechaModificacion(); }
void Tienda::incrementarClientes()    { totalClientes++;    actualizarFechaModificacion(); }
void Tienda::decrementarClientes()    { if (totalClientes > 0) totalClientes--;  actualizarFechaModificacion(); }

void Tienda::agregarVenta(float monto) {
    if (monto > 0) {
        totalVentas += monto;
        totalTransacciones++;
        actualizarFechaModificacion();
    }
}

void Tienda::revertirVenta(float monto) {
    totalVentas -= monto;
    if (totalVentas < 0) totalVentas = 0;
    actualizarFechaModificacion();
}

void Tienda::agregarCompra(float monto) {
    if (monto > 0) {
        totalCompras += monto;
        totalTransacciones++;
        actualizarFechaModificacion();
    }
}

void Tienda::revertirCompra(float monto) {
    totalCompras -= monto;
    if (totalCompras < 0) totalCompras = 0;
    actualizarFechaModificacion();
}

// =============================================================================
// Metodos de presentacion
// =============================================================================

void Tienda::mostrarInformacionBasica() const {
    printf("%-20s | RIF: %-15s | Tel: %s\n", nombre, rif, telefono);
}

void Tienda::mostrarInformacionCompleta() const {
    cout << "========================================" << endl;
    cout << "  INFORMACION DE LA TIENDA" << endl;
    cout << "========================================" << endl;
    cout << "Nombre:    " << nombre    << endl;
    cout << "RIF:       " << rif       << endl;
    cout << "Email:     " << email     << endl;
    cout << "Telefono:  " << telefono  << endl;
    cout << "Direccion: " << direccion << endl;
    cout << "----------------------------------------" << endl;
    cout << "ESTADISTICAS:" << endl;
    cout << "  Productos:     " << totalProductos    << endl;
    cout << "  Proveedores:   " << totalProveedores  << endl;
    cout << "  Clientes:      " << totalClientes     << endl;
    cout << "  Transacciones: " << totalTransacciones << endl;
    printf("  Ventas totales:  $%.2f\n", totalVentas);
    printf("  Compras totales: $%.2f\n", totalCompras);
    printf("  Balance:         $%.2f\n", totalVentas - totalCompras);
    cout << "========================================" << endl;
}

// =============================================================================
// Metodo estatico
// =============================================================================

int Tienda::obtenerTamano() {
    return sizeof(Tienda);
}
