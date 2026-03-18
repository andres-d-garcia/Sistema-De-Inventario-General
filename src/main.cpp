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
    char identificacion[20];
    int  productosIDs[MAX_PRODUCTOS_POR_PROVEEDOR];
    int  cantidadProductos;
    bool eliminado;
    time_t fechaRegistro;
    time_t fechaUltimaModificacion;
};
 
// CORREGIDO: Struct Producto - Eliminada la duplicada y fijada la estructura
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
    char identificacion[20];
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
    char tipo[10];
    int  idCliente;
    int  idProveedor;
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
 
// CORREGIDO: Parámetros faltantes y nombre de variable incorrecto
template<typename T>
bool leerRegistroPorIndice(const char* nombreArchivo, int indiceFisico, T& registro) {
    fstream archivo(nombreArchivo, ios::in | ios::binary);
    if (!archivo.is_open()) return false;
 
    long offset = calcularOffset<T>(indiceFisico);
    archivo.seekg(offset, ios::beg);
    archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));
 
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
 
// ---- Funciones de Producto ----
 
void mostrarInfoProveedor(int idProveedor) {
    int idx = buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, idProveedor);
    if (idx == -1) {
        cout << "  Proveedor: [ID:" << idProveedor << " - No encontrado]" << endl;
        return;
    }
    Proveedor prov;
    leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idx, prov);
    cout << "  Proveedor: " << prov.nombre << " | RIF: " << prov.identificacion
         << " | Tel: " << prov.telefono << endl;
}
 
void mostrarProducto(const Producto& p, bool detallado = false) {
    char stockLabel[20] = "";
    if (p.stock <= p.stockMinimo) strcpy(stockLabel, " [CRITICO]");
 
    printf("ID: %-3d | Cod: %-10s | %-25s | $%-8.2f | Stock: %d%s\n",
           p.id, p.codigo, p.nombre, p.precio, p.stock, stockLabel);
 
    if (detallado) {
        cout << "  Descripcion: " << p.descripcion << endl;
        cout << "  Stock minimo: " << p.stockMinimo << " | Vendidos: " << p.totalVendidos << endl;
        mostrarInfoProveedor(p.idProveedor);
    }
}
 
void listarProductos() {
    ArchivoHeader h = leerHeader(ARCHIVO_PRODUCTO);
    if (h.registrosActivos == 0) {
        cout << "No hay productos registrados." << endl;
        return;
    }
    imprimirLinea(70, '=');
    cout << "LISTADO DE PRODUCTOS (" << h.registrosActivos << " activos)" << endl;
    imprimirLinea();
    cout << "ID  | Codigo     | Nombre                    | Precio   | Stock" << endl;
    imprimirLinea();
 
    Producto p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, i, p) && !p.eliminado) {
            mostrarProducto(p);
        }
    }
    imprimirLinea();
}
 
// ---- CRUD DE Productos ----
 
void listarProveedores();
 
