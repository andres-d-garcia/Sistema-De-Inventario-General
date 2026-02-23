#include <iostream>
#include <cstring>
#include <ctime>
#include <cctype>
#include <limits>
using namespace std;

struct Producto {
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    int idProveedor;
    float precio;
    int stock;
    char fechaRegistro[20];
};

struct Proveedor {
    int id;
    char nombre[100];
    char direccion[200];
    char telefono[20];
    char email[100];
    char rif[20];
    char fechaRegistro[20];
};

struct Cliente {
    int id;
    char nombre[100];
    char direccion[200];
    char telefono[20];
    char email[100];
    char cedula[20];
    char fechaRegistro[20];
};

struct Transaccion {
    int id;
    char tipo[20];
    int idProducto;
    int idRelacionado;
    int cantidad;
    float precioUnitario;
    float total;
    char fecha[11];
    char descripcion[200];
};

struct Tienda {
    char nombre[100];
    char rif[20];
    Producto* productos;
    int numProductos;
    int capacidadProductos;
    Proveedor* proveedores;
    int numProveedores;
    int capacidadProveedores;
    Cliente* clientes;
    int numClientes;
    int capacidadClientes;
    Transaccion* transacciones;
    int numTransacciones;
    int capacidadTransacciones;
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

struct CambiosProducto {
    bool codigoModificado;
    char nuevoCodigo[20];
    bool nombreModificado;
    char nuevoNombre[100];
    bool descripcionModificada;
    char nuevaDescripcion[200];
    bool proveedorModificado;
    int nuevoIdProveedor;
    bool precioModificado;
    float nuevoPrecio;
    bool stockModificado;
    int nuevoStock;
};

struct CambiosProveedor {
    bool nombreModificado;
    char nuevoNombre[100];
    bool direccionModificada;
    char nuevaDireccion[200];
    bool telefonoModificado;
    char nuevoTelefono[20];
    bool emailModificado;
    char nuevoEmail[100];
    bool rifModificado;
    char nuevoRif[20];
};

void inicializarCambios(CambiosProducto& cambios) {
    cambios.codigoModificado = false;
    cambios.nombreModificado = false;
    cambios.descripcionModificada = false;
    cambios.proveedorModificado = false;
    cambios.precioModificado = false;
    cambios.stockModificado = false;
}

void inicializarCambiosProveedor(CambiosProveedor& cambios) {
    cambios.nombreModificado = false;
    cambios.direccionModificada = false;
    cambios.telefonoModificado = false;
    cambios.emailModificado = false;
    cambios.rifModificado = false;
}

void inicializarTienda(Tienda* tienda, const char* nombre = "Mi Tienda", const char* rif ="J-12345678-9"){
    cout << "Inicializando tienda..." << endl;
    strcpy(tienda->nombre, nombre);
    strcpy(tienda->rif, rif);
    cout << "Nombre: " << tienda->nombre << endl;
    cout << "RIF: " << tienda->rif << endl;
    tienda->productos = new Producto[5];
    tienda->numProductos = 0;
    tienda->capacidadProductos = 5;
    tienda->proveedores = new Proveedor[5];
    tienda->numProveedores = 0;
    tienda->capacidadProveedores = 5;
    tienda->clientes = new Cliente[5];
    tienda->numClientes = 0;
    tienda->capacidadClientes = 5;
    tienda->transacciones = new Transaccion[5];
    tienda->numTransacciones = 0;   
    tienda->capacidadTransacciones = 5;
    tienda->siguienteIdProducto = 1;
    tienda->siguienteIdProveedor = 1;
    tienda->siguienteIdCliente = 1;
    tienda->siguienteIdTransaccion = 1;
    cout << "Tienda inicializada correctamente." << endl;
}

void liberarTienda(Tienda* tienda) {
    delete[] tienda->productos;
    delete[] tienda->proveedores;
    delete[] tienda->clientes;
    delete[] tienda->transacciones;
    cout << "Memoria de la tienda liberada correctamente." << endl;
}

void convertirAminusculas(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = tolower(origen[i]);
        i++;
    }
    destino[i] = '\0';
}

bool contieneSubstring(const char* str, const char* substr) {
    char strMinusculas[200];
    char substrMinusculas[200];
    convertirAminusculas(strMinusculas, str);
    convertirAminusculas(substrMinusculas, substr);
    return strstr(strMinusculas, substrMinusculas) != nullptr;
}

bool validarChar(const char* mensaje, char* destino, int tamanio) {
    cout << mensaje;
    cin.getline(destino, tamanio);
    if (strlen(destino) == 0) {
        cout << "Error: El campo no puede estar vacío." << endl;
        return false;
    } else if (strcmp(destino, "cancelar") == 0) {
        cout << "Operación cancelada por el usuario." << endl;
        return false;
    } else {
        return true;
    }
}

bool validarFloat(const char* mensaje, float& destino) {
    cout << mensaje;
    string input;
    cin >> input;
    
    if (input.empty()) {
        cout << "Error: El campo no puede estar vacío." << endl;
        return false;
    }
    if (input == "cancelar") {
        cout << "Operación cancelada por el usuario." << endl;
        return false;
    }
    try {
        destino = stof(input);
        if (destino < 0) {
            cout << "Error: El valor no puede ser negativo." << endl;
            return false;
        }
        return true;
    } catch (const invalid_argument&) {
        cout << "Error: Entrada no válida. Por favor ingrese un número válido." << endl;
        return false;
    }
}

