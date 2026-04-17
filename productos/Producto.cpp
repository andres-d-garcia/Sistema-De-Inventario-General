#include "Producto.hpp"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

Producto::Producto() {
    id = 0;
    memset(codigo,      0, sizeof(codigo));
    memset(nombre,      0, sizeof(nombre));
    memset(descripcion, 0, sizeof(descripcion));
    precio        = 0.0f;
    stock         = 0;
    idProveedor   = 0;
    stockMinimo   = 0;
    totalVendidos = 0;
    eliminado     = false;
    fechaCreacion           = 0;
    fechaUltimaModificacion = 0;
}

Producto::Producto(const char* codigo_, const char* nombre_,
                   const char* descripcion_, int idProveedor_,
                   float precio_, int stock_, int stockMinimo_) : Producto() {
    setCodigo(codigo_);
    setNombre(nombre_);
    setDescripcion(descripcion_);
    setIdProveedor(idProveedor_);
    setPrecio(precio_);
    setStock(stock_);
    setStockMinimo(stockMinimo_);
    fechaCreacion = time(nullptr);
}

Producto::Producto(const Producto& otro) {
    id = otro.id;
    strncpy(codigo,      otro.codigo,      sizeof(codigo)      - 1);
    strncpy(nombre,      otro.nombre,      sizeof(nombre)      - 1);
    strncpy(descripcion, otro.descripcion, sizeof(descripcion) - 1);
    precio        = otro.precio;
    stock         = otro.stock;
    idProveedor   = otro.idProveedor;
    stockMinimo   = otro.stockMinimo;
    totalVendidos = otro.totalVendidos;
    eliminado     = otro.eliminado;
    fechaCreacion           = otro.fechaCreacion;
    fechaUltimaModificacion = otro.fechaUltimaModificacion;
}

Producto::~Producto() {}

// --- Getters ---
int         Producto::getId()           const { return id; }
const char* Producto::getCodigo()       const { return codigo; }
const char* Producto::getNombre()       const { return nombre; }
const char* Producto::getDescripcion()  const { return descripcion; }
float       Producto::getPrecio()       const { return precio; }
int         Producto::getStock()        const { return stock; }
int         Producto::getIdProveedor()  const { return idProveedor; }
int         Producto::getStockMinimo()  const { return stockMinimo; }
int         Producto::getTotalVendidos() const { return totalVendidos; }
bool        Producto::getEliminado()    const { return eliminado; }
time_t      Producto::getFechaCreacion() const { return fechaCreacion; }
time_t      Producto::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// --- Setters ---
bool Producto::setId(int id_) {
    if (id_ <= 0) return false;
    id = id_;
    return true;
}

bool Producto::setCodigo(const char* c) {
    if (!c || strlen(c) == 0) return false;
    strncpy(codigo, c, sizeof(codigo) - 1);
    codigo[sizeof(codigo) - 1] = '\0';
    return true;
}

bool Producto::setNombre(const char* n) {
    if (!n || strlen(n) == 0) return false;
    strncpy(nombre, n, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    return true;
}

bool Producto::setDescripcion(const char* d) {
    if (!d) return false;
    strncpy(descripcion, d, sizeof(descripcion) - 1);
    descripcion[sizeof(descripcion) - 1] = '\0';
    return true;
}

bool Producto::setPrecio(float p) {
    if (p <= 0) { cout << "ERROR: El precio debe ser mayor a 0." << endl; return false; }
    precio = p;
    return true;
}

bool Producto::setStock(int s) {
    if (s < 0) { cout << "ERROR: El stock no puede ser negativo." << endl; return false; }
    stock = s;
    return true;
}

bool Producto::setIdProveedor(int id_) {
    if (id_ <= 0) return false;
    idProveedor = id_;
    return true;
}

bool Producto::setStockMinimo(int sm) {
    if (sm < 0) return false;
    stockMinimo = sm;
    return true;
}

void Producto::setTotalVendidos(int t)      { if (t >= 0) totalVendidos = t; }
void Producto::setEliminado(bool e)         { eliminado = e; }
void Producto::setFechaCreacion(time_t f)   { fechaCreacion = f; }
void Producto::actualizarFechaModificacion(){ fechaUltimaModificacion = time(nullptr); }

// --- Metodos de negocio ---
bool Producto::tieneStockCritico() const {
    return stock <= stockMinimo;
}

bool Producto::tieneStockSuficiente(int cantidad) const {
    return stock >= cantidad;
}

void Producto::vender(int cantidad) {
    if (cantidad > 0 && stock >= cantidad) {
        stock -= cantidad;
        totalVendidos += cantidad;
        actualizarFechaModificacion();
    }
}

void Producto::reabastecer(int cantidad) {
    if (cantidad > 0) {
        stock += cantidad;
        actualizarFechaModificacion();
    }
}

void Producto::revertirVenta(int cantidad) {
    if (cantidad > 0) {
        stock += cantidad;
        totalVendidos -= cantidad;
        if (totalVendidos < 0) totalVendidos = 0;
        actualizarFechaModificacion();
    }
}

// --- Presentacion ---
void Producto::mostrarInformacionBasica() const {
    char stockLabel[20] = "";
    if (tieneStockCritico()) strcpy(stockLabel, " [CRITICO]");
    printf("ID: %-3d | Cod: %-10s | %-25s | $%-8.2f | Stock: %d%s\n",
           id, codigo, nombre, precio, stock, stockLabel);
}

void Producto::mostrarInformacionCompleta() const {
    mostrarInformacionBasica();
    cout << "  Descripcion:  " << descripcion << endl;
    cout << "  Stock minimo: " << stockMinimo
         << " | Vendidos: "    << totalVendidos
         << " | ID Proveedor: " << idProveedor << endl;
}

int Producto::obtenerTamano() { return sizeof(Producto); }