void crearProducto() {
    limpiarPantalla();
    ArchivoHeader hProv = leerHeader(ARCHIVO_PROVEEDOR);
    if (hProv.registrosActivos == 0) {
        cout << "ERROR: Debe registrar al menos un proveedor primero." << endl;
        return;
    }
 
    cout << "=== REGISTRAR NUEVO PRODUCTO ===" << endl;
 
    Producto p;
    memset(&p, 0, sizeof(Producto));
 
    if (!validarChar("Codigo (o 'cancelar'): ", p.codigo, 20)) return;
    if (codigoProductoExiste(p.codigo)) {
        cout << "ERROR: Ya existe un producto con ese codigo." << endl;
        return;
    }
    if (!validarChar("Nombre (o 'cancelar'): ", p.nombre, 100)) return;
    if (!validarChar("Descripcion (o 'cancelar'): ", p.descripcion, 200)) return;
 
    listarProveedores();
    do {
        if (!validarInt("ID del proveedor (o 'cancelar'): ", p.idProveedor)) return;
        if (buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, p.idProveedor) != -1) break;
        cout << "ERROR: Proveedor con ID " << p.idProveedor << " no existe. Intente de nuevo." << endl;
    } while(true);
 
    if (!validarFloat("Precio: ", p.precio)) return;
    if (p.precio <= 0) { cout << "ERROR: El precio debe ser mayor a 0." << endl; return; }
    if (!validarInt("Stock inicial: ", p.stock)) return;
    if (!validarInt("Stock minimo (para alertas): ", p.stockMinimo)) return;
 
    ArchivoHeader h = leerHeader(ARCHIVO_PRODUCTO);
    p.id = h.proximoId;
    p.eliminado = false;
    p.totalVendidos = 0;
    p.fechaCreacion = time(nullptr);
    p.fechaUltimaModificacion = time(nullptr);
 
    imprimirLinea();
    cout << "RESUMEN:" << endl;
    mostrarProducto(p, true);
    imprimirLinea();
    cout << "Confirmar? (s/n): ";
    char conf[5]; cin.getline(conf, 5);
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }
 
    int idx = escribirRegistroAlFinal<Producto>(ARCHIVO_PRODUCTO, p);
    if (idx >= 0) {
        int idxProv = buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, p.idProveedor);
        if (idxProv != -1) {
            Proveedor prov;
            leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idxProv, prov);
            if (prov.cantidadProductos < MAX_PRODUCTOS_POR_PROVEEDOR) {
                prov.productosIDs[prov.cantidadProductos++] = p.id;
                prov.fechaUltimaModificacion = time(nullptr);
                escribirRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idxProv, prov);
            }
        }
 
        Tienda t;
        if (leerTienda(t)) {
            t.totalProductos++;
            t.fechaUltimaModificacion = time(nullptr);
            guardarTienda(t);
        }
 
        cout << "Producto creado con ID: " << p.id << endl;
    }
}
 
void buscarProductoPorId(int id) {
    int idx = buscarIndiceFisicoPorId<Producto>(ARCHIVO_PRODUCTO, id);
    if (idx == -1) { cout << "Producto con ID " << id << " no encontrado." << endl; return; }
    Producto p;
    leerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, idx, p);
    mostrarProducto(p, true);
}
 
void buscarProductoPorNombre(const char* nombre) {
    ArchivoHeader h = leerHeader(ARCHIVO_PRODUCTO);
    int encontrados = 0;
    Producto p;
    imprimirLinea();
    cout << "Resultados para '" << nombre << "':" << endl;
    imprimirLinea();
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, i, p) && !p.eliminado) {
            if (contieneSubstring(p.nombre, nombre)) {
                mostrarProducto(p);
                encontrados++;
            }
        }
    }
    imprimirLinea();
    if (encontrados == 0) cout << "Sin resultados." << endl;
    else cout << "Total: " << encontrados << endl;
    imprimirLinea();
}
 