bool validarInt(const char* mensaje, int& destino, bool permitirNegativos = false) {
    cout << mensaje;
    string input;
    cin >> input;
    
    if (input.empty()) {
        cout << "Error: El campo no puede estar vacío." << endl;
        return false;
    }
    if (input == "cancelar") {
        cout << "Operación cancelada por el usuario." << endl;
        return false;
    }
    try {
        destino = stoi(input);
        if (!permitirNegativos && destino < 0) {
            cout << "Error: El valor no puede ser negativo." << endl;
            return false;
        }
        return true;
    } catch (const invalid_argument&) {
        cout << "Error: Entrada no válida. Por favor ingrese un número válido." << endl;
        return false;
    }
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10 || fecha[4] != '-' || fecha[7] != '-') {
        cout << "Error: El formato de fecha debe ser YYYY-MM-DD." << endl;
        return false;
    }
    int anio = atoi(string(fecha, 4).c_str());
    int mes = atoi(string(fecha + 5, 2).c_str());
    int dia = atoi(string(fecha + 8, 2).c_str());
    
    if (anio < 1900 || anio > 2100) {
        cout << "Error: El año debe estar entre 1900 y 2100." << endl;
        return false;
    }
    if (mes < 1 || mes > 12) {
        cout << "Error: El mes debe estar entre 1 y 12." << endl;
        return false;
    }
    if (dia < 1 || dia > 31) {
        cout << "Error: El día debe estar entre 1 y 31." << endl;
        return false;
    }
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        cout << "Error: El mes seleccionado tiene solo 30 días." << endl;
        return false;
    }
    return true;
}

bool validarCodigoUnico(Tienda* tienda, const char* codigo) {
    for (int i = 0; i < tienda->numProductos; i++) {
        if (strcmp(tienda->productos[i].codigo, codigo) == 0) {
            cout << "Error: El código '" << codigo << "' ya existe para otro producto." << endl;
            return false;
        }
    }
    return true;
}

bool validarRifUnico(Tienda* tienda, const char* rif, int idExcluir = -1) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (tienda->proveedores[i].id != idExcluir && strcmp(tienda->proveedores[i].rif, rif) == 0) {
            cout << "Error: El RIF '" << rif << "' ya existe para otro proveedor." << endl;
            return false;
        }
    }
    return true;
}

bool validarCedulaUnica(Tienda* tienda, const char* cedula) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (strcmp(tienda->clientes[i].cedula, cedula) == 0) {
            cout << "Error: La cédula '" << cedula << "' ya existe para otro cliente." << endl;
            return false;
        }
    }
    return true;
}

bool validarEmail(const char* email) {
    const char* atPos = strchr(email, '@');
    if (atPos == nullptr || atPos == email || atPos == email + strlen(email) - 1) {
        cout << "Error: El correo electrónico debe contener un '@' válido." << endl;
        return false;
    }
    const char* dotPos = strrchr(atPos, '.');
    if (dotPos == nullptr || dotPos == atPos + 1 || dotPos == email + strlen(email) - 1) {
        cout << "Error: El correo electrónico debe contener un '.' después del '@'." << endl;
        return false;
    }
    return true;
}

void redimensionarArrayProducto(Tienda* tienda) {
    int nuevaCapacidad = tienda->capacidadProductos * 2;
    Producto* nuevoArray = new Producto[nuevaCapacidad];
    for (int i = 0; i < tienda->numProductos; i++) {
        nuevoArray[i] = tienda->productos[i];
    }
    delete[] tienda->productos;
    tienda->productos = nuevoArray;
    tienda->capacidadProductos = nuevaCapacidad;
    cout << "Capacidad de productos aumentada a " << tienda->capacidadProductos << "." << endl;
}

void redimensionarArrayProveedores(Tienda* tienda) {
    int nuevaCapacidad = tienda->capacidadProveedores * 2;
    Proveedor* nuevoArray = new Proveedor[nuevaCapacidad];
    for (int i = 0; i < tienda->numProveedores; i++) {
        nuevoArray[i] = tienda->proveedores[i];
    }
    delete[] tienda->proveedores;
    tienda->proveedores = nuevoArray;
    tienda->capacidadProveedores = nuevaCapacidad;
    cout << "Capacidad de proveedores aumentada a " << tienda->capacidadProveedores << "." << endl;
}

void redimensionarArrayClientes(Tienda* tienda) {
    int nuevaCapacidad = tienda->capacidadClientes * 2;
    Cliente* nuevoArray = new Cliente[nuevaCapacidad];
    for (int i = 0; i < tienda->numClientes; i++) {
        nuevoArray[i] = tienda->clientes[i];
    }
    delete[] tienda->clientes;
    tienda->clientes = nuevoArray;
    tienda->capacidadClientes = nuevaCapacidad;
    cout << "Capacidad de clientes aumentada a " << tienda->capacidadClientes << "." << endl;
}

void redimensionarArrayTransacciones(Tienda* tienda) {
    int nuevaCapacidad = tienda->capacidadTransacciones * 2;
    Transaccion* nuevoArray = new Transaccion[nuevaCapacidad];
    for (int i = 0; i < tienda->numTransacciones; i++) {
        nuevoArray[i] = tienda->transacciones[i];
    }
    delete[] tienda->transacciones;
    tienda->transacciones = nuevoArray;
    tienda->capacidadTransacciones = nuevaCapacidad;
    cout << "Capacidad de transacciones aumentada a " << tienda->capacidadTransacciones << "." << endl;
}

void tomarFechaActual(char* buffer, int tamanio) {
    time_t tiempo = time(nullptr);
    tm* localTime = localtime(&tiempo);
    strftime(buffer, tamanio, "%Y-%m-%d", localTime);
}

int buscarIndiceProductoPorId(Tienda* tienda, int id) {
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].id == id) {
            return i;
        }
    }
    return -1;
}

