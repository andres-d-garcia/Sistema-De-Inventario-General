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
    ok &= inicializarArchivo(ARCHIVO_PRODUCTO);
    ok &= inicializarArchivo(ARCHIVO_PROVEEDOR);
    ok &= inicializarArchivo(ARCHIVO_CLIENTE);
    ok &= inicializarArchivo(ARCHIVO_TRANSACCION);

    if (ok) {
        cout << "Sistema de archivos inicializado correctamente." << endl;
    }
    return ok;
}

template<typename T>
int escribirRegistroAlFinal(const char* nombreArchivo, T registro) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return -1;

    ArchivoHeader header = leerHeader(nombreArchivo);
    int idAsignado = header.proximoId;
    registro.id = idAsignado;

    archivo.seekp(calcularOffset<T>(header.cantidadRegistros), ios::beg);
    archivo.write(reinterpret_cast<char*>(&registro), sizeof(T));
    archivo.close();

    header.cantidadRegistros++;
    header.proximoId++;
    header.registrosActivos++;
    actualizarHeader(nombreArchivo, header);

    return idAsignado;
}

template<typename T>
bool leerRegistroPorIndice(const char* nombreArchivo, int id, T& registro) {
    fstream archivo(nombreArchivo, ios::in | ios::binary);
    if (!archivo.is_open()) return false;

    long offset = calcularOffset<T>(indiceFisico);
    archivo.seekg(offset, ios::beg);
    archivo.read(reinterpret_cast<char*>(&destino), sizeof(T));

    bool ok = !archivo.fail();
    archivo.close();
    return ok;
}

template<typename T>
bool escribirRegistroPorIndice(const char* nombreArchivo, int indiceFisico, T& registro) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return false;

    long offset = calcularOffset<T>(indiceFisico);
    archivo.seekp(offset, ios::beg);
    archivo.write(reinterpret_cast<char*>(&registro), sizeof(T));
    archivo.close();
    return true;
}

template<typename T>
int buscarIndiceFisicoPorId(const char* nombreArchivo, int idBuscado) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    if (header.cantidadRegistros == 0) return -1;

    fstream archivo(nombreArchivo, ios::in | ios::binary);
    if (!archivo.is_open()) return -1;

    T registro;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long offset = calcularOffset<T>(i);
        archivo.seekg(offset, ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

        if (!archivo.fail() && registro.id == idBuscado && !registro.eliminado) {
            archivo.close();
            return i;
        }
    }

    archivo.close();
    return -1;
}

template<typename T>
bool borradoLogico(const char* nombreArchivo, int indiceFisico) {
    T registro;
    if (!leerRegistroPorIndice<T>(nombreArchivo, indiceFisico, registro)) return false;

    registro.eliminado = true;
    registro.fechaUltimaModificacion = time(nullptr);

    if (!escribirRegistroPorIndice<T>(nombreArchivo, indiceFisico, registro)) return false;

    ArchivoHeader header = leerHeader(nombreArchivo);
    if (header.registrosActivos > 0) header.registrosActivos--;
    actualizarHeader(nombreArchivo, header);

    return true;
}

// ---- Validaciones de entrada ----   

bool validarChar(const char* mensaje, char* destino, int tamanio) {
    cout << mensaje;
    cin.getline(destino, tamanio);
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ERROR: Entrada demasiado larga." << endl;
        return false;
    }
    if (strlen(destino) == 0) {
        cout << "ERROR: El campo no puede estar vacio." << endl;
        return false;
    }
    if (strcmp(destino, "cancelar") == 0) {
        cout << "Operacion cancelada." << endl;
        return false;
    }
    return true;
}

bool validarInt(const char* mensaje, int& destino, bool permitirNegativos = false) {
    cout << mensaje;
    string input;
    cin >> input;
    limpiarBuffer();
    if (input == "cancelar") { cout << "Operacion cancelada." << endl; return false; }
    try {
        destino = stoi(input);
        if (!permitirNegativos && destino < 0) {
            cout << "ERROR: El valor no puede ser negativo." << endl;
            return false;
        }
        return true;
    } catch (...) {
        cout << "ERROR: Ingrese un numero valido." << endl;
        return false;
    }
}