void actualizarProducto() {
    limpiarPantalla();
    int id;
    if (!validarInt("ID del producto a actualizar (o 'cancelar'): ", id)) return;
 
    int idx = buscarIndiceFisicoPorId<Producto>(ARCHIVO_PRODUCTO, id);
    if (idx == -1) { cout << "Producto no encontrado." << endl; return; }
 
    Producto p;
    leerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, idx, p);
    cout << "Producto actual:" << endl;
    mostrarProducto(p, true);
 
    int op;
    do {
        cout << "\n=== ACTUALIZAR PRODUCTO ===" << endl;
        cout << "1. Codigo    2. Nombre    3. Descripcion" << endl;
        cout << "4. Proveedor 5. Precio    6. Stock" << endl;
        cout << "7. Stock Minimo 8. Ajustar Stock 9. Guardar" << endl;
        cout << "0. Cancelar" << endl;
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();
 
        char temp[200];
        int tempInt;
        float tempFloat;
 
        switch(op) {
            case 1:
                if (validarChar("Nuevo codigo: ", temp, 20)) {
                    if (codigoProductoExiste(temp, p.id)) cout << "ERROR: Codigo ya existe.\n";
                    else strcpy(p.codigo, temp);
                }
                break;
            case 2:
                if (validarChar("Nuevo nombre: ", temp, 100)) strcpy(p.nombre, temp);
                break;
            case 3:
                if (validarChar("Nueva descripcion: ", temp, 200)) strcpy(p.descripcion, temp);
                break;
            case 4: {
                listarProveedores();
                if (validarInt("Nuevo ID proveedor: ", tempInt)) {
                    if (buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, tempInt) != -1)
                        p.idProveedor = tempInt;
                    else cout << "ERROR: Proveedor no existe.\n";
                }
                break;
            }
            case 5:
                if (validarFloat("Nuevo precio: ", tempFloat) && tempFloat > 0) p.precio = tempFloat;
                break;
            case 6:
                cout << "Stock actual: " << p.stock << endl;
                if (validarInt("Nuevo stock: ", tempInt)) p.stock = tempInt;
                break;
            case 7:
                if (validarInt("Nuevo stock minimo: ", tempInt)) p.stockMinimo = tempInt;
                break;
            case 8: {
                int delta;
                cout << "Stock actual: " << p.stock << endl;
                if (validarInt("Cantidad a sumar (+)/restar (-): ", delta, true)) {
                    p.stock += delta;
                    cout << "Nuevo stock: " << p.stock << endl;
                }
                break;
            }
            case 9:
                p.fechaUltimaModificacion = time(nullptr);
                if (escribirRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, idx, p))
                    cout << "Producto actualizado exitosamente." << endl;
                break;
            case 0:
                cout << "Cancelado.\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    } while(op != 0 && op != 9);
}
 
void eliminarProducto() {
    int id;
    if (!validarInt("ID del producto a eliminar (o 'cancelar'): ", id)) return;
 
    int idx = buscarIndiceFisicoPorId<Producto>(ARCHIVO_PRODUCTO, id);
    if (idx == -1) { cout << "Producto no encontrado." << endl; return; }
 
    Producto p;
    leerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, idx, p);
    mostrarProducto(p, true);
 
    cout << "Confirmar eliminacion? (s/n): ";
    char conf[5]; cin.getline(conf, 5);
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }
 
    if (borradoLogico<Producto>(ARCHIVO_PRODUCTO, idx)) {
        Tienda t;
        if (leerTienda(t)) {
            t.totalProductos--;
            t.fechaUltimaModificacion = time(nullptr);
            guardarTienda(t);
        }
        cout << "Producto eliminado (borrado logico)." << endl;
    }
}
 
void menuProductos() {
    int op;
    do {
        limpiarPantalla();
        imprimirLinea(70, '=');
        cout << "            GESTION DE PRODUCTOS" << endl;
        imprimirLinea(70, '=');
        cout << "1. Nuevo producto" << endl;
        cout << "2. Listar productos" << endl;
        cout << "3. Buscar por ID" << endl;
        cout << "4. Buscar por nombre" << endl;
        cout << "5. Actualizar producto" << endl;
        cout << "6. Eliminar producto" << endl;
        cout << "0. Volver" << endl;
        imprimirLinea();
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();
 
        switch(op) {
            case 1: crearProducto(); break;
            case 2: listarProductos(); break;
            case 3: { int id; if(validarInt("ID: ", id)) buscarProductoPorId(id); break; }
            case 4: { char n[100]; if(validarChar("Nombre: ", n, 100)) buscarProductoPorNombre(n); break; }
            case 5: actualizarProducto(); break;
            case 6: eliminarProducto(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) pausar();
    } while(op != 0);
}
 
// --- Funciones de Proveedor ---
 
void mostrarProveedor(const Proveedor& p, bool detallado = false) {
    printf("ID: %-3d | %-25s | RIF: %-12s | Tel: %s\n",
           p.id, p.nombre, p.identificacion, p.telefono);
    if (detallado) {
        cout << "  Email:     " << p.email << endl;
        cout << "  Direccion: " << p.direccion << endl;
        cout << "  Productos: " << p.cantidadProductos << endl;
    }
}
 
void listarProveedores() {
    ArchivoHeader h = leerHeader(ARCHIVO_PROVEEDOR);
    if (h.registrosActivos == 0) {
        cout << "No hay proveedores registrados." << endl;
        return;
    }
    imprimirLinea(70, '=');
    cout << "LISTADO DE PROVEEDORES (" << h.registrosActivos << " activos)" << endl;
    imprimirLinea();
    cout << "ID  | Nombre                      | RIF          | Telefono" << endl;
    imprimirLinea();
 
    Proveedor p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, i, p) && !p.eliminado) {
            mostrarProveedor(p);
        }
    }
    imprimirLinea();
}
 
