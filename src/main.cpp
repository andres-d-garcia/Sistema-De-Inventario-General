#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cctype>
#include <cstdio>
#include <limits>
#include <iomanip>
#include <sys/stat.h>
#include <direct.h>
using namespace std;

const char* ARCHIVO_TIENDA = "tienda.bin";
const char* ARCHIVO_PRODUCTO = "productos.bin";
const char* ARCHIVO_PROVEEDOR = "proveedores.bin";
const char* ARCHIVO_CLIENTE = "clientes.bin";
const char* ARCHIVO_TRANSACCION = "transacciones.bin";

const int MAX_PRODUCTOS_POR_PROVEEDOR = 100;
const int MAX_TRANSACCIONES_POR_ENTIDAD = 200;
const int MAX_ITEMS_POR_TRANSACCION = 50;
struct ArchivoHeader {
    int cantidadRegistros;
    int proximoId;
    int registrosActivos;   
    int version;
};

struct Tienda {
    char nombre[100];
    char rif[20];
    char direccion[200];
    char telefono[20];
    char email[100];
    int  totalProductos;
    int  totalProveedores;
    int  totalClientes;
    int  totalTransacciones;
    float totalVentas;
    float totalCompras;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Proveedor {
    int  id;
    char nombre[100];
    char direccion[200];
    char telefono[20];
    char email[100];
    char identificacion[20];   // RIF formato J-12345678-9
    int  productosIDs[MAX_PRODUCTOS_POR_PROVEEDOR];
    int  cantidadProductos;
    bool eliminado;
    time_t fechaRegistro;
    time_t fechaUltimaModificacion;
};

struct Producto {
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    int idProveedor;
    float precio;
    int stock;
    int stockMinimo;
    int totalVendidos;
    bool eliminado;
    time_t fechaUltimaVenta;
    time_t fechaRegistro;
    char fechaRegistro[20];
};
struct Producto {
    int   id;
    char  codigo[20];
    char  nombre[100];
    char  descripcion[200];
    float precio;
    int   stock;
    int   idProveedor;
    int   stockMinimo;
    int   totalVendidos;
    bool  eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Cliente {
    int  id;
    char nombre[100];
    char direccion[200];
    char telefono[20];
    char email[100];
    char identificacion[20];   // Cedula formato V-12345678
    int  comprasIDs[MAX_TRANSACCIONES_POR_ENTIDAD];
    int  cantidadCompras;
    float totalGastado;
    bool eliminado;
    time_t fechaRegistro;
    time_t fechaUltimaModificacion;
};

struct ItemTransaccion {
    int idProducto;
    int cantidad;
    float precioUnitario;
    float subtotal;
};
struct Transaccion {
    int  id;
    char tipo[10];              // "COMPRA" o "VENTA"
    int  idCliente;             // Llave foranea (0 si es COMPRA)
    int  idProveedor;           // Llave foranea (0 si es VENTA)
    ItemTransaccion items[MAX_ITEMS_POR_TRANSACCION];
    int  cantidadItems;
    float total;
    char fecha[11];
    char descripcion[200];
    bool eliminado;
    time_t fechaRegistro;
    time_t fechaUltimaModificacion;
};

// --- Utilidades ---


void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void obtenerFechaActual(char* buffer, int tamanio) {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    strftime(buffer, tamanio, "%Y-%m-%d", lt);
}

void convertirMinusculas(char* dest, const char* src) {
    int i = 0;
    while (src[i]) { dest[i] = tolower(src[i]); i++; }
    dest[i] = '\0';
}

bool contieneSubstring(const char* str, const char* sub) {
    char a[300], b[300];
    convertirMinusculas(a, str);
    convertirMinusculas(b, sub);
    return strstr(a, b) != nullptr;
}

void imprimirLinea(int ancho = 70, char c = '-') {
    for (int i = 0; i < ancho; i++) cout << c;
    cout << endl;
}
// --- Funciones de manejo de archivos, headers y offsets --- 

template<typename T>
long calcularOffset(int indiceFisico) {
    return (long)sizeof(ArchivoHeader) + (long)(indiceFisico * sizeof(T));
}

bool inicializarArchivo(const char* nombreArchivo) {
    fstream prueba(nombreArchivo, ios::in | ios::binary);
    if (prueba.is_open()) {
        prueba.close();
        return true;
    }

    fstream archivo(nombreArchivo, ios::out | ios::binary);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo crear el archivo " << nombreArchivo << endl;
        return false;
    }

    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoId         = 1;
    header.registrosActivos  = 0;
    header.version           = 1;

    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header = {0, 1, 0, 1};

    fstream archivo(nombreArchivo, ios::in | ios::binary);
    if (!archivo.is_open()) return header;

    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return header;
}

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return false;

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

bool inicializarSistema() {
    bool ok = true;
    ok &= inicializarArchivo(ARCHIVO_TIENDA);
    ok &= inicializarArchivo(ARCHIVO_PRODUCTOS);
    ok &= inicializarArchivo(ARCHIVO_PROVEEDORES);
    ok &= inicializarArchivo(ARCHIVO_CLIENTES);
    ok &= inicializarArchivo(ARCHIVO_TRANSACCIONES);

    if (ok) {
        cout << "Sistema de archivos inicializado correctamente." << endl;
    }
    return ok;
}