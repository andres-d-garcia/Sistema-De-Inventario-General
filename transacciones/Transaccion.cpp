#include "Transaccion.hpp"
#include <cstring>
#include <iostream>

ItemTransaccion::ItemTransaccion() : idProducto(0), cantidad(0), precioUnitario(0.0f), subtotal(0.0f) {}

ItemTransaccion::ItemTransaccion(int idProducto_, int cantidad_, float precioUnitario_)
    : idProducto(idProducto_), cantidad(cantidad_), precioUnitario(precioUnitario_) {
    subtotal = precioUnitario * cantidad;
}

int ItemTransaccion::getIdProducto() const { return idProducto; }
int ItemTransaccion::getCantidad() const { return cantidad; }
float ItemTransaccion::getPrecioUnitario() const { return precioUnitario; }
float ItemTransaccion::getSubtotal() const { return subtotal; }

bool ItemTransaccion::setIdProducto(int id) { idProducto = id; return true; }
bool ItemTransaccion::setCantidad(int cantidad_) { cantidad = cantidad_; subtotal = precioUnitario * cantidad; return true; }
bool ItemTransaccion::setPrecioUnitario(float precio) { precioUnitario = precio; subtotal = precioUnitario * cantidad; return true; }

void ItemTransaccion::mostrar() const { std::cout << "Item: " << idProducto << " x" << cantidad << " = " << subtotal << "\n"; }

Transaccion::Transaccion() {
    id = 0;
    tipo[0] = '\0';
    idCliente = idProveedor = 0;
    cantidadItems = 0;
    total = 0.0f;
    descripcion[0] = '\0';
    eliminado = false;
    fecha[0] = '\0';
    fechaRegistro = fechaUltimaModificacion = 0;
}

Transaccion::Transaccion(const char* tipo_, int idCliente_, int idProveedor_) : Transaccion() {
    setTipo(tipo_);
    idCliente = idCliente_;
    idProveedor = idProveedor_;
}

Transaccion::Transaccion(const Transaccion& otro) { *this = otro; }
Transaccion::~Transaccion() {}

int Transaccion::getId() const { return id; }
const char* Transaccion::getTipo() const { return tipo; }
int Transaccion::getIdCliente() const { return idCliente; }
int Transaccion::getIdProveedor() const { return idProveedor; }
int Transaccion::getCantidadItems() const { return cantidadItems; }
float Transaccion::getTotal() const { return total; }
const char* Transaccion::getFecha() const { return fecha; }
const char* Transaccion::getDescripcion() const { return descripcion; }
bool Transaccion::getEliminado() const { return eliminado; }
time_t Transaccion::getFechaRegistro() const { return fechaRegistro; }
time_t Transaccion::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }
const ItemTransaccion& Transaccion::getItem(int posicion) const { return items[posicion]; }

bool Transaccion::setId(int id_) { id = id_; return true; }
bool Transaccion::setTipo(const char* tipo_) { std::strncpy(tipo, tipo_, sizeof(tipo)); tipo[sizeof(tipo)-1] = '\0'; return true; }
bool Transaccion::setIdCliente(int id_) { idCliente = id_; return true; }
bool Transaccion::setIdProveedor(int id_) { idProveedor = id_; return true; }
bool Transaccion::setDescripcion(const char* desc) { std::strncpy(descripcion, desc, sizeof(descripcion)); descripcion[sizeof(descripcion)-1] = '\0'; return true; }
void Transaccion::setFecha(const char* fecha_) { std::strncpy(fecha, fecha_, sizeof(fecha)); fecha[sizeof(fecha)-1] = '\0'; }
void Transaccion::setEliminado(bool eliminado_) { eliminado = eliminado_; }
void Transaccion::setFechaRegistro(time_t fecha_) { fechaRegistro = fecha_; }
void Transaccion::actualizarFechaModificacion() { fechaUltimaModificacion = std::time(nullptr); }

bool Transaccion::agregarItem(const ItemTransaccion& item) {
    if (cantidadItems >= MAX_ITEMS_POR_TRANSACCION) return false;
    items[cantidadItems++] = item;
    recalcularTotal();
    return true;
}

void Transaccion::recalcularTotal() {
    total = 0.0f;
    for (int i = 0; i < cantidadItems; ++i) total += items[i].getSubtotal();
}

bool Transaccion::esVenta() const { return std::strcmp(tipo, "VENTA") == 0; }
bool Transaccion::esCompra() const { return std::strcmp(tipo, "COMPRA") == 0; }

void Transaccion::mostrarInformacionBasica() const { std::cout << "Transaccion " << id << " total=" << total << "\n"; }
void Transaccion::mostrarInformacionCompleta() const { mostrarInformacionBasica(); for (int i=0;i<cantidadItems;++i) items[i].mostrar(); }

int Transaccion::obtenerTamano() { return sizeof(Transaccion); }