// --- CRUD de Proveedores ---
 
void crearProveedor() {
    limpiarPantalla();
    cout << "=== REGISTRAR NUEVO PROVEEDOR ===" << endl;
 
    Proveedor p;
    memset(&p, 0, sizeof(Proveedor));
 
    if (!validarChar("Nombre (o 'cancelar'): ", p.nombre, 100)) return;
 
    cout << "Identificacion (RIF, formato J-12345678-9, o 'cancelar'): ";
    cin.getline(p.identificacion, 20);
    if (strcmp(p.identificacion, "cancelar") == 0) { cout << "Cancelado.\n"; return; }
    if (!validarRIF(p.identificacion)) return;
    if (identificacionProveedorExiste(p.identificacion)) {
        cout << "ERROR: Ya existe un proveedor con esa identificacion." << endl;
        return;
    }
 
    if (!validarChar("Telefono (o 'cancelar'): ", p.telefono, 20)) return;
    if (!validarChar("Email (o 'cancelar'): ", p.email, 100)) return;
    if (!validarEmail(p.email)) return;
    if (!validarChar("Direccion (o 'cancelar'): ", p.direccion, 200)) return;
 
    ArchivoHeader h = leerHeader(ARCHIVO_PROVEEDOR);
    p.id = h.proximoId;
    p.cantidadProductos = 0;
    p.eliminado = false;
    p.fechaRegistro = time(nullptr);
    p.fechaUltimaModificacion = time(nullptr);
 
    imprimirLinea();
    cout << "RESUMEN:" << endl;
    mostrarProveedor(p, true);
    imprimirLinea();
    cout << "Confirmar? (s/n): ";
    char conf[5]; cin.getline(conf, 5);
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }
 
    if (escribirRegistroAlFinal<Proveedor>(ARCHIVO_PROVEEDOR, p) >= 0) {
        Tienda t;
        if (leerTienda(t)) {
            t.totalProveedores++;
            t.fechaUltimaModificacion = time(nullptr);
            guardarTienda(t);
        }
        cout << "Proveedor creado con ID: " << p.id << endl;
    }
}
 
void buscarProveedorPorId(int id) {
    int idx = buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, id);
    if (idx == -1) { cout << "Proveedor no encontrado." << endl; return; }
    Proveedor p;
    leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idx, p);
    mostrarProveedor(p, true);
}
 
void buscarProveedorPorNombre(const char* nombre) {
    ArchivoHeader h = leerHeader(ARCHIVO_PROVEEDOR);
    int encontrados = 0;
    Proveedor p;
    imprimirLinea();
    cout << "Resultados para '" << nombre << "':" << endl;
    imprimirLinea();
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, i, p) && !p.eliminado) {
            if (contieneSubstring(p.nombre, nombre)) {
                mostrarProveedor(p);
                encontrados++;
            }
        }
    }
    imprimirLinea();
    if (encontrados == 0) cout << "Sin resultados." << endl;
    else cout << "Total: " << encontrados << endl;
    imprimirLinea();
}
 