int buscarIndiceProveedorPorId(Tienda* tienda, int id) {
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (tienda->proveedores[i].id == id) {
            return i;
        }
    }
    return -1;
}

int buscarIndiceClientePorId(Tienda* tienda, int id) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (tienda->clientes[i].id == id) {
            return i;
        }
    }
    return -1;
}

int buscarIndiceTransaccionPorId(Tienda* tienda, int id) {
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (tienda->transacciones[i].id == id) {
            return i;
        }
    }
    return -1;
}

// ==================== FUNCIONES PARA PRODUCTOS ====================

void crearProducto(Tienda* tienda) {
    Producto nuevoProducto;
    cout << "Desea agregar un nuevo producto? (s/n): ";
    char respuesta;
    cin >> respuesta;
    cin.ignore(10000, '\n');
    if (respuesta != 's' && respuesta != 'S') {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    if (!validarChar("Ingrese el código del producto (o 'cancelar' para cancelar): ", nuevoProducto.codigo, 20)) {
        return;
    }
    if (!validarCodigoUnico(tienda, nuevoProducto.codigo)) {
        return;
    }
    if (!validarChar("Ingrese el nombre del producto (o 'cancelar' para cancelar): ", nuevoProducto.nombre, 100)) {
        return;
    }
    if (!validarChar("Ingrese la descripción del producto (o 'cancelar' para cancelar): ", nuevoProducto.descripcion, 200)) {
        return;
    }
    if (!validarFloat("Ingrese el ID del proveedor (o 'cancelar' para cancelar): ", (float&)nuevoProducto.idProveedor)) {
        return;
    }
    if (!validarFloat("Ingrese el precio del producto (o 'cancelar' para cancelar): ", nuevoProducto.precio)) {
        return;
    }
    if (!validarInt("Ingrese el stock del producto (o 'cancelar' para cancelar): ", nuevoProducto.stock)) {
        return;
    }
    tomarFechaActual(nuevoProducto.fechaRegistro, sizeof(nuevoProducto.fechaRegistro));
    nuevoProducto.id = tienda->siguienteIdProducto++;
    cout << "-----------------------------" << endl;
    cout << "Resumen del nuevo producto:" << endl;
    cout << "ID: " << nuevoProducto.id << endl;
    cout << "Código: " << nuevoProducto.codigo << endl;
    cout << "Nombre: " << nuevoProducto.nombre << endl;
    cout << "Descripción: " << nuevoProducto.descripcion << endl;
    cout << "ID Proveedor: " << nuevoProducto.idProveedor << endl;
    cout << "Precio: " << nuevoProducto.precio << "$" << endl;
    cout << "Stock: " << nuevoProducto.stock << endl;
    cout << "Fecha de Registro: " << nuevoProducto.fechaRegistro << endl;
    cout << "-----------------------------" << endl;
    cout << "\n ¿Desea confirmar la creación de este producto? (s/n): "; 
    char confirmacion;
    cin >> confirmacion;
    cin.ignore(10000, '\n');
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    if (tienda->numProductos >= tienda->capacidadProductos) {
        redimensionarArrayProducto(tienda);
    }
    tienda->productos[tienda->numProductos] = nuevoProducto;
    tienda->numProductos++;
    cout << "Producto creado exitosamente con ID: " << nuevoProducto.id << endl;
}

void listarProductos(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "No hay productos registrados." << endl;
        return;
    }
    cout << "Lista de productos en la tienda:" << endl;
    for (int i = 0; i < tienda->numProductos; i++) {
        Producto& p = tienda->productos[i];
        cout << "ID: " << p.id << ", Código: " << p.codigo << ", Nombre: " << p.nombre 
             << ", Precio: " << p.precio << "$, Stock: " << p.stock 
             << ", Fecha Registro: " << p.fechaRegistro << endl;
    }
}

void eliminarProducto(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "No hay productos para eliminar." << endl;
        return;
    }
    
    int id;
    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> id;
    cin.ignore(10000, '\n');
    
    int indice = buscarIndiceProductoPorId(tienda, id);
    if (indice == -1) {
        cout << "Producto con ID " << id << " no encontrado. No se puede eliminar.\n";
        return;
    }
    
    cout << "\nProducto a eliminar:\n";
    Producto& p = tienda->productos[indice];
    cout << "ID: " << p.id << ", Código: " << p.codigo << ", Nombre: " << p.nombre 
         << ", Precio: " << p.precio << "$, Stock: " << p.stock << endl;
    
    cout << "¿Está seguro que desea eliminar este producto? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    cin.ignore(10000, '\n');
    
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Eliminación cancelada.\n";
        return;
    }
    
    for (int i = indice; i < tienda->numProductos - 1; i++) {
        tienda->productos[i] = tienda->productos[i + 1];
    }
    tienda->numProductos--;
    cout << "Producto con ID " << id << " eliminado exitosamente.\n";
}

void buscarProductoPorId(Tienda* tienda, int id) {
    int indice = buscarIndiceProductoPorId(tienda, id);
    if (indice != -1) {
        Producto& p = tienda->productos[indice];
        cout << "Producto encontrado: ID: " << p.id << ", Código: " << p.codigo 
             << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
             << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
    } else {
        cout << "Producto con ID " << id << " no encontrado.\n";
    }
}