bool validarFloat(const char* mensaje, float& destino) {
    cout << mensaje;
    string input;
    cin >> input;
    limpiarBuffer();
    if (input == "cancelar") { cout << "Operacion cancelada." << endl; return false; }
    try {
        destino = stof(input);
        if (destino < 0) { cout << "ERROR: El valor no puede ser negativo." << endl; return false; }
        return true;
    } catch (...) {
        cout << "ERROR: Ingrese un numero valido." << endl;
        return false;
    }
}

bool validarEmail(const char* email) {
    int len = strlen(email);
    if (len < 5 || len > 100) {
        cout << "ERROR: Email debe tener entre 5 y 100 caracteres." << endl;
        return false;
    }
    const char* arroba = strchr(email, '@');
    if (!arroba || arroba == email) {
        cout << "ERROR: Email debe contener '@' y no puede estar al inicio." << endl;
        return false;
    }
    const char* punto = strchr(arroba + 1, '.');
    if (!punto || punto == arroba + 1) {
        cout << "ERROR: Email debe contener '.' despues del '@'." << endl;
        return false;
    }
    if (email[len - 1] == '.') {
        cout << "ERROR: Email no puede terminar con punto." << endl;
        return false;
    }
    if (strchr(email, ' ') != nullptr) {
        cout << "ERROR: Email no puede contener espacios." << endl;
        return false;
    }
    return true;
}

bool validarRIF(const char* rif) {
    int len = strlen(rif);
    if (len != 13) {
        cout << "ERROR: RIF debe tener formato: T-12345678-V (13 caracteres)." << endl;
        return false;
    }
    char tipo = toupper(rif[0]);
    const char* tiposValidos = "JGVEPCD";
    if (!strchr(tiposValidos, tipo)) {
        cout << "ERROR: Tipo de RIF invalido. Debe ser J, G, V, E, P, C o D." << endl;
        return false;
    }
    if (rif[1] != '-' || rif[10] != '-') {
        cout << "ERROR: Formato RIF invalido. Los guiones deben estar en las posiciones 1 y 10." << endl;
        return false;
    }
    for (int i = 2; i <= 9; i++) {
        if (!isdigit(rif[i])) {
            cout << "ERROR: Posiciones 2-9 del RIF deben ser digitos." << endl;
            return false;
        }
    }
    char verificador = toupper(rif[12]);
    if (!isdigit(verificador) && verificador != 'K') {
        cout << "ERROR: Digito verificador debe ser un numero (0-9) o la letra K." << endl;
        return false;
    }
    return true;
}

bool validarCedula(const char* cedula) {
    int len = strlen(cedula);
    if (len < 10 || len > 13) {
        cout << "ERROR: Cedula debe tener entre 10 y 13 caracteres (T-12345678)." << endl;
        return false;
    }
    char tipo_id = toupper(cedula[0]);
    if (tipo_id != 'V' && tipo_id != 'E') {
        cout << "ERROR: Cedula debe comenzar con V (Venezolano) o E (Extranjero)." << endl;
        return false;
    }
    if (cedula[1] != '-') {
        cout << "ERROR: Formato invalido. Ejemplo: V-12345678" << endl;
        return false;
    }
    for (int i = 2; i < len; i++) {
        if (!isdigit(cedula[i])) {
            cout << "ERROR: Cedula debe contener solo digitos despues del tipo." << endl;
            return false;
        }
    }
    return true;
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10 || fecha[4] != '-' || fecha[7] != '-') {
        cout << "ERROR: Formato de fecha debe ser YYYY-MM-DD." << endl;
        return false;
    }
    int mes = atoi(string(fecha + 5, 2).c_str());
    int dia = atoi(string(fecha + 8, 2).c_str());
    if (mes < 1 || mes > 12 || dia < 1 || dia > 31) {
        cout << "ERROR: Fecha fuera de rango." << endl;
        return false;
    }
    return true;
}

// ---- Validacion de Unicidad ----

bool codigoProductoExiste(const char* codigo, int idExcluir = -1) {
    ArchivoHeader h = leerHeader(ARCHIVO_PRODUCTO);
    Producto p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, i, p) && !p.eliminado && p.id != idExcluir) {
            char a[20], b[20];
            convertirMinusculas(a, p.codigo);
            convertirMinusculas(b, codigo);
            if (strcmp(a, b) == 0) return true;
        }
    }
    return false;
}