void actualizarProveedor() {
    limpiarPantalla();
    int id;
    if (!validarInt("ID del proveedor a actualizar (o 'cancelar'): ", id)) return;
 
    int idx = buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, id);
    if (idx == -1) { cout << "Proveedor no encontrado." << endl; return; }
 
    Proveedor p;
    leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idx, p);
    cout << "Proveedor actual:" << endl;
    mostrarProveedor(p, true);
 
    int op;
    do {
        cout << "\n=== ACTUALIZAR PROVEEDOR ===" << endl;
        cout << "1. Nombre       2. Direccion" << endl;
        cout << "3. Telefono     4. Email" << endl;
        cout << "5. Identificacion  9. Guardar" << endl;
        cout << "0. Cancelar" << endl;
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();
 
        char temp[200];
 
        switch(op) {
            case 1:
                if (validarChar("Nuevo nombre: ", temp, 100)) strcpy(p.nombre, temp);
                break;
            case 2:
                if (validarChar("Nueva direccion: ", temp, 200)) strcpy(p.direccion, temp);
                break;
            case 3:
                if (validarChar("Nuevo telefono: ", temp, 20)) strcpy(p.telefono, temp);
                break;
            case 4:
                if (validarChar("Nuevo email: ", temp, 100) && validarEmail(temp))
                    strcpy(p.email, temp);
                break;
            case 5:
                cout << "Nueva identificacion: ";
                cin.getline(temp, 20);
                if (validarRIF(temp) && !identificacionProveedorExiste(temp, p.id))
                    strcpy(p.identificacion, temp);
                else if (identificacionProveedorExiste(temp, p.id))
                    cout << "ERROR: Ya existe ese RIF." << endl;
                break;
            case 9:
                p.fechaUltimaModificacion = time(nullptr);
                if (escribirRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idx, p))
                    cout << "Proveedor actualizado exitosamente." << endl;
                break;
            case 0:
                cout << "Cancelado.\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    } while(op != 0 && op != 9);
}
 
void eliminarProveedor() {
    int id;
    if (!validarInt("ID del proveedor a eliminar (o 'cancelar'): ", id)) return;
 
    int idx = buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, id);
    if (idx == -1) { cout << "Proveedor no encontrado." << endl; return; }
 
    Proveedor p;
    leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idx, p);
    mostrarProveedor(p, true);
 
    if (p.cantidadProductos > 0) {
        cout << "ADVERTENCIA: Este proveedor tiene " << p.cantidadProductos << " producto(s) asociado(s)." << endl;
    }
 
    cout << "Confirmar eliminacion? (s/n): ";
    char conf[5]; cin.getline(conf, 5);
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }
 
    if (borradoLogico<Proveedor>(ARCHIVO_PROVEEDOR, idx)) {
        Tienda t;
        if (leerTienda(t)) {
            t.totalProveedores--;
            t.fechaUltimaModificacion = time(nullptr);
            guardarTienda(t);
        }
        cout << "Proveedor eliminado (borrado logico)." << endl;
    }
}
 