void buscarProductoPorNombre(Tienda* tienda, const char* nombre) {
    int encontrados = 0;
    cout << "Resultados de búsqueda por nombre '" << nombre << "':\n";
    for (int i = 0; i < tienda->numProductos; i++) {
        if (contieneSubstring(tienda->productos[i].nombre, nombre)) {
            Producto& p = tienda->productos[i];
            cout << "ID: " << p.id << ", Código: " << p.codigo 
                 << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
                 << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
            encontrados++;
        }
    }
    if (encontrados == 0) {
        cout << "No se encontraron productos con nombre que contenga '" << nombre << "'.\n";
    } else {
        cout << "Total de productos encontrados: " << encontrados << endl;
    }
}

void buscarProductoPorCodigo(Tienda* tienda, const char* codigo) {
    int encontrados = 0;
    cout << "Resultados de búsqueda por código '" << codigo << "':\n";
    for (int i = 0; i < tienda->numProductos; i++) {
        if (contieneSubstring(tienda->productos[i].codigo, codigo)) {
            Producto& p = tienda->productos[i];
            cout << "ID: " << p.id << ", Código: " << p.codigo 
                 << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
                 << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
            encontrados++;
        }
    }
    if (encontrados == 0) {
        cout << "No se encontraron productos con código que contenga '" << codigo << "'.\n";
    } else {
        cout << "Total de productos encontrados: " << encontrados << endl;
    }
}

void buscarProductoPorProveedor(Tienda* tienda, int idProveedor) {
    int encontrados = 0;
    cout << "Resultados de búsqueda por ID de proveedor '" << idProveedor << "':\n";
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].idProveedor == idProveedor) {
            Producto& p = tienda->productos[i];
            cout << "ID: " << p.id << ", Código: " << p.codigo 
                 << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
                 << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
            encontrados++;
        }
    }
    if (encontrados == 0) {
        cout << "No se encontraron productos para el proveedor con ID '" << idProveedor << "'.\n";
    } else {
        cout << "Total de productos encontrados: " << encontrados << endl;
    }
}

void buscarProducto(Tienda* tienda) {
    int opcion;
    char busqueda[100];
    int idProveedor;
    do {
        cout << "\n=== BUSCAR PRODUCTO ===\n";
        cout << "1. Buscar por ID (exacto)\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por código (coincidencia parcial)\n";
        cout << "4. Listar por proveedor\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
        switch(opcion) {
            case 1: {
                cout << "Ingrese ID del producto: ";
                int id;
                cin >> id;
                cin.ignore(10000, '\n');
                buscarProductoPorId(tienda, id);
                break;
            }
            case 2:
                cout << "Ingrese nombre a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProductoPorNombre(tienda, busqueda);
                break;
            case 3:
                cout << "Ingrese código a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProductoPorCodigo(tienda, busqueda);
                break;
            case 4:
                cout << "Ingrese ID del proveedor: ";
                cin >> idProveedor;
                cin.ignore(10000, '\n');
                buscarProductoPorProveedor(tienda, idProveedor);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opción no válida\n";
        }
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
    } while(opcion != 0);
}

void apCodigo(CambiosProducto& cambios, const char* nuevoCodigo) {
    strcpy(cambios.nuevoCodigo, nuevoCodigo);
    cambios.codigoModificado = true;
    cout << "Cambio en código pendiente de guardar.\n";
}

void apNombre(CambiosProducto& cambios, const char* nuevoNombre) {
    strcpy(cambios.nuevoNombre, nuevoNombre);
    cambios.nombreModificado = true;
    cout << "Cambio en nombre pendiente de guardar.\n";
}

void apDescripcion(CambiosProducto& cambios, const char* nuevaDescripcion) {
    strcpy(cambios.nuevaDescripcion, nuevaDescripcion);
    cambios.descripcionModificada = true;
    cout << "Cambio en descripción pendiente de guardar.\n";
}

void apProveedor(CambiosProducto& cambios, int nuevoIdProveedor) {
    cambios.nuevoIdProveedor = nuevoIdProveedor;
    cambios.proveedorModificado = true;
    cout << "Cambio en proveedor pendiente de guardar.\n";
}

void apPrecio(CambiosProducto& cambios, float nuevoPrecio) {
    cambios.nuevoPrecio = nuevoPrecio;
    cambios.precioModificado = true;
    cout << "Cambio en precio pendiente de guardar.\n";
}

void apStock(CambiosProducto& cambios, int nuevoStock) {
    cambios.nuevoStock = nuevoStock;
    cambios.stockModificado = true;
    cout << "Cambio en stock pendiente de guardar.\n";
}

void sumarStock(CambiosProducto& cambios, int cantidad, int stockActual) {
    if (cambios.stockModificado) {
        cambios.nuevoStock += cantidad;
    } else {
        cambios.nuevoStock = stockActual + cantidad;
    }
    cambios.stockModificado = true;
    cout << "Cambio en stock pendiente de guardar. Nuevo stock temporal: " << cambios.nuevoStock << endl;
}

void mostrarCambiosPendientes(const CambiosProducto& cambios) {
    cout << "\n=== CAMBIOS PENDIENTES ===\n";
    if (cambios.codigoModificado) cout << "- Código: " << cambios.nuevoCodigo << endl;
    if (cambios.nombreModificado) cout << "- Nombre: " << cambios.nuevoNombre << endl;
    if (cambios.descripcionModificada) cout << "- Descripción: " << cambios.nuevaDescripcion << endl;
    if (cambios.proveedorModificado) cout << "- ID Proveedor: " << cambios.nuevoIdProveedor << endl;
    if (cambios.precioModificado) cout << "- Precio: " << cambios.nuevoPrecio << endl;
    if (cambios.stockModificado) cout << "- Stock: " << cambios.nuevoStock << endl;
    if (!cambios.codigoModificado && !cambios.nombreModificado && !cambios.descripcionModificada && 
        !cambios.proveedorModificado && !cambios.precioModificado && !cambios.stockModificado) {
        cout << "No hay cambios pendientes.\n";
    }
}