bool identificacionProveedorExiste(const char* id, int idExcluir = -1) {
    ArchivoHeader h = leerHeader(ARCHIVO_PROVEEDOR);
    Proveedor p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, i, p) && !p.eliminado && p.id != idExcluir) {
            char a[20], b[20];
            convertirMinusculas(a, p.identificacion);
            convertirMinusculas(b, id);
            if (strcmp(a, b) == 0) return true;
        }
    }
    return false;
}

bool identificacionClienteExiste(const char* id, int idExcluir = -1) {
    ArchivoHeader h = leerHeader(ARCHIVO_CLIENTE);
    Cliente c;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, i, c) && !c.eliminado && c.id != idExcluir) {
            char a[20], b[20];
            convertirMinusculas(a, c.identificacion);
            convertirMinusculas(b, id);
            if (strcmp(a, b) == 0) return true;
        }
    }
    return false;
}

// ----Funciones de Tienda----

bool tiendaExiste() {
    ArchivoHeader h = leerHeader(ARCHIVO_TIENDA);
    return h.cantidadRegistros > 0;
}

bool leerTienda(Tienda& t) {
    return leerRegistroPorIndice<Tienda>(ARCHIVO_TIENDA, 0, t);
}

bool guardarTienda(Tienda& t) {
    t.fechaUltimaModificacion = time(nullptr);
    if (!tiendaExiste()) {
        t.fechaCreacion = time(nullptr);
        t.eliminado = false;
        t.totalProductos = 0;
        t.totalProveedores = 0;
        t.totalClientes = 0;
        t.totalTransacciones = 0;
        t.totalVentas = 0;
        t.totalCompras = 0;
        return escribirRegistroAlFinal<Tienda>(ARCHIVO_TIENDA, t) >= 0;
    } else {
        return escribirRegistroPorIndice<Tienda>(ARCHIVO_TIENDA, 0, t);
    }
}

void mostrarTienda(const Tienda& t) {
    imprimirLinea();
    cout << "INFORMACION DE LA TIENDA" << endl;
    imprimirLinea();
    cout << "Nombre:  " << t.nombre << endl;
    cout << "RIF:     " << t.rif << endl;
    cout << "Email:   " << t.email << endl;
    cout << "Telefono:" << t.telefono << endl;
    cout << "Direccion:" << t.direccion << endl;
    imprimirLinea();
    cout << "Estadisticas:" << endl;
    cout << "  Productos:     " << t.totalProductos << endl;
    cout << "  Proveedores:   " << t.totalProveedores << endl;
    cout << "  Clientes:      " << t.totalClientes << endl;
    cout << "  Transacciones: " << t.totalTransacciones << endl;
    printf("  Ventas totales:  $%.2f\n", t.totalVentas);
    printf("  Compras totales: $%.2f\n", t.totalCompras);
    printf("  Balance:         $%.2f\n", t.totalVentas - t.totalCompras);
    imprimirLinea();
}

void configurarTienda() {
    limpiarPantalla();
    Tienda t;
    bool existe = tiendaExiste();

    if (existe) {
        leerTienda(t);
        cout << "Tienda actual:" << endl;
        mostrarTienda(t);
        cout << "Desea modificar los datos de la tienda? (s/n): ";
        char resp;
        cin >> resp;
        limpiarBuffer();
        if (resp != 's' && resp != 'S') return;
    } else {
        memset(&t, 0, sizeof(Tienda));
        cout << "=== CONFIGURACION INICIAL DE LA TIENDA ===" << endl;
    }

    if (!validarChar("Nombre de la tienda: ", t.nombre, 100)) return;
    if (!validarChar("RIF de la tienda (J-XXXXXXXX-X): ", t.rif, 20)) return;
    if (!validarChar("Direccion: ", t.direccion, 200)) return;
    if (!validarChar("Telefono: ", t.telefono, 20)) return;
    if (!validarChar("Email: ", t.email, 100)) return;
    if (!validarEmail(t.email)) return;

    if (guardarTienda(t)) {
        cout << "Tienda configurada exitosamente." << endl;
    }
}