void menuProveedores() {
    int op;
    do {
        limpiarPantalla();
        imprimirLinea(70, '=');
        cout << "           GESTION DE PROVEEDORES" << endl;
        imprimirLinea(70, '=');
        cout << "1. Nuevo proveedor" << endl;
        cout << "2. Listar proveedores" << endl;
        cout << "3. Buscar por ID" << endl;
        cout << "4. Buscar por nombre" << endl;
        cout << "5. Actualizar proveedor" << endl;
        cout << "6. Eliminar proveedor" << endl;
        cout << "0. Volver" << endl;
        imprimirLinea();
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();
 
        switch(op) {
            case 1: crearProveedor(); break;
            case 2: listarProveedores(); break;
            case 3: { int id; if(validarInt("ID: ", id)) buscarProveedorPorId(id); break; }
            case 4: { char n[100]; if(validarChar("Nombre: ", n, 100)) buscarProveedorPorNombre(n); break; }
            case 5: actualizarProveedor(); break;
            case 6: eliminarProveedor(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) pausar();
    } while(op != 0);
}
 
// --- Funciones de Cliente ---
 
void mostrarCliente(const Cliente& c, bool detallado = false) {
    printf("ID: %-3d | %-25s | CI: %-12s | Tel: %s\n",
           c.id, c.nombre, c.identificacion, c.telefono);
    if (detallado) {
        cout << "  Email:        " << c.email << endl;
        cout << "  Direccion:    " << c.direccion << endl;
        cout << "  Compras:      " << c.cantidadCompras << endl;
        printf("  Total gastado: $%.2f\n", c.totalGastado);
    }
}
 
void listarClientes() {
    ArchivoHeader h = leerHeader(ARCHIVO_CLIENTE);
    if (h.registrosActivos == 0) {
        cout << "No hay clientes registrados." << endl;
        return;
    }
    imprimirLinea(70, '=');
    cout << "LISTADO DE CLIENTES (" << h.registrosActivos << " activos)" << endl;
    imprimirLinea();
    cout << "ID  | Nombre                    | CI           | Telefono" << endl;
    imprimirLinea();
 
    Cliente c;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, i, c) && !c.eliminado) {
            mostrarCliente(c);
        }
    }
    imprimirLinea();
}
 
// --- CRUD de clientes ---
 
void crearCliente(){
    limpiarPantalla();
    cout << "=== REGISTRAR NUEVO CLIENTE ===" << endl;
 
    Cliente c;
    memset(&c, 0, sizeof(Cliente));
 
    if (!validarChar("Nombre (o 'cancelar'): ", c.nombre, 100)) return;
 
    cout << "Cedula (V-12345678 o E-12345678, o 'cancelar'): ";
    cin.getline(c.identificacion, 20);
    if (strcmp(c.identificacion, "cancelar") == 0) { cout << "Cancelado.\n"; return; }
    if (!validarCedula(c.identificacion)) return;
    if (identificacionClienteExiste(c.identificacion)) {
        cout << "ERROR: Ya existe un cliente con esa cedula." << endl;
        return;
    }
 
    if (!validarChar("Telefono (o 'cancelar'): ", c.telefono, 20)) return;
    if (!validarChar("Email (o 'cancelar'): ", c.email, 100)) return;
    if (!validarEmail(c.email)) return;
    if (!validarChar("Direccion (o 'cancelar'): ", c.direccion, 200)) return;
 
    ArchivoHeader h = leerHeader(ARCHIVO_CLIENTE);
    c.id = h.proximoId;
    c.cantidadCompras = 0;
    c.totalGastado = 0;
    c.eliminado = false;
    c.fechaRegistro = time(nullptr);
    c.fechaUltimaModificacion = time(nullptr);
 
    imprimirLinea();
    cout << "RESUMEN:" << endl;
    mostrarCliente(c, true);
    imprimirLinea();
    cout << "Confirmar? (s/n): ";
    char conf[5]; cin.getline(conf, 5);
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }
 
    if (escribirRegistroAlFinal<Cliente>(ARCHIVO_CLIENTE, c) >= 0) {
        Tienda t;
        if (leerTienda(t)) {
            t.totalClientes++;
            t.fechaUltimaModificacion = time(nullptr);
            guardarTienda(t);
        }
        cout << "Cliente creado con ID: " << c.id << endl;
    }
}
 
void buscarClientePorId(int id) {
    int idx = buscarIndiceFisicoPorId<Cliente>(ARCHIVO_CLIENTE, id);
    if (idx == -1) { cout << "Cliente no encontrado." << endl; return; }
    Cliente c;
    leerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, idx, c);
    mostrarCliente(c, true);
}
 