void aplicarCambios(Tienda* tienda, int indice, const CambiosProducto& cambios) {
    if (indice < 0 || indice >= tienda->numProductos) {
        cout << "Error: Índice de producto inválido.\n";
        return;
    }
    
    bool cambiosAplicados = false;
    
    if (cambios.codigoModificado) {
        if (validarCodigoUnico(tienda, cambios.nuevoCodigo)) {
            strcpy(tienda->productos[indice].codigo, cambios.nuevoCodigo);
            cambiosAplicados = true;
        } else {
            cout << "No se pudo actualizar el código: ya existe.\n";
        }
    }
    if (cambios.nombreModificado) {
        strcpy(tienda->productos[indice].nombre, cambios.nuevoNombre);
        cambiosAplicados = true;
    }
    if (cambios.descripcionModificada) {
        strcpy(tienda->productos[indice].descripcion, cambios.nuevaDescripcion);
        cambiosAplicados = true;
    }
    if (cambios.proveedorModificado) {
        tienda->productos[indice].idProveedor = cambios.nuevoIdProveedor;
        cambiosAplicados = true;
    }
    if (cambios.precioModificado) {
        tienda->productos[indice].precio = cambios.nuevoPrecio;
        cambiosAplicados = true;
    }
    if (cambios.stockModificado) {
        tienda->productos[indice].stock = cambios.nuevoStock;
        cambiosAplicados = true;
    }
    
    if (cambiosAplicados) {
        cout << "Cambios guardados exitosamente.\n";
    } else {
        cout << "No se aplicaron cambios.\n";
    }
}