void buscarClientePorNombre(const char* nombre) {
    ArchivoHeader h = leerHeader(ARCHIVO_CLIENTE);
    int encontrados = 0;
    Cliente c;
    imprimirLinea();
    cout << "Resultados para '" << nombre << "':" << endl;
    imprimirLinea();
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, i, c) && !c.eliminado) {
            if (contieneSubstring(c.nombre, nombre)) {
                mostrarCliente(c);
                encontrados++;
            }
        }
    }
    imprimirLinea();
    if (encontrados == 0) cout << "Sin resultados." << endl;
    else cout << "Total: " << encontrados << endl;
    imprimirLinea();
}
 
void actualizarCliente() {
    limpiarPantalla();
    int id;
    if (!validarInt("ID del cliente a actualizar (o 'cancelar'): ", id)) return;
 
    int idx = buscarIndiceFisicoPorId<Cliente>(ARCHIVO_CLIENTE, id);
    if (idx == -1) { cout << "Cliente no encontrado." << endl; return; }
 
    Cliente c;
    leerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, idx, c);
    cout << "Cliente actual:" << endl;
    mostrarCliente(c, true);
 
    int op;
    do {
        cout << "\n=== ACTUALIZAR CLIENTE ===" << endl;
        cout << "1. Nombre    2. Direccion" << endl;
        cout << "3. Telefono  4. Email" << endl;
        cout << "5. Cedula    9. Guardar" << endl;
        cout << "0. Cancelar" << endl;
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();
 
        char temp[200];
 
        switch(op) {
            case 1:
                if (validarChar("Nuevo nombre: ", temp, 100)) strcpy(c.nombre, temp);
                break;
            case 2:
                if (validarChar("Nueva direccion: ", temp, 200)) strcpy(c.direccion, temp);
                break;
            case 3:
                if (validarChar("Nuevo telefono: ", temp, 20)) strcpy(c.telefono, temp);
                break;
            case 4:
                if (validarChar("Nuevo email: ", temp, 100) && validarEmail(temp))
                    strcpy(c.email, temp);
                break;
            case 5:
                cout << "Nueva cedula: ";
                cin.getline(temp, 20);
                if (validarCedula(temp) && !identificacionClienteExiste(temp, c.id))
                    strcpy(c.identificacion, temp);
                else if (identificacionClienteExiste(temp, c.id))
                    cout << "ERROR: Ya existe esa cedula." << endl;
                break;
            case 9:
                c.fechaUltimaModificacion = time(nullptr);
                if (escribirRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, idx, c))
                    cout << "Cliente actualizado exitosamente." << endl;
                break;
            case 0:
                cout << "Cancelado.\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    } while(op != 0 && op != 9);
}
 
void eliminarCliente() {
    int id;
    if (!validarInt("ID del cliente a eliminar (o 'cancelar'): ", id)) return;
 
    int idx = buscarIndiceFisicoPorId<Cliente>(ARCHIVO_CLIENTE, id);
    if (idx == -1) { cout << "Cliente no encontrado." << endl; return; }
 
    Cliente c;
    leerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, idx, c);
    mostrarCliente(c, true);
 
    if (c.cantidadCompras > 0) {
        cout << "ADVERTENCIA: Este cliente tiene " << c.cantidadCompras << " compra(s)." << endl;
    }
 
    cout << "Confirmar eliminacion? (s/n): ";
    char conf[5]; cin.getline(conf, 5);
    if (tolower(conf[0]) != 's') { cout << "Cancelado.\n"; return; }
 
    if (borradoLogico<Cliente>(ARCHIVO_CLIENTE, idx)) {
        Tienda t;
        if (leerTienda(t)) {
            t.totalClientes--;
            t.fechaUltimaModificacion = time(nullptr);
            guardarTienda(t);
        }
        cout << "Cliente eliminado (borrado logico)." << endl;
    }
}
 