void actualizarProducto(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "No hay productos registrados para actualizar.\n";
        return;
    }
    
    int id;
    cout << "Ingrese el ID del producto que desea actualizar: ";
    cin >> id;
    cin.ignore(10000, '\n');
    
    int indice = buscarIndiceProductoPorId(tienda, id);
    if (indice == -1) {
        cout << "Producto con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nProducto seleccionado:\n";
    Producto& p = tienda->productos[indice];
    cout << "ID: " << p.id << ", Código: " << p.codigo << ", Nombre: " << p.nombre 
         << ", Precio: " << p.precio << "$, Stock: " << p.stock << endl;
    
    CambiosProducto cambios;
    inicializarCambios(cambios);
    
    int opcion;
    do {
        cout << "\n=== ACTUALIZAR PRODUCTO ===\n";
        cout << "1. Editar código\n";
        cout << "2. Editar nombre\n";
        cout << "3. Editar descripción\n";
        cout << "4. Editar proveedor\n";
        cout << "5. Editar precio\n";
        cout << "6. Editar stock (reemplazar)\n";
        cout << "7. Ajustar stock (sumar/restar)\n";
        cout << "8. Ver cambios pendientes\n";
        cout << "9. Guardar todos los cambios\n";
        cout << "0. Cancelar y salir sin guardar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
        
        switch(opcion) {
            case 1: {
                char nuevoCodigo[20];
                if (validarChar("Ingrese el nuevo código: ", nuevoCodigo, 20)) {
                    apCodigo(cambios, nuevoCodigo);
                }
                break;
            }
            case 2: {
                char nuevoNombre[100];
                if (validarChar("Ingrese el nuevo nombre: ", nuevoNombre, 100)) {
                    apNombre(cambios, nuevoNombre);
                }
                break;
            }
            case 3: {
                char nuevaDescripcion[200];
                if (validarChar("Ingrese la nueva descripción: ", nuevaDescripcion, 200)) {
                    apDescripcion(cambios, nuevaDescripcion);
                }
                break;
            }
            case 4: {
                int nuevoIdProveedor;
                if (validarInt("Ingrese el nuevo ID del proveedor: ", nuevoIdProveedor)) {
                    apProveedor(cambios, nuevoIdProveedor);
                }
                break;
            }
            case 5: {
                float nuevoPrecio;
                if (validarFloat("Ingrese el nuevo precio: ", nuevoPrecio)) {
                    apPrecio(cambios, nuevoPrecio);
                }
                break;
            }
            case 6: {
                int nuevoStock;
                cout << "Stock actual: " << p.stock << endl;
                if (validarInt("Ingrese el nuevo stock: ", nuevoStock)) {
                    apStock(cambios, nuevoStock);
                }
                break;
            }
            case 7: {
                int cantidad;
                cout << "Stock actual: " << p.stock << endl;
                if (validarInt("Ingrese cantidad a sumar (positiva) o restar (negativa): ", cantidad, true)) {
                    sumarStock(cambios, cantidad, p.stock);
                }
                break;
            }
            case 8:
                mostrarCambiosPendientes(cambios);
                break;
            case 9:
                aplicarCambios(tienda, indice, cambios);
                opcion = 0;
                break;
            case 0:
                cout << "Actualización cancelada. No se guardaron cambios.\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
        
        if (opcion != 0 && opcion != 9) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
        
    } while(opcion != 0);
}

// ==================== FUNCIONES PARA PROVEEDORES ====================

void listarProveedores(Tienda* tienda) {
    if (tienda->numProveedores == 0) {
        cout << "No hay proveedores registrados." << endl;
        return;
    }
    
    cout << "\n=== LISTADO DE PROVEEDORES ===\n";
    cout << "ID\tNombre\t\tRIF\t\tTeléfono\tEmail\n";
    cout << "----------------------------------------------------------------------------\n";
    
    for (int i = 0; i < tienda->numProveedores; i++) {
        Proveedor& p = tienda->proveedores[i];
        cout << p.id << "\t" << p.nombre << "\t" << p.rif << "\t" << p.telefono << "\t" << p.email << endl;
    }
    cout << "Total de proveedores: " << tienda->numProveedores << endl;
}

void buscarProveedorPorId(Tienda* tienda, int id) {
    int indice = buscarIndiceProveedorPorId(tienda, id);
    if (indice != -1) {
        Proveedor& p = tienda->proveedores[indice];
        cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
        cout << "ID: " << p.id << endl;
        cout << "Nombre: " << p.nombre << endl;
        cout << "RIF: " << p.rif << endl;
        cout << "Teléfono: " << p.telefono << endl;
        cout << "Email: " << p.email << endl;
        cout << "Dirección: " << p.direccion << endl;
        cout << "Fecha de Registro: " << p.fechaRegistro << endl;
    } else {
        cout << "Proveedor con ID " << id << " no encontrado.\n";
    }
}

void buscarProveedorPorNombre(Tienda* tienda, const char* nombre) {
    int encontrados = 0;
    cout << "\n=== RESULTADOS DE BÚSQUEDA POR NOMBRE: '" << nombre << "' ===\n";
    
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (contieneSubstring(tienda->proveedores[i].nombre, nombre)) {
            Proveedor& p = tienda->proveedores[i];
            cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", RIF: " << p.rif 
                 << ", Teléfono: " << p.telefono << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron proveedores con nombre que contenga '" << nombre << "'.\n";
    } else {
        cout << "Total de proveedores encontrados: " << encontrados << endl;
    }
}

void buscarProveedorPorRif(Tienda* tienda, const char* rif) {
    int encontrados = 0;
    cout << "\n=== RESULTADOS DE BÚSQUEDA POR RIF: '" << rif << "' ===\n";
    
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (contieneSubstring(tienda->proveedores[i].rif, rif)) {
            Proveedor& p = tienda->proveedores[i];
            cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", RIF: " << p.rif 
                 << ", Teléfono: " << p.telefono << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron proveedores con RIF que contenga '" << rif << "'.\n";
    } else {
        cout << "Total de proveedores encontrados: " << encontrados << endl;
    }
}

void buscarProveedor(Tienda* tienda) {
    int opcion;
    char busqueda[100];
    
    do {
        cout << "\n=== BUSCAR PROVEEDOR ===\n";
        cout << "1. Buscar por ID (exacto)\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por RIF (coincidencia parcial)\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
        
        switch(opcion) {
            case 1: {
                int id;
                cout << "Ingrese ID del proveedor: ";
                cin >> id;
                cin.ignore(10000, '\n');
                buscarProveedorPorId(tienda, id);
                break;
            }
            case 2:
                cout << "Ingrese nombre a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProveedorPorNombre(tienda, busqueda);
                break;
            case 3:
                cout << "Ingrese RIF a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProveedorPorRif(tienda, busqueda);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opción no válida\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
        
    } while(opcion != 0);
}

void crearProveedor(Tienda* tienda) {
    cout << "Desea registrar un nuevo proveedor? (s/n): ";
    char respuesta;
    cin >> respuesta;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (respuesta != 's' && respuesta != 'S') {
        cout << "Creación de proveedor cancelada." << endl;
        return;
    }
    
    Proveedor nuevoProveedor;
    
    cout << "\n=== REGISTRO DE NUEVO PROVEEDOR ===\n";
    
    if (!validarChar("Ingrese el nombre del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.nombre, 100)) {
        return;
    }
    
    if (!validarChar("Ingrese el RIF del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.rif, 20)) {
        return;
    }
    
    if (!validarRifUnico(tienda, nuevoProveedor.rif)) {
        return;
    }
    
    if (!validarChar("Ingrese el teléfono del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.telefono, 20)) {
        return;
    }
    
    if (!validarChar("Ingrese el email del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.email, 100)) {
        return;
    }
    
    if (!validarEmail(nuevoProveedor.email)) {
        return;
    }
    
    if (!validarChar("Ingrese la dirección del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.direccion, 200)) {
        return;
    }
    
    tomarFechaActual(nuevoProveedor.fechaRegistro, sizeof(nuevoProveedor.fechaRegistro));
    nuevoProveedor.id = tienda->siguienteIdProveedor;
    
    cout << "\n-----------------------------\n";
    cout << "Resumen del nuevo proveedor:\n";
    cout << "ID: " << nuevoProveedor.id << endl;
    cout << "Nombre: " << nuevoProveedor.nombre << endl;
    cout << "RIF: " << nuevoProveedor.rif << endl;
    cout << "Teléfono: " << nuevoProveedor.telefono << endl;
    cout << "Email: " << nuevoProveedor.email << endl;
    cout << "Dirección: " << nuevoProveedor.direccion << endl;
    cout << "Fecha de Registro: " << nuevoProveedor.fechaRegistro << endl;
    cout << "-----------------------------\n";
    
    cout << "\n¿Desea confirmar la creación de este proveedor? (s/n): ";
char linea[10];
cin.getline(linea, 10);

if (tolower(linea[0]) != 's') {
    cout << "Creación de proveedor cancelada." << endl;
    return;
}

if (tienda->numProveedores >= tienda->capacidadProveedores) {
    redimensionarArrayProveedores(tienda);
}

tienda->proveedores[tienda->numProveedores] = nuevoProveedor;
tienda->numProveedores++;

cout << "Proveedor creado exitosamente con ID: " << nuevoProveedor.id << endl;
}

void apNombreProveedor(CambiosProveedor& cambios, const char* nuevoNombre) {
    strcpy(cambios.nuevoNombre, nuevoNombre);
    cambios.nombreModificado = true;
    cout << "Cambio en nombre pendiente de guardar.\n";
}

void apDireccionProveedor(CambiosProveedor& cambios, const char* nuevaDireccion) {
    strcpy(cambios.nuevaDireccion, nuevaDireccion);
    cambios.direccionModificada = true;
    cout << "Cambio en dirección pendiente de guardar.\n";
}

void apTelefonoProveedor(CambiosProveedor& cambios, const char* nuevoTelefono) {
    strcpy(cambios.nuevoTelefono, nuevoTelefono);
    cambios.telefonoModificado = true;
    cout << "Cambio en teléfono pendiente de guardar.\n";
}

void apEmailProveedor(CambiosProveedor& cambios, const char* nuevoEmail) {
    strcpy(cambios.nuevoEmail, nuevoEmail);
    cambios.emailModificado = true;
    cout << "Cambio en email pendiente de guardar.\n";
}

void apRifProveedor(CambiosProveedor& cambios, const char* nuevoRif) {
    strcpy(cambios.nuevoRif, nuevoRif);
    cambios.rifModificado = true;
    cout << "Cambio en RIF pendiente de guardar.\n";
}

void mostrarCambiosPendientesProveedor(const CambiosProveedor& cambios) {
    cout << "\n=== CAMBIOS PENDIENTES ===\n";
    if (cambios.nombreModificado) cout << "- Nombre: " << cambios.nuevoNombre << endl;
    if (cambios.direccionModificada) cout << "- Dirección: " << cambios.nuevaDireccion << endl;
    if (cambios.telefonoModificado) cout << "- Teléfono: " << cambios.nuevoTelefono << endl;
    if (cambios.emailModificado) cout << "- Email: " << cambios.nuevoEmail << endl;
    if (cambios.rifModificado) cout << "- RIF: " << cambios.nuevoRif << endl;
    
    if (!cambios.nombreModificado && !cambios.direccionModificada && 
        !cambios.telefonoModificado && !cambios.emailModificado && !cambios.rifModificado) {
        cout << "No hay cambios pendientes.\n";
    }
}

void aplicarCambiosProveedor(Tienda* tienda, int indice, const CambiosProveedor& cambios) {
    if (indice < 0 || indice >= tienda->numProveedores) {
        cout << "Error: Índice de proveedor inválido.\n";
        return;
    }
    
    bool cambiosAplicados = false;
    
    if (cambios.nombreModificado) {
        strcpy(tienda->proveedores[indice].nombre, cambios.nuevoNombre);
        cambiosAplicados = true;
    }
    
    if (cambios.direccionModificada) {
        strcpy(tienda->proveedores[indice].direccion, cambios.nuevaDireccion);
        cambiosAplicados = true;
    }
    
    if (cambios.telefonoModificado) {
        strcpy(tienda->proveedores[indice].telefono, cambios.nuevoTelefono);
        cambiosAplicados = true;
    }
    
    if (cambios.emailModificado) {
        if (validarEmail(cambios.nuevoEmail)) {
            strcpy(tienda->proveedores[indice].email, cambios.nuevoEmail);
            cambiosAplicados = true;
        } else {
            cout << "No se pudo actualizar el email: formato inválido.\n";
        }
    }
    
    if (cambios.rifModificado) {
        if (validarRifUnico(tienda, cambios.nuevoRif, tienda->proveedores[indice].id)) {
            strcpy(tienda->proveedores[indice].rif, cambios.nuevoRif);
            cambiosAplicados = true;
        } else {
            cout << "No se pudo actualizar el RIF: ya existe.\n";
        }
    }
    
    if (cambiosAplicados) {
        cout << "Cambios guardados exitosamente.\n";
    } else {
        cout << "No se aplicaron cambios.\n";
    }
}

void actualizarProveedor(Tienda* tienda) {
    if (tienda->numProveedores == 0) {
        cout << "No hay proveedores registrados para actualizar.\n";
        return;
    }
    
    int id;
    cout << "Ingrese el ID del proveedor que desea actualizar: ";
    cin >> id;
    cin.ignore(10000, '\n');
    
    int indice = buscarIndiceProveedorPorId(tienda, id);
    if (indice == -1) {
        cout << "Proveedor con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nProveedor seleccionado:\n";
    Proveedor& p = tienda->proveedores[indice];
    cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", RIF: " << p.rif 
         << ", Teléfono: " << p.telefono << ", Email: " << p.email << endl;
    
    CambiosProveedor cambios;
    inicializarCambiosProveedor(cambios);
    
    int opcion;
    do {
        cout << "\n=== ACTUALIZAR PROVEEDOR ===\n";
        cout << "1. Editar nombre\n";
        cout << "2. Editar dirección\n";
        cout << "3. Editar teléfono\n";
        cout << "4. Editar email\n";
        cout << "5. Editar RIF\n";
        cout << "6. Ver cambios pendientes\n";
        cout << "7. Guardar todos los cambios\n";
        cout << "0. Cancelar y salir sin guardar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
        
        switch(opcion) {
            case 1: {
                char nuevoNombre[100];
                if (validarChar("Ingrese el nuevo nombre: ", nuevoNombre, 100)) {
                    apNombreProveedor(cambios, nuevoNombre);
                }
                break;
            }
            case 2: {
                char nuevaDireccion[200];
                if (validarChar("Ingrese la nueva dirección: ", nuevaDireccion, 200)) {
                    apDireccionProveedor(cambios, nuevaDireccion);
                }
                break;
            }
            case 3: {
                char nuevoTelefono[20];
                if (validarChar("Ingrese el nuevo teléfono: ", nuevoTelefono, 20)) {
                    apTelefonoProveedor(cambios, nuevoTelefono);
                }
                break;
            }
            case 4: {
                char nuevoEmail[100];
                if (validarChar("Ingrese el nuevo email: ", nuevoEmail, 100)) {
                    if (validarEmail(nuevoEmail)) {
                        apEmailProveedor(cambios, nuevoEmail);
                    }
                }
                break;
            }
            case 5: {
                char nuevoRif[20];
                if (validarChar("Ingrese el nuevo RIF: ", nuevoRif, 20)) {
                    apRifProveedor(cambios, nuevoRif);
                }
                break;
            }
            case 6:
                mostrarCambiosPendientesProveedor(cambios);
                break;
            case 7:
                aplicarCambiosProveedor(tienda, indice, cambios);
                opcion = 0;
                break;
            case 0:
                cout << "Actualización cancelada. No se guardaron cambios.\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
        
        if (opcion != 0 && opcion != 7) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
        
    } while(opcion != 0);
}

int contarProductosPorProveedor(Tienda* tienda, int idProveedor) {
    int count = 0;
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].idProveedor == idProveedor) {
            count++;
        }
    }
    return count;
}

void eliminarProveedor(Tienda* tienda) {
    if (tienda->numProveedores == 0) {
        cout << "No hay proveedores para eliminar." << endl;
        return;
    }
    
    int id;
    cout << "Ingrese el ID del proveedor a eliminar: ";
    cin >> id;
    cin.ignore(10000, '\n');
    
    int indice = buscarIndiceProveedorPorId(tienda, id);
    if (indice == -1) {
        cout << "Proveedor con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nProveedor a eliminar:\n";
    Proveedor& p = tienda->proveedores[indice];
    cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", RIF: " << p.rif << endl;
    
    int productosAsociados = contarProductosPorProveedor(tienda, id);
    if (productosAsociados > 0) {
        cout << "ADVERTENCIA: Este proveedor tiene " << productosAsociados 
             << " producto(s) asociado(s).\n";
        cout << "¿Desea eliminar el proveedor de todas formas? (s/n): ";
        char confirmacion;
        cin >> confirmacion;
        cin.ignore(10000, '\n');
        
        if (confirmacion != 's' && confirmacion != 'S') {
            cout << "Eliminación cancelada.\n";
            return;
        }
    }
    
    cout << "¿Está seguro que desea eliminar este proveedor? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    cin.ignore(10000, '\n');
    
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Eliminación cancelada.\n";
        return;
    }
    
    for (int i = indice; i < tienda->numProveedores - 1; i++) {
        tienda->proveedores[i] = tienda->proveedores[i + 1];
    }
    tienda->numProveedores--;
    cout << "Proveedor con ID " << id << " eliminado exitosamente.\n";
}

// ==================== FUNCIONES DE MENÚ ====================

void menuGestionProductos(Tienda* tienda) {
    int opcion;
    do {
        cout << "\n=== GESTIÓN DE PRODUCTOS ===\n";
        cout << "1. Crear nuevo producto\n";
        cout << "2. Listar productos\n";
        cout << "3. Buscar producto\n";
        cout << "4. Actualizar producto\n";
        cout << "5. Eliminar producto\n";
        cout << "0. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
        
        switch(opcion) {
            case 1:
                crearProducto(tienda);
                break;
            case 2:
                listarProductos(tienda);
                break;
            case 3:
                buscarProducto(tienda);
                break;
            case 4:
                actualizarProducto(tienda);
                break;
            case 5:
                eliminarProducto(tienda);
                break;
            case 0:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
        
    } while(opcion != 0);
}

void menuGestionProveedores(Tienda* tienda) {
    int opcion;
    do {
        cout << "\n=== GESTIÓN DE PROVEEDORES ===\n";
        cout << "1. Crear nuevo proveedor\n";
        cout << "2. Listar proveedores\n";
        cout << "3. Buscar proveedor\n";
        cout << "4. Actualizar proveedor\n";
        cout << "5. Eliminar proveedor\n";
        cout << "0. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
        
        switch(opcion) {
            case 1:
                crearProveedor(tienda);
                break;
            case 2:
                listarProveedores(tienda);
                break;
            case 3:
                buscarProveedor(tienda);
                break;
            case 4:
                actualizarProveedor(tienda);
                break;
            case 5:
                eliminarProveedor(tienda);
                break;
            case 0:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
        
    } while(opcion != 0);
}

void menuPrincipal(Tienda* tienda) {
    int opcion;
    do {
        cout << "\n=== MENÚ PRINCIPAL ===\n";
        cout << "1. Gestión de productos\n";
        cout << "2. Gestión de proveedores\n";
        cout << "3. Gestión de clientes\n";
        cout << "4. Gestión de transacciones\n";
        cout << "0. Salir\n\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
        
        switch(opcion) {
            case 1:
                menuGestionProductos(tienda);
                break;
            case 2:
                menuGestionProveedores(tienda);
                break;
            case 3:
                cout << "Gestión de clientes aún no implementada.\n";
                break;
            case 4:
                cout << "Gestión de transacciones aún no implementada.\n";
                break;
            case 0:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opción no válida. Por favor intente nuevamente.\n";
        }
    } while(opcion != 0);
}

int main() {
    setlocale(LC_ALL, "");
    Tienda miTienda;
    inicializarTienda(&miTienda, "Garibaldi", "J-67676767");
    
    menuPrincipal(&miTienda);
    
    liberarTienda(&miTienda);
    return 0;
}