void menuClientes() {
    int op;
    do {
        limpiarPantalla();
        imprimirLinea(70, '=');
        cout << "             GESTION DE CLIENTES" << endl;
        imprimirLinea(70, '=');
        cout << "1. Nuevo cliente" << endl;
        cout << "2. Listar clientes" << endl;
        cout << "3. Buscar por ID" << endl;
        cout << "4. Buscar por nombre" << endl;
        cout << "5. Actualizar cliente" << endl;
        cout << "6. Eliminar cliente" << endl;
        cout << "0. Volver" << endl;
        imprimirLinea();
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();
 
        switch(op) {
            case 1: crearCliente(); break;
            case 2: listarClientes(); break;
            case 3: { int id; if(validarInt("ID: ", id)) buscarClientePorId(id); break; }
            case 4: { char n[100]; if(validarChar("Nombre: ", n, 100)) buscarClientePorNombre(n); break; }
            case 5: actualizarCliente(); break;
            case 6: eliminarCliente(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
        if (op != 0) pausar();
    } while(op != 0);
}
 
// --- Funciones de Transaccion ---
 
void mostrarTransaccion(const Transaccion& t, bool detallado = false) {
    const char* contraparte = "N/A";
    char nombreContraparte[100] = "N/A";
 
    if (strcmp(t.tipo, "VENTA") == 0) {
        int idx = buscarIndiceFisicoPorId<Cliente>(ARCHIVO_CLIENTE, t.idCliente);
        if (idx != -1) {
            Cliente c; leerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTE, idx, c);
            strcpy(nombreContraparte, c.nombre);
        }
        contraparte = "Cliente";
    } else {
        int idx = buscarIndiceFisicoPorId<Proveedor>(ARCHIVO_PROVEEDOR, t.idProveedor);
        if (idx != -1) {
            Proveedor p; leerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDOR, idx, p);
            strcpy(nombreContraparte, p.nombre);
        }
        contraparte = "Proveedor";
    }
 
    printf("ID: %-3d | %-6s | %s: %-20s | Items: %d | Total: $%.2f | Fecha: %s\n",
           t.id, t.tipo, contraparte, nombreContraparte,
           t.cantidadItems, t.total, t.fecha);
 
    if (detallado) {
        cout << "  Descripcion: " << t.descripcion << endl;
        imprimirLinea(50);
        cout << "  DETALLE DE ITEMS:" << endl;
        for (int i = 0; i < t.cantidadItems; i++) {
            const ItemTransaccion& item = t.items[i];
            int idxProd = buscarIndiceFisicoPorId<Producto>(ARCHIVO_PRODUCTO, item.idProducto);
            char nomProd[100] = "N/A";
            if (idxProd != -1) {
                Producto prod; leerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTO, idxProd, prod);
                strcpy(nomProd, prod.nombre);
            }
            printf("  %d) %-20s | Cant: %d | P.Unit: $%.2f | Sub: $%.2f\n",
                   i+1, nomProd, item.cantidad, item.precioUnitario, item.subtotal);
        }
        imprimirLinea(50);
    }
}
 
void listarTransacciones() {
    ArchivoHeader h = leerHeader(ARCHIVO_TRANSACCION);
    if (h.registrosActivos == 0) {
        cout << "No hay transacciones registradas." << endl;
        return;
    }
    imprimirLinea(70, '=');
    cout << "LISTADO DE TRANSACCIONES (" << h.registrosActivos << " activas)" << endl;
    imprimirLinea();
 
    Transaccion t;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (leerRegistroPorIndice<Transaccion>(ARCHIVO_TRANSACCION, i, t) && !t.eliminado) {
            mostrarTransaccion(t);
        }
    }
    imprimirLinea();
}
 
// --- Funciones Main (Stub - añadir según necesidad) ---
int main() {
    if (!inicializarSistema()) {
        cout << "Error al inicializar el sistema." << endl;
        return 1;
    }
    
    cout << "Sistema inicializado correctamente. El código restante puede implementarse según sea necesario." << endl;
