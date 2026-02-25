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
    char identificacion[20];
    char fechaRegistro[20];
};

struct Cliente {
    int id;
    char nombre[100];
    char direccion[200];
    char telefono[20];
    char email[100];
    char identificacion[20];
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
    bool identificacionModificada;
    char nuevaIdentificacion[20];
};

struct CambiosCliente {
    bool nombreModificado;
    char nuevoNombre[100];
    bool direccionModificada;
    char nuevaDireccion[200];
    bool telefonoModificado;
    char nuevoTelefono[20];
    bool emailModificado;
    char nuevoEmail[100];
    bool identificacionModificada;
    char nuevaIdentificacion[20];
};

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
    cambios.identificacionModificada = false;
}

void inicializarCambiosCliente(CambiosCliente& cambios) {
    cambios.nombreModificado = false;
    cambios.direccionModificada = false;
    cambios.telefonoModificado = false;
    cambios.emailModificado = false;
    cambios.identificacionModificada = false;
}

void inicializarTienda(Tienda* tienda, const char* nombre = "Mi Tienda", const char* rif ="J-12345678-9"){
    limpiarPantalla();
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
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ERROR: Entrada demasiado larga." << endl;
        return false;
    }
    if (strlen(destino) == 0) {
        cout << "ERROR: El campo no puede estar vacio." << endl;
        return false;
    } else if (strcmp(destino, "cancelar") == 0) {
        cout << "Operacion cancelada por el usuario." << endl;
        return false;
    } else {
        return true;
    }
}

bool validarFloat(const char* mensaje, float& destino) {
    cout << mensaje;
    string input;
    cin >> input;
    
    if (cin.fail()) {
        cin.clear();
        limpiarBuffer();
        cout << "ERROR: Entrada invalida." << endl;
        return false;
    }
    
    if (input.empty()) {
        cout << "ERROR: El campo no puede estar vacio." << endl;
        return false;
    }
    if (input == "cancelar") {
        cout << "Operacion cancelada por el usuario." << endl;
        return false;
    }
    try {
        destino = stof(input);
        if (destino < 0) {
            cout << "ERROR: El valor no puede ser negativo." << endl;
            return false;
        }
        return true;
    } catch (const invalid_argument&) {
        cout << "ERROR: Entrada no valida. Por favor ingrese un numero valido." << endl;
        return false;
    }
}

bool validarInt(const char* mensaje, int& destino, bool permitirNegativos = false) {
    cout << mensaje;
    string input;
    cin >> input;
    
    if (cin.fail()) {
        cin.clear();
        limpiarBuffer();
        cout << "ERROR: Entrada invalida." << endl;
        return false;
    }
    
    if (input.empty()) {
        cout << "ERROR: El campo no puede estar vacio." << endl;
        return false;
    }
    if (input == "cancelar") {
        cout << "Operacion cancelada por el usuario." << endl;
        return false;
    }
    try {
        destino = stoi(input);
        if (!permitirNegativos && destino < 0) {
            cout << "ERROR: El valor no puede ser negativo." << endl;
            return false;
        }
        return true;
    } catch (const invalid_argument&) {
        cout << "ERROR: Entrada no valida. Por favor ingrese un numero valido." << endl;
        return false;
    }
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10 || fecha[4] != '-' || fecha[7] != '-') {
        cout << "ERROR: El formato de fecha debe ser YYYY-MM-DD." << endl;
        return false;
    }
    int anio = atoi(string(fecha, 4).c_str());
    int mes = atoi(string(fecha + 5, 2).c_str());
    int dia = atoi(string(fecha + 8, 2).c_str());
    
    if (anio < 1900 || anio > 2100) {
        cout << "ERROR: El ano debe estar entre 1900 y 2100." << endl;
        return false;
    }
    if (mes < 1 || mes > 12) {
        cout << "ERROR: El mes debe estar entre 1 y 12." << endl;
        return false;
    }
    if (dia < 1 || dia > 31) {
        cout << "ERROR: El dia debe estar entre 1 y 31." << endl;
        return false;
    }
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        cout << "ERROR: El mes seleccionado tiene solo 30 dias." << endl;
        return false;
    }
    return true;
}

bool validarCodigoUnico(Tienda* tienda, const char* codigo) {
    for (int i = 0; i < tienda->numProductos; i++) {
        if (strcmp(tienda->productos[i].codigo, codigo) == 0) {
            cout << "ERROR: El codigo '" << codigo << "' ya existe para otro producto." << endl;
            return false;
        }
    }
    return true;
}

bool validarIdentificacionUnica(Tienda* tienda, const char* identificacion, const char* tipo, int idExcluir = -1) {
    if (strlen(identificacion) == 0) {
        cout << "ERROR: La identificacion no puede estar vacia." << endl;
        return false;
    }
    
    if (strcmp(tipo, "proveedor") == 0) {
        for (int i = 0; i < tienda->numProveedores; i++) {
            if (tienda->proveedores[i].id != idExcluir) {
                char idActual[20], idNuevo[20];
                convertirAminusculas(idActual, tienda->proveedores[i].identificacion);
                convertirAminusculas(idNuevo, identificacion);
                
                if (strcmp(idActual, idNuevo) == 0) {
                    cout << "ERROR: La identificacion '" << identificacion << "' ya existe para otro proveedor." << endl;
                    return false;
                }
            }
        }
    } else if (strcmp(tipo, "cliente") == 0) {
        for (int i = 0; i < tienda->numClientes; i++) {
            if (tienda->clientes[i].id != idExcluir) {
                char idActual[20], idNuevo[20];
                convertirAminusculas(idActual, tienda->clientes[i].identificacion);
                convertirAminusculas(idNuevo, identificacion);
                
                if (strcmp(idActual, idNuevo) == 0) {
                    cout << "ERROR: La identificacion '" << identificacion << "' ya existe para otro cliente." << endl;
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool validarFormatoIdentificacion(const char* identificacion, const char* tipo) {
    if (strlen(identificacion) < 5) {
        cout << "ERROR: La identificacion es demasiado corta." << endl;
        return false;
    }
    
    if (strcmp(tipo, "proveedor") == 0) {
        if (identificacion[1] != '-') {
            cout << "ERROR: Formato de RIF invalido. Debe ser tipo: J-12345678-9" << endl;
            return false;
        }
    } else if (strcmp(tipo, "cliente") == 0) {
        if (identificacion[1] != '-') {
            cout << "ERROR: Formato de cedula invalido. Debe ser: V-12345678 o E-12345678" << endl;
            return false;
        }
    }
    
    return true;
}

bool validarEmail(const char* email) {
    const char* atPos = strchr(email, '@');
    if (atPos == nullptr || atPos == email || atPos == email + strlen(email) - 1) {
        cout << "ERROR: El correo electronico debe contener un '@' valido." << endl;
        return false;
    }
    const char* dotPos = strrchr(atPos, '.');
    if (dotPos == nullptr || dotPos == atPos + 1 || dotPos == email + strlen(email) - 1) {
        cout << "ERROR: El correo electronico debe contener un '.' despues del '@'." << endl;
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

void apCodigo(CambiosProducto& cambios, const char* nuevoCodigo) {
    strcpy(cambios.nuevoCodigo, nuevoCodigo);
    cambios.codigoModificado = true;
    cout << "Cambio en codigo pendiente de guardar.\n";
}

void apNombre(CambiosProducto& cambios, const char* nuevoNombre) {
    strcpy(cambios.nuevoNombre, nuevoNombre);
    cambios.nombreModificado = true;
    cout << "Cambio en nombre pendiente de guardar.\n";
}

void apDescripcion(CambiosProducto& cambios, const char* nuevaDescripcion) {
    strcpy(cambios.nuevaDescripcion, nuevaDescripcion);
    cambios.descripcionModificada = true;
    cout << "Cambio en descripcion pendiente de guardar.\n";
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
    if (cambios.codigoModificado) cout << "- Codigo: " << cambios.nuevoCodigo << endl;
    if (cambios.nombreModificado) cout << "- Nombre: " << cambios.nuevoNombre << endl;
    if (cambios.descripcionModificada) cout << "- Descripcion: " << cambios.nuevaDescripcion << endl;
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
        cout << "ERROR: indice de producto invalido.\n";
        return;
    }
    
    bool cambiosAplicados = false;
    
    if (cambios.codigoModificado) {
        if (validarCodigoUnico(tienda, cambios.nuevoCodigo)) {
            strcpy(tienda->productos[indice].codigo, cambios.nuevoCodigo);
            cambiosAplicados = true;
        } else {
            cout << "No se pudo actualizar el codigo: ya existe.\n";
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

void listarProveedores(Tienda* tienda) {
    if (tienda->numProveedores == 0) {
        cout << "No hay proveedores registrados." << endl;
        return;
    }
    
    cout << "\n=== LISTADO DE PROVEEDORES ===\n";
    cout << "ID\tNombre\t\t\tIdentificacion\tTelefono\tEmail\n";
    cout << "--------------------------------------------------------------------------------\n";
    
    for (int i = 0; i < tienda->numProveedores; i++) {
        Proveedor& p = tienda->proveedores[i];
        cout << p.id << "\t" << p.nombre << "\t" << p.identificacion << "\t" << p.telefono << "\t" << p.email << endl;
    }
    cout << "--------------------------------------------------------------------------------\n";
    cout << "Total de proveedores: " << tienda->numProveedores << endl;
}

void crearProducto(Tienda* tienda) {
    limpiarPantalla();
    Producto nuevoProducto;
    cout << "Desea agregar un nuevo producto? (s/n): ";
    char respuesta;
    cin >> respuesta;
    limpiarBuffer();
    
    if (respuesta != 's' && respuesta != 'S') {
        cout << "Creacion de producto cancelada." << endl;
        return;
    }
    if (!validarChar("Ingrese el codigo del producto (o 'cancelar' para cancelar): ", nuevoProducto.codigo, 20)) {
        return;
    }
    if (!validarCodigoUnico(tienda, nuevoProducto.codigo)) {
        return;
    }
    if (!validarChar("Ingrese el nombre del producto (o 'cancelar' para cancelar): ", nuevoProducto.nombre, 100)) {
        return;
    }
    if (!validarChar("Ingrese la descripcion del producto (o 'cancelar' para cancelar): ", nuevoProducto.descripcion, 200)) {
        return;
    }
    
    do {
        if (!validarInt("Ingrese el ID del proveedor (o 'cancelar' para cancelar): ", nuevoProducto.idProveedor)) {
            return;
        }
        
        int indiceProveedor = buscarIndiceProveedorPorId(tienda, nuevoProducto.idProveedor);
        if (indiceProveedor == -1) {
            cout << "ERROR: No existe un proveedor con ID " << nuevoProducto.idProveedor << ". Intente nuevamente.\n";
            cout << "Proveedores disponibles:\n";
            listarProveedores(tienda);
            cout << "\n";
        } else {
            break;
        }
    } while (true);
    
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
    cout << "Codigo: " << nuevoProducto.codigo << endl;
    cout << "Nombre: " << nuevoProducto.nombre << endl;
    cout << "Descripcion: " << nuevoProducto.descripcion << endl;
    
    int indiceProveedor = buscarIndiceProveedorPorId(tienda, nuevoProducto.idProveedor);
    cout << "Proveedor: " << tienda->proveedores[indiceProveedor].nombre << " (ID: " << nuevoProducto.idProveedor << ")" << endl;
    
    cout << "Precio: " << nuevoProducto.precio << "$" << endl;
    cout << "Stock: " << nuevoProducto.stock << endl;
    cout << "Fecha de Registro: " << nuevoProducto.fechaRegistro << endl;
    cout << "-----------------------------" << endl;
    cout << "\n Desea confirmar la creacion de este producto? (s/n): "; 
    char confirmacion;
    cin >> confirmacion;
    limpiarBuffer();
    
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Creacion de producto cancelada." << endl;
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
    cout << "--------------------------------------------------------------------------------\n";
    cout << "Lista de productos en la tienda:" << endl;
    for (int i = 0; i < tienda->numProductos; i++) {
        Producto& p = tienda->productos[i];
        cout << "ID: " << p.id << ", Codigo: " << p.codigo << ", Nombre: " << p.nombre 
             << ", Precio: " << p.precio << "$, Stock: " << p.stock 
             << ", Fecha Registro: " << p.fechaRegistro << endl;
    }
    cout << "Total de productos: " << tienda->numProductos << endl;
    cout << "--------------------------------------------------------------------------------\n";
}

void eliminarProducto(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "No hay productos para eliminar." << endl;
        return;
    }
    
    int id;
    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> id;
    limpiarBuffer();
    
    int indice = buscarIndiceProductoPorId(tienda, id);
    if (indice == -1) {
        cout << "Producto con ID " << id << " no encontrado. No se puede eliminar.\n";
        return;
    }
    
    cout << "\nProducto a eliminar:\n";
    Producto& p = tienda->productos[indice];
    cout << "ID: " << p.id << ", Codigo: " << p.codigo << ", Nombre: " << p.nombre 
         << ", Precio: " << p.precio << "$, Stock: " << p.stock << endl;
    
    cout << "Esta seguro que desea eliminar este producto? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    limpiarBuffer();
    
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Eliminacion cancelada.\n";
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
        cout << "Producto encontrado: ID: " << p.id << ", Codigo: " << p.codigo 
             << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
             << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
    } else {
        cout << "Producto con ID " << id << " no encontrado.\n";
    }
}

void buscarProductoPorNombre(Tienda* tienda, const char* nombre) {
    int encontrados = 0;
    cout << "--------------------------------------------------------------------------------------\n";
    cout << "RESULTADOS DE BUSQUEDA POR NOMBRE '" << nombre << "':\n";
    for (int i = 0; i < tienda->numProductos; i++) {
        if (contieneSubstring(tienda->productos[i].nombre, nombre)) {
            Producto& p = tienda->productos[i];
            cout << "ID: " << p.id << ", Codigo: " << p.codigo 
                 << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
                 << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
            encontrados++;
        }
    }
    cout << "--------------------------------------------------------------------------------------\n";
    if (encontrados == 0) {
        cout << "No se encontraron productos con nombre que contenga '" << nombre << "'.\n";
    } else {
        cout << "Total de productos encontrados: " << encontrados << endl;
    }
}

void buscarProductoPorCodigo(Tienda* tienda, const char* codigo) {
    int encontrados = 0;
    cout << "Resultados de busqueda por codigo '" << codigo << "':\n";
    for (int i = 0; i < tienda->numProductos; i++) {
        if (contieneSubstring(tienda->productos[i].codigo, codigo)) {
            Producto& p = tienda->productos[i];
            cout << "ID: " << p.id << ", Codigo: " << p.codigo 
                 << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
                 << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
            encontrados++;
        }
    }
    if (encontrados == 0) {
        cout << "No se encontraron productos con codigo que contenga '" << codigo << "'.\n";
    } else {
        cout << "Total de productos encontrados: " << encontrados << endl;
    }
}

void buscarProductoPorProveedor(Tienda* tienda, int idProveedor) {
    int encontrados = 0;
    cout << "--------------------------------------------------------------------------------\n";
    cout << "RESULTADOS DE BUSQUEDA POR ID DE PROVEEDOR '" << idProveedor << "':\n";
    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].idProveedor == idProveedor) {
            Producto& p = tienda->productos[i];
            cout << "ID: " << p.id << ", Codigo: " << p.codigo 
                 << ", Nombre: " << p.nombre << ", Precio: " << p.precio 
                 << "$, Stock: " << p.stock << ", Fecha Registro: " << p.fechaRegistro << endl;
            encontrados++;
        }
    }
    cout << "--------------------------------------------------------------------------------\n";
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
        cout << "3. Buscar por codigo (coincidencia parcial)\n";
        cout << "4. Listar por proveedor\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1: {
                cout << "Ingrese ID del producto: ";
                int id;
                cin >> id;
                limpiarBuffer();
                buscarProductoPorId(tienda, id);
                break;
            }
            case 2:
                cout << "Ingrese nombre a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProductoPorNombre(tienda, busqueda);
                break;
            case 3:
                cout << "Ingrese codigo a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProductoPorCodigo(tienda, busqueda);
                break;
            case 4:
                cout << "Ingrese ID del proveedor: ";
                cin >> idProveedor;
                limpiarBuffer();
                buscarProductoPorProveedor(tienda, idProveedor);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while(opcion != 0);
}

void actualizarProducto(Tienda* tienda) {
    limpiarPantalla();
    if (tienda->numProductos == 0) {
        cout << "No hay productos registrados para actualizar.\n";
        return;
    }
    
    int id;
    cout << "Ingrese el ID del producto que desea actualizar: ";
    cin >> id;
    limpiarBuffer();
    
    int indice = buscarIndiceProductoPorId(tienda, id);
    if (indice == -1) {
        cout << "Producto con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nProducto seleccionado:\n";
    Producto& p = tienda->productos[indice];
    cout << "ID: " << p.id << ", Codigo: " << p.codigo << ", Nombre: " << p.nombre 
         << ", Precio: " << p.precio << "$, Stock: " << p.stock << endl;
    
    CambiosProducto cambios;
    inicializarCambios(cambios);
    
    int opcion;
    do {
        cout << "\n=== ACTUALIZAR PRODUCTO ===\n";
        cout << "1. Editar codigo\n";
        cout << "2. Editar nombre\n";
        cout << "3. Editar descripcion\n";
        cout << "4. Editar proveedor\n";
        cout << "5. Editar precio\n";
        cout << "6. Editar stock (reemplazar)\n";
        cout << "7. Ajustar stock (sumar/restar)\n";
        cout << "8. Ver cambios pendientes\n";
        cout << "9. Guardar todos los cambios\n";
        cout << "0. Cancelar y salir sin guardar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1: {
                char nuevoCodigo[20];
                if (validarChar("Ingrese el nuevo codigo: ", nuevoCodigo, 20)) {
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
                if (validarChar("Ingrese la nueva descripcion: ", nuevaDescripcion, 200)) {
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
                cout << "Actualizacion cancelada. No se guardaron cambios.\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        
        if (opcion != 0 && opcion != 9) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

void buscarProveedorPorId(Tienda* tienda, int id) {
    int indice = buscarIndiceProveedorPorId(tienda, id);
    if (indice != -1) {
        Proveedor& p = tienda->proveedores[indice];
        cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
        cout << "ID: " << p.id << endl;
        cout << "Nombre: " << p.nombre << endl;
        cout << "Identificacion: " << p.identificacion << endl;
        cout << "Telefono: " << p.telefono << endl;
        cout << "Email: " << p.email << endl;
        cout << "Direccion: " << p.direccion << endl;
        cout << "Fecha de Registro: " << p.fechaRegistro << endl;
    } else {
        cout << "Proveedor con ID " << id << " no encontrado.\n";
    }
}

void buscarProveedorPorNombre(Tienda* tienda, const char* nombre) {
    int encontrados = 0;
    cout << "\n=== RESULTADOS DE BUSQUEDA POR NOMBRE: '" << nombre << "' ===\n";
    
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (contieneSubstring(tienda->proveedores[i].nombre, nombre)) {
            Proveedor& p = tienda->proveedores[i];
            cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", Identificacion: " << p.identificacion 
                 << ", Telefono: " << p.telefono << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron proveedores con nombre que contenga '" << nombre << "'.\n";
    } else {
        cout << "Total de proveedores encontrados: " << encontrados << endl;
    }
}

void buscarProveedorPorIdentificacion(Tienda* tienda, const char* identificacion) {
    int encontrados = 0;
    cout << "\n=== RESULTADOS DE BUSQUEDA POR IDENTIFICACION: '" << identificacion << "' ===\n";
    
    for (int i = 0; i < tienda->numProveedores; i++) {
        if (contieneSubstring(tienda->proveedores[i].identificacion, identificacion)) {
            Proveedor& p = tienda->proveedores[i];
            cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", Identificacion: " << p.identificacion 
                 << ", Telefono: " << p.telefono << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron proveedores con identificacion que contenga '" << identificacion << "'.\n";
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
        cout << "3. Buscar por identificacion (coincidencia parcial)\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1: {
                int id;
                cout << "Ingrese ID del proveedor: ";
                cin >> id;
                limpiarBuffer();
                buscarProveedorPorId(tienda, id);
                break;
            }
            case 2:
                cout << "Ingrese nombre a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProveedorPorNombre(tienda, busqueda);
                break;
            case 3:
                cout << "Ingrese identificacion a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarProveedorPorIdentificacion(tienda, busqueda);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

void actualizarNombreProveedor(CambiosProveedor& cambios, const char* nuevoNombre) {
    strcpy(cambios.nuevoNombre, nuevoNombre);
    cambios.nombreModificado = true;
    cout << "Cambio en nombre pendiente de guardar.\n";
}

void actualizarDireccionProveedor(CambiosProveedor& cambios, const char* nuevaDireccion) {
    strcpy(cambios.nuevaDireccion, nuevaDireccion);
    cambios.direccionModificada = true;
    cout << "Cambio en direccion pendiente de guardar.\n";
}

void actualizarTelefProveedor(CambiosProveedor& cambios, const char* nuevoTelefono) {
    strcpy(cambios.nuevoTelefono, nuevoTelefono);
    cambios.telefonoModificado = true;
    cout << "Cambio en telefono pendiente de guardar.\n";
}

void actualizarEmailProveedor(CambiosProveedor& cambios, const char* nuevoEmail) {
    strcpy(cambios.nuevoEmail, nuevoEmail);
    cambios.emailModificado = true;
    cout << "Cambio en email pendiente de guardar.\n";
}

void actualizarIdProveedor(CambiosProveedor& cambios, const char* nuevaIdentificacion) {
    strcpy(cambios.nuevaIdentificacion, nuevaIdentificacion);
    cambios.identificacionModificada = true;
    cout << "Cambio en identificacion pendiente de guardar.\n";
}

void mostrarCambiosPendientesProveedor(const CambiosProveedor& cambios) {
    cout << "\n=== CAMBIOS PENDIENTES ===\n";
    if (cambios.nombreModificado) cout << "- Nombre: " << cambios.nuevoNombre << endl;
    if (cambios.direccionModificada) cout << "- Direccion: " << cambios.nuevaDireccion << endl;
    if (cambios.telefonoModificado) cout << "- Telefono: " << cambios.nuevoTelefono << endl;
    if (cambios.emailModificado) cout << "- Email: " << cambios.nuevoEmail << endl;
    if (cambios.identificacionModificada) cout << "- Identificacion: " << cambios.nuevaIdentificacion << endl;
    
    if (!cambios.nombreModificado && !cambios.direccionModificada && 
        !cambios.telefonoModificado && !cambios.emailModificado && !cambios.identificacionModificada) {
        cout << "No hay cambios pendientes.\n";
    }
}

void aplicarCambiosProveedor(Tienda* tienda, int indice, const CambiosProveedor& cambios) {
    if (indice < 0 || indice >= tienda->numProveedores) {
        cout << "ERROR: indice de proveedor invalido.\n";
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
            cout << "No se pudo actualizar el email: formato invalido.\n";
        }
    }
    
    if (cambios.identificacionModificada) {
        if (validarIdentificacionUnica(tienda, cambios.nuevaIdentificacion, "proveedor", tienda->proveedores[indice].id)) {
            strcpy(tienda->proveedores[indice].identificacion, cambios.nuevaIdentificacion);
            cambiosAplicados = true;
        } else {
            cout << "No se pudo actualizar la identificacion: ya existe.\n";
        }
    }
    
    if (cambiosAplicados) {
        cout << "Cambios guardados exitosamente.\n";
    } else {
        cout << "No se aplicaron cambios.\n";
    }
}

void crearProveedor(Tienda* tienda) {
    limpiarPantalla();
    cout << "Desea registrar un nuevo proveedor? (s/n): ";
    char respuesta;
    cin >> respuesta;
    limpiarBuffer();
    
    if (respuesta != 's' && respuesta != 'S') {
        cout << "Creacion de proveedor cancelada." << endl;
        return;
    }
    
    Proveedor nuevoProveedor;
    
    cout << "\n=== REGISTRO DE NUEVO PROVEEDOR ===\n";
    
    if (!validarChar("Ingrese el nombre del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.nombre, 100)) {
        return;
    }
    
    cout << "Ingrese la identificacion del proveedor (formato: J-12345678-9): ";
    cin.getline(nuevoProveedor.identificacion, 20);
    
    if (strlen(nuevoProveedor.identificacion) == 0) {
        cout << "ERROR: La identificacion no puede estar vacia." << endl;
        return;
    }
    if (strcmp(nuevoProveedor.identificacion, "cancelar") == 0) {
        cout << "Operacion cancelada por el usuario." << endl;
        return;
    }
    
    if (!validarFormatoIdentificacion(nuevoProveedor.identificacion, "proveedor")) {
        return;
    }
    
    if (!validarIdentificacionUnica(tienda, nuevoProveedor.identificacion, "proveedor")) {
        return;
    }
    
    if (!validarChar("Ingrese el telefono del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.telefono, 20)) {
        return;
    }
    
    if (!validarChar("Ingrese el email del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.email, 100)) {
        return;
    }
    
    if (!validarEmail(nuevoProveedor.email)) {
        return;
    }
    
    if (!validarChar("Ingrese la direccion del proveedor (o 'cancelar' para cancelar): ", nuevoProveedor.direccion, 200)) {
        return;
    }
    
    tomarFechaActual(nuevoProveedor.fechaRegistro, sizeof(nuevoProveedor.fechaRegistro));
    nuevoProveedor.id = tienda->siguienteIdProveedor;
    
    cout << "\n-----------------------------\n";
    cout << "Resumen del nuevo proveedor:\n";
    cout << "ID: " << nuevoProveedor.id << endl;
    cout << "Nombre: " << nuevoProveedor.nombre << endl;
    cout << "Identificacion: " << nuevoProveedor.identificacion << endl;
    cout << "Telefono: " << nuevoProveedor.telefono << endl;
    cout << "Email: " << nuevoProveedor.email << endl;
    cout << "Direccion: " << nuevoProveedor.direccion << endl;
    cout << "Fecha de Registro: " << nuevoProveedor.fechaRegistro << endl;
    cout << "-----------------------------\n";
    
    cout << "\nDesea confirmar la creacion de este proveedor? (s/n): ";
    char linea[10];
    cin.getline(linea, 10);

    if (tolower(linea[0]) != 's') {
        cout << "Creacion de proveedor cancelada." << endl;
        return;
    }

    if (tienda->numProveedores >= tienda->capacidadProveedores) {
        redimensionarArrayProveedores(tienda);
    }

    tienda->proveedores[tienda->numProveedores] = nuevoProveedor;
    tienda->numProveedores++;
    tienda->siguienteIdProveedor++;

    cout << "Proveedor creado exitosamente con ID: " << nuevoProveedor.id << endl;
}

void actualizarProveedor(Tienda* tienda) {
    limpiarPantalla();
    if (tienda->numProveedores == 0) {
        cout << "No hay proveedores registrados para actualizar.\n";
        return;
    }
    
    int id;
    cout << "Ingrese el ID del proveedor que desea actualizar: ";
    cin >> id;
    limpiarBuffer();
    
    int indice = buscarIndiceProveedorPorId(tienda, id);
    if (indice == -1) {
        cout << "Proveedor con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nProveedor seleccionado:\n";
    Proveedor& p = tienda->proveedores[indice];
    cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", Identificacion: " << p.identificacion 
         << ", Telefono: " << p.telefono << ", Email: " << p.email << endl;
    
    CambiosProveedor cambios;
    inicializarCambiosProveedor(cambios);
    
    int opcion;
    do {
        cout << "\n=== ACTUALIZAR PROVEEDOR ===\n";
        cout << "1. Editar nombre\n";
        cout << "2. Editar direccion\n";
        cout << "3. Editar telefono\n";
        cout << "4. Editar email\n";
        cout << "5. Editar identificacion\n";
        cout << "6. Ver cambios pendientes\n";
        cout << "7. Guardar todos los cambios\n";
        cout << "0. Cancelar y salir sin guardar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1: {
                char nuevoNombre[100];
                if (validarChar("Ingrese el nuevo nombre: ", nuevoNombre, 100)) {
                    actualizarNombreProveedor(cambios, nuevoNombre);
                }
                break;
            }
            case 2: {
                char nuevaDireccion[200];
                if (validarChar("Ingrese la nueva direccion: ", nuevaDireccion, 200)) {
                    actualizarDireccionProveedor(cambios, nuevaDireccion);
                }
                break;
            }
            case 3: {
                char nuevoTelefono[20];
                if (validarChar("Ingrese el nuevo telefono: ", nuevoTelefono, 20)) {
                    actualizarTelefProveedor(cambios, nuevoTelefono);
                }
                break;
            }
            case 4: {
                char nuevoEmail[100];
                if (validarChar("Ingrese el nuevo email: ", nuevoEmail, 100)) {
                    if (validarEmail(nuevoEmail)) {
                        actualizarEmailProveedor(cambios, nuevoEmail);
                    }
                }
                break;
            }
            case 5: {
                char nuevaIdentificacion[20];
                cout << "Ingrese la nueva identificacion (formato: J-12345678-9): ";
                cin.getline(nuevaIdentificacion, 20);
                if (strlen(nuevaIdentificacion) > 0) {
                    if (validarFormatoIdentificacion(nuevaIdentificacion, "proveedor")) {
                        actualizarIdProveedor(cambios, nuevaIdentificacion);
                    }
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
                cout << "Actualizacion cancelada. No se guardaron cambios.\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        
        if (opcion != 0 && opcion != 7) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    limpiarBuffer();
    
    int indice = buscarIndiceProveedorPorId(tienda, id);
    if (indice == -1) {
        cout << "Proveedor con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nProveedor a eliminar:\n";
    Proveedor& p = tienda->proveedores[indice];
    cout << "ID: " << p.id << ", Nombre: " << p.nombre << ", Identificacion: " << p.identificacion << endl;
    
    int productosAsociados = contarProductosPorProveedor(tienda, id);
    if (productosAsociados > 0) {
        cout << "ADVERTENCIA: Este proveedor tiene " << productosAsociados << " producto(s) asociado(s).\n";
        cout << "Desea eliminar el proveedor de todas formas? (s/n): ";
        char confirmacion;
        cin >> confirmacion;
        limpiarBuffer();
        
        if (confirmacion != 's' && confirmacion != 'S') {
            cout << "Eliminacion cancelada.\n";
            return;
        }
    }
    
    cout << "Esta seguro que desea eliminar este proveedor? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    limpiarBuffer();
    
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Eliminacion cancelada.\n";
        return;
    }
    
    for (int i = indice; i < tienda->numProveedores - 1; i++) {
        tienda->proveedores[i] = tienda->proveedores[i + 1];
    }
    tienda->numProveedores--;
    cout << "Proveedor con ID " << id << " eliminado exitosamente.\n";
}

void crearCliente(Tienda* tienda) {
    limpiarPantalla();
    cout << "Desea registrar un nuevo cliente? (s/n): ";
    char respuesta;
    cin >> respuesta;
    limpiarBuffer();
    
    if (respuesta != 's' && respuesta != 'S') {
        cout << "Creacion de cliente cancelada." << endl;
        return;
    }
    
    Cliente nuevoCliente;
    
    cout << "\n=== REGISTRO DE NUEVO CLIENTE ===\n";
    
    if (!validarChar("Ingrese el nombre del Cliente (o 'cancelar' para cancelar): ", nuevoCliente.nombre, 100)) {
        return;
    }

    cout << "Ingrese la cedula del cliente (formato: V-12345678 o J-12345678): ";
    cin.getline(nuevoCliente.identificacion, 20);
    
    if (strlen(nuevoCliente.identificacion) == 0) {
        cout << "ERROR: La identificacion no puede estar vacia." << endl;
        return;
    }
    if (strcmp(nuevoCliente.identificacion, "cancelar") == 0) {
        cout << "Operacion cancelada por el usuario." << endl;
        return;
    }
    
    if (!validarFormatoIdentificacion(nuevoCliente.identificacion, "cliente")) {
        return;
    }
    
    if (!validarIdentificacionUnica(tienda, nuevoCliente.identificacion, "cliente")) {
        return;
    }
    
    if (!validarChar("Ingrese el telefono del Cliente (o 'cancelar'): ", nuevoCliente.telefono, 20)) {
        return;
    }
    
    if (!validarChar("Ingrese el email del Cliente (o 'cancelar'): ", nuevoCliente.email, 100)) {
        return;
    }
    
    if (!validarEmail(nuevoCliente.email)) {
        return;
    }
    
    if (!validarChar("Ingrese la direccion del Cliente (o 'cancelar'): ", nuevoCliente.direccion, 200)) {
        return;
    }

    tomarFechaActual(nuevoCliente.fechaRegistro, sizeof(nuevoCliente.fechaRegistro));
    nuevoCliente.id = tienda->siguienteIdCliente;
    
    cout << "\n-----------------------------\n";
    cout << "Resumen del nuevo cliente:\n";
    cout << "ID: " << nuevoCliente.id << endl;
    cout << "Nombre: " << nuevoCliente.nombre << endl;
    cout << "Identificacion: " << nuevoCliente.identificacion << endl;
    cout << "Telefono: " << nuevoCliente.telefono << endl;
    cout << "Email: " << nuevoCliente.email << endl;
    cout << "Direccion: " << nuevoCliente.direccion << endl;
    cout << "Fecha de Registro: " << nuevoCliente.fechaRegistro << endl;
    cout << "-----------------------------\n";
    
    cout << "\nDesea confirmar la creacion de este cliente? (s/n): ";
    char confirmacion[10];
    cin.getline(confirmacion, 10);
    
    if (tolower(confirmacion[0]) != 's') {
        cout << "Creacion de cliente cancelada." << endl;
        return;
    }
    
    if (tienda->numClientes >= tienda->capacidadClientes) {
        redimensionarArrayClientes(tienda);
    }
    
    tienda->clientes[tienda->numClientes] = nuevoCliente;
    tienda->numClientes++;
    tienda->siguienteIdCliente++;
    
    cout << "Cliente creado exitosamente con ID: " << nuevoCliente.id << endl;
}

void buscarClientePorId(Tienda* tienda, int id) {
    int indice = buscarIndiceClientePorId(tienda, id);
    if (indice != -1) {
        Cliente& c = tienda->clientes[indice];
        cout << "\n=== CLIENTE ENCONTRADO ===\n";
        cout << "ID: " << c.id << endl;
        cout << "Nombre: " << c.nombre << endl;
        cout << "Identificacion: " << c.identificacion << endl;
        cout << "Telefono: " << c.telefono << endl;
        cout << "Email: " << c.email << endl;
        cout << "Direccion: " << c.direccion << endl;
        cout << "Fecha de Registro: " << c.fechaRegistro << endl;
    } else {
        cout << "Cliente con ID " << id << " no encontrado.\n";
    }
}

void buscarClientePorNombre(Tienda* tienda, const char* nombre) {
    int encontrados = 0;
    cout << "\n=== RESULTADOS DE BUSQUEDA POR NOMBRE: '" << nombre << "' ===\n";
    
    for (int i = 0; i < tienda->numClientes; i++) {
        if (contieneSubstring(tienda->clientes[i].nombre, nombre)) {
            Cliente& c = tienda->clientes[i];
            cout << "ID: " << c.id << ", Nombre: " << c.nombre 
                 << ", Identificacion: " << c.identificacion 
                 << ", Telefono: " << c.telefono << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron clientes con nombre que contenga '" << nombre << "'.\n";
    } else {
        cout << "Total de clientes encontrados: " << encontrados << endl;
    }
}

void buscarClientePorIdentificacion(Tienda* tienda, const char* identificacion) {
    int encontrados = 0;
    cout << "\n=== RESULTADOS DE BUSQUEDA POR IDENTIFICACION: '" << identificacion << "' ===\n";
    
    for (int i = 0; i < tienda->numClientes; i++) {
        if (contieneSubstring(tienda->clientes[i].identificacion, identificacion)) {
            Cliente& c = tienda->clientes[i];
            cout << "ID: " << c.id << ", Nombre: " << c.nombre 
                 << ", Identificacion: " << c.identificacion 
                 << ", Telefono: " << c.telefono << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron clientes con identificacion que contenga '" << identificacion << "'.\n";
    } else {
        cout << "Total de clientes encontrados: " << encontrados << endl;
    }
}

void buscarCliente(Tienda* tienda) {
    int opcion;
    char busqueda[100];
    
    do {
        cout << "\n=== BUSCAR CLIENTE ===\n";
        cout << "1. Buscar por ID (exacto)\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por identificacion (coincidencia parcial)\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1: {
                int id;
                cout << "Ingrese ID del cliente: ";
                cin >> id;
                limpiarBuffer();
                buscarClientePorId(tienda, id);
                break;
            }
            case 2:
                cout << "Ingrese nombre a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarClientePorNombre(tienda, busqueda);
                break;
            case 3:
                cout << "Ingrese identificacion a buscar (puede ser parcial): ";
                cin.getline(busqueda, 100);
                buscarClientePorIdentificacion(tienda, busqueda);
                break;
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

void listarClientes(Tienda* tienda) {
    if (tienda->numClientes == 0) {
        cout << "No hay clientes registrados." << endl;
        return;
    }
    
    cout << "\n=== LISTADO DE CLIENTES ===\n";
    cout << "ID\tNombre\tIdentificacion\tTelefono\tEmail\n";
    cout << "--------------------------------------------------------------------------------\n";
    
    for (int i = 0; i < tienda->numClientes; i++) {
        Cliente& c = tienda->clientes[i];
        cout << c.id << "\t" << c.nombre << "\t" << c.identificacion << "\t" 
             << c.telefono << "\t" << c.email << endl;
    }
    cout << "Total de clientes: " << tienda->numClientes << endl;
}

void actualizarNombreCliente(CambiosCliente& cambios, const char* nuevoNombre) {
    strcpy(cambios.nuevoNombre, nuevoNombre);
    cambios.nombreModificado = true;
    cout << "Cambio en nombre pendiente de guardar.\n";
}

void actualizarDireccionCliente(CambiosCliente& cambios, const char* nuevaDireccion) {
    strcpy(cambios.nuevaDireccion, nuevaDireccion);
    cambios.direccionModificada = true;
    cout << "Cambio en direccion pendiente de guardar.\n";
}

void actualizarTlfCliente(CambiosCliente& cambios, const char* nuevoTelefono) {
    strcpy(cambios.nuevoTelefono, nuevoTelefono);
    cambios.telefonoModificado = true;
    cout << "Cambio en telefono pendiente de guardar.\n";
}

void actualizarEmailCliente(CambiosCliente& cambios, const char* nuevoEmail) {
    strcpy(cambios.nuevoEmail, nuevoEmail);
    cambios.emailModificado = true;
    cout << "Cambio en email pendiente de guardar.\n";
}

void actualizarIdCliente(CambiosCliente& cambios, const char* nuevaIdentificacion) {
    strcpy(cambios.nuevaIdentificacion, nuevaIdentificacion);
    cambios.identificacionModificada = true;
    cout << "Cambio en identificacion pendiente de guardar.\n";
}

void mostrarCambiosPendientesCliente(const CambiosCliente& cambios) {
    cout << "\n=== CAMBIOS PENDIENTES ===\n";
    if (cambios.nombreModificado) cout << "- Nombre: " << cambios.nuevoNombre << endl;
    if (cambios.direccionModificada) cout << "- Direccion: " << cambios.nuevaDireccion << endl;
    if (cambios.telefonoModificado) cout << "- Telefono: " << cambios.nuevoTelefono << endl;
    if (cambios.emailModificado) cout << "- Email: " << cambios.nuevoEmail << endl;
    if (cambios.identificacionModificada) cout << "- Identificacion: " << cambios.nuevaIdentificacion << endl;
    
    if (!cambios.nombreModificado && !cambios.direccionModificada && 
        !cambios.telefonoModificado && !cambios.emailModificado && 
        !cambios.identificacionModificada) {
        cout << "No hay cambios pendientes.\n";
    }
}

void aplicarCambiosCliente(Tienda* tienda, int indice, const CambiosCliente& cambios) {
    if (indice < 0 || indice >= tienda->numClientes) {
        cout << "ERROR: indice de cliente invalido.\n";
        return;
    }
    
    bool cambiosAplicados = false;
    
    if (cambios.nombreModificado) {
        strcpy(tienda->clientes[indice].nombre, cambios.nuevoNombre);
        cambiosAplicados = true;
    }
    
    if (cambios.direccionModificada) {
        strcpy(tienda->clientes[indice].direccion, cambios.nuevaDireccion);
        cambiosAplicados = true;
    }
    
    if (cambios.telefonoModificado) {
        strcpy(tienda->clientes[indice].telefono, cambios.nuevoTelefono);
        cambiosAplicados = true;
    }
    
    if (cambios.emailModificado) {
        if (validarEmail(cambios.nuevoEmail)) {
            strcpy(tienda->clientes[indice].email, cambios.nuevoEmail);
            cambiosAplicados = true;
        } else {
            cout << "No se pudo actualizar el email: formato invalido.\n";
        }
    }
    
    if (cambios.identificacionModificada) {
        if (validarIdentificacionUnica(tienda, cambios.nuevaIdentificacion, "cliente", tienda->clientes[indice].id)) {
            strcpy(tienda->clientes[indice].identificacion, cambios.nuevaIdentificacion);
            cambiosAplicados = true;
        } else {
            cout << "No se pudo actualizar la identificacion: ya existe.\n";
        }
    }
    
    if (cambiosAplicados) {
        cout << "Cambios guardados exitosamente.\n";
    } else {
        cout << "No se aplicaron cambios.\n";
    }
}

void actualizarCliente(Tienda* tienda) {
    limpiarPantalla();
    if (tienda->numClientes == 0) {
        cout << "No hay clientes registrados para actualizar.\n";
        return;
    }
    
    int id;
    cout << "Ingrese el ID del cliente que desea actualizar: ";
    cin >> id;
    limpiarBuffer();
    
    int indice = buscarIndiceClientePorId(tienda, id);
    if (indice == -1) {
        cout << "Cliente con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nCliente seleccionado:\n";
    Cliente& c = tienda->clientes[indice];
    cout << "ID: " << c.id << ", Nombre: " << c.nombre 
         << ", Identificacion: " << c.identificacion 
         << ", Telefono: " << c.telefono 
         << ", Email: " << c.email << endl;
    
    CambiosCliente cambios;
    inicializarCambiosCliente(cambios);
    
    int opcion;
    do {
        cout << "\n=== ACTUALIZAR CLIENTE ===\n";
        cout << "1. Editar nombre\n";
        cout << "2. Editar direccion\n";
        cout << "3. Editar telefono\n";
        cout << "4. Editar email\n";
        cout << "5. Editar identificacion\n";
        cout << "6. Ver cambios pendientes\n";
        cout << "7. Guardar todos los cambios\n";
        cout << "0. Cancelar y salir sin guardar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1: {
                char nuevoNombre[100];
                if (validarChar("Ingrese el nuevo nombre: ", nuevoNombre, 100)) {
                    actualizarNombreCliente(cambios, nuevoNombre);
                }
                break;
            }
            case 2: {
                char nuevaDireccion[200];
                if (validarChar("Ingrese la nueva direccion: ", nuevaDireccion, 200)) {
                    actualizarDireccionCliente(cambios, nuevaDireccion);
                }
                break;
            }
            case 3: {
                char nuevoTelefono[20];
                if (validarChar("Ingrese el nuevo telefono: ", nuevoTelefono, 20)) {
                    actualizarTlfCliente(cambios, nuevoTelefono);
                }
                break;
            }
            case 4: {
                char nuevoEmail[100];
                if (validarChar("Ingrese el nuevo email: ", nuevoEmail, 100)) {
                    if (validarEmail(nuevoEmail)) {
                        actualizarEmailCliente(cambios, nuevoEmail);
                    }
                }
                break;
            }
            case 5: {
                char nuevaIdentificacion[20];
                cout << "Ingrese la nueva identificacion (formato: V-12345678 o E-12345678): ";
                cin.getline(nuevaIdentificacion, 20);
                if (strlen(nuevaIdentificacion) > 0) {
                    if (validarFormatoIdentificacion(nuevaIdentificacion, "cliente")) {
                        actualizarIdCliente(cambios, nuevaIdentificacion);
                    }
                }
                break;
            }
            case 6:
                mostrarCambiosPendientesCliente(cambios);
                break;
            case 7:
                aplicarCambiosCliente(tienda, indice, cambios);
                opcion = 0;
                break;
            case 0:
                cout << "Actualizacion cancelada. No se guardaron cambios.\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        
        if (opcion != 0 && opcion != 7) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

int contarTransaccionesPorCliente(Tienda* tienda, int idCliente) {
    int count = 0;
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (strcmp(tienda->transacciones[i].tipo, "VENTA") == 0 &&
            tienda->transacciones[i].idRelacionado == idCliente) {
            count++;
        }
    }
    return count;
}

void eliminarCliente(Tienda* tienda) {
    if (tienda->numClientes == 0) {
        cout << "No hay clientes para eliminar." << endl;
        return;
    }
    
    int id;
    cout << "Ingrese el ID del cliente a eliminar: ";
    cin >> id;
    limpiarBuffer();
    
    int indice = buscarIndiceClientePorId(tienda, id);
    if (indice == -1) {
        cout << "Cliente con ID " << id << " no encontrado.\n";
        return;
    }
    
    cout << "\nCliente a eliminar:\n";
    Cliente& c = tienda->clientes[indice];
    cout << "ID: " << c.id << ", Nombre: " << c.nombre 
         << ", Identificacion: " << c.identificacion << endl;
    
    int transaccionesAsociadas = contarTransaccionesPorCliente(tienda, id);
    if (transaccionesAsociadas > 0) {
        cout << "ADVERTENCIA: Este cliente tiene " << transaccionesAsociadas << " transaccion(es) asociada(s).\n";
    }
    
    cout << "Esta seguro que desea eliminar este cliente? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    limpiarBuffer();
    
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Eliminacion cancelada.\n";
        return;
    }
    
    for (int i = indice; i < tienda->numClientes - 1; i++) {
        tienda->clientes[i] = tienda->clientes[i + 1];
    }
    tienda->numClientes--;
    cout << "Cliente con ID " << id << " eliminado exitosamente.\n";
}

void registrarCompra(Tienda* tienda) {
    limpiarPantalla();
    if (tienda->numProveedores == 0) {
        cout << "ERROR: No hay proveedores registrados. Debe crear un proveedor primero.\n";
        return;
    }
    if (tienda->numProductos == 0) {
        cout << "ERROR: No hay productos registrados. Debe crear un producto primero.\n";
        return;
    }
    
    cout << "\n=== REGISTRAR COMPRA A PROVEEDOR ===\n";
    
    Transaccion nuevaCompra;
    strcpy(nuevaCompra.tipo, "COMPRA");
    
    listarProveedores(tienda);
    cout << "\n";
    
    if (!validarInt("Ingrese ID del proveedor (o 'cancelar' para cancelar): ", nuevaCompra.idRelacionado)) {
        return;
    }
    
    int indiceProveedor = buscarIndiceProveedorPorId(tienda, nuevaCompra.idRelacionado);
    if (indiceProveedor == -1) {
        cout << "ERROR: Proveedor con ID " << nuevaCompra.idRelacionado << " no existe.\n";
        return;
    }
    
    listarProductos(tienda);
    cout << "\n";
    
    if (!validarInt("Ingrese ID del producto (o 'cancelar' para cancelar): ", nuevaCompra.idProducto)) {
        return;
    }
    
    int indiceProducto = buscarIndiceProductoPorId(tienda, nuevaCompra.idProducto);
    if (indiceProducto == -1) {
        cout << "ERROR: Producto con ID " << nuevaCompra.idProducto << " no existe.\n";
        return;
    }
    
    if (!validarInt("Ingrese cantidad a comprar (o 'cancelar' para cancelar): ", nuevaCompra.cantidad)) {
        return;
    }
    if (nuevaCompra.cantidad <= 0) {
        cout << "ERROR: La cantidad debe ser mayor a 0.\n";
        return;
    }
    
    if (!validarFloat("Ingrese precio unitario de compra (o 'cancelar' para cancelar): ", nuevaCompra.precioUnitario)) {
        return;
    }
    if (nuevaCompra.precioUnitario <= 0) {
        cout << "ERROR: El precio debe ser mayor a 0.\n";
        return;
    }
    
    limpiarBuffer();
    
    nuevaCompra.total = nuevaCompra.cantidad * nuevaCompra.precioUnitario;
    tomarFechaActual(nuevaCompra.fecha, sizeof(nuevaCompra.fecha));
    
    cout << "Ingrese descripcion de la compra (opcional, Enter para omitir): ";
    cin.getline(nuevaCompra.descripcion, 200);
    
    cout << "\n-----------------------------\n";
    cout << "RESUMEN DE COMPRA:\n";
    cout << "Proveedor: " << tienda->proveedores[indiceProveedor].nombre << " (ID: " << nuevaCompra.idRelacionado << ")\n";
    cout << "Producto: " << tienda->productos[indiceProducto].nombre << " (ID: " << nuevaCompra.idProducto << ")\n";
    cout << "Cantidad: " << nuevaCompra.cantidad << "\n";
    cout << "Precio Unitario: $" << nuevaCompra.precioUnitario << "\n";
    cout << "Total: $" << nuevaCompra.total << "\n";
    cout << "Fecha: " << nuevaCompra.fecha << "\n";
    cout << "Descripcion: " << nuevaCompra.descripcion << "\n";
    cout << "-----------------------------\n";
    
    cout << "\nConfirmar compra? (s/n): ";
    char confirmacion[10];
    cin.getline(confirmacion, 10);
    
    if (tolower(confirmacion[0]) != 's') {
        cout << "Compra cancelada.\n";
        return;
    }
    
    nuevaCompra.id = tienda->siguienteIdTransaccion;
    
    if (tienda->numTransacciones >= tienda->capacidadTransacciones) {
        redimensionarArrayTransacciones(tienda);
    }
    
    tienda->transacciones[tienda->numTransacciones] = nuevaCompra;
    tienda->numTransacciones++;
    tienda->siguienteIdTransaccion++;
    
    tienda->productos[indiceProducto].stock += nuevaCompra.cantidad;
    
    cout << "Compra registrada exitosamente con ID: " << nuevaCompra.id << endl;
    cout << "Stock actualizado. Nuevo stock de " << tienda->productos[indiceProducto].nombre 
         << ": " << tienda->productos[indiceProducto].stock << endl;
}

void registrarVenta(Tienda* tienda) {
    limpiarPantalla();
    if (tienda->numClientes == 0) {
        cout << "ERROR: No hay clientes registrados. Debe crear un cliente primero.\n";
        return;
    }
    if (tienda->numProductos == 0) {
        cout << "ERROR: No hay productos registrados. Debe crear un producto primero.\n";
        return;
    }
    
    cout << "\n=== REGISTRAR VENTA A CLIENTE ===\n";
    
    Transaccion nuevaVenta;
    strcpy(nuevaVenta.tipo, "VENTA");
    
    listarClientes(tienda);
    cout << "\n";
    
    if (!validarInt("Ingrese ID del cliente (o 'cancelar' para cancelar): ", nuevaVenta.idRelacionado)) {
        return;
    }
    
    int indiceCliente = buscarIndiceClientePorId(tienda, nuevaVenta.idRelacionado);
    if (indiceCliente == -1) {
        cout << "ERROR: Cliente con ID " << nuevaVenta.idRelacionado << " no existe.\n";
        return;
    }
    
    listarProductos(tienda);
    cout << "\n";
    
    if (!validarInt("Ingrese ID del producto (o 'cancelar' para cancelar): ", nuevaVenta.idProducto)) {
        return;
    }
    
    int indiceProducto = buscarIndiceProductoPorId(tienda, nuevaVenta.idProducto);
    if (indiceProducto == -1) {
        cout << "ERROR: Producto con ID " << nuevaVenta.idProducto << " no existe.\n";
        return;
    }
    
    cout << "Stock disponible: " << tienda->productos[indiceProducto].stock << endl;
    
    if (!validarInt("Ingrese cantidad a vender (o 'cancelar' para cancelar): ", nuevaVenta.cantidad)) {
        return;
    }
    if (nuevaVenta.cantidad <= 0) {
        cout << "ERROR: La cantidad debe ser mayor a 0.\n";
        cout << "Presione Enter para continuar y ajustar la cantidad o cancelar la venta.\n";
        cin.get();
        return;
    }
    
    if (nuevaVenta.cantidad > tienda->productos[indiceProducto].stock) {
        cout << "ERROR: Stock insuficiente. Disponible: " << tienda->productos[indiceProducto].stock 
             << ", Solicitado: " << nuevaVenta.cantidad << endl;
        cout << "Presione Enter para continuar y ajustar la cantidad o cancelar la venta.\n";
        cin.get();
        return;
    }
    
    limpiarBuffer();
    
    cout << "Precio del producto: $" << tienda->productos[indiceProducto].precio << endl;
    cout << "Desea usar el precio del producto? (s/n): ";
    char usarPrecio[10];
    cin.getline(usarPrecio, 10);
    
    if (tolower(usarPrecio[0]) == 's') {
        nuevaVenta.precioUnitario = tienda->productos[indiceProducto].precio;
    } else {
        if (!validarFloat("Ingrese precio unitario de venta (o 'cancelar' para cancelar): ", nuevaVenta.precioUnitario)) {
            return;
        }
        if (nuevaVenta.precioUnitario <= 0) {
            cout << "ERROR: El precio debe ser mayor a 0.\n";
            return;
        }
        limpiarBuffer();
    }
    
    nuevaVenta.total = nuevaVenta.cantidad * nuevaVenta.precioUnitario;
    tomarFechaActual(nuevaVenta.fecha, sizeof(nuevaVenta.fecha));
    
    cout << "Ingrese descripcion de la venta (opcional, Enter para omitir): ";
    cin.getline(nuevaVenta.descripcion, 200);
    
    cout << "\n-----------------------------\n";
    cout << "RESUMEN DE VENTA:\n";
    cout << "Cliente: " << tienda->clientes[indiceCliente].nombre << " (ID: " << nuevaVenta.idRelacionado << ")\n";
    cout << "Producto: " << tienda->productos[indiceProducto].nombre << " (ID: " << nuevaVenta.idProducto << ")\n";
    cout << "Cantidad: " << nuevaVenta.cantidad << "\n";
    cout << "Precio Unitario: $" << nuevaVenta.precioUnitario << "\n";
    cout << "Total: $" << nuevaVenta.total << "\n";
    cout << "Fecha: " << nuevaVenta.fecha << "\n";
    cout << "Descripcion: " << nuevaVenta.descripcion << "\n";
    cout << "-----------------------------\n";
    
    cout << "\nConfirmar venta? (s/n): ";
    char confirmacion[10];
    cin.getline(confirmacion, 10);
    
    if (tolower(confirmacion[0]) != 's') {
        cout << "Venta cancelada.\n";
        return;
    }
    
    nuevaVenta.id = tienda->siguienteIdTransaccion;
    
    if (tienda->numTransacciones >= tienda->capacidadTransacciones) {
        redimensionarArrayTransacciones(tienda);
    }
    
    tienda->transacciones[tienda->numTransacciones] = nuevaVenta;
    tienda->numTransacciones++;
    tienda->siguienteIdTransaccion++;
    
    tienda->productos[indiceProducto].stock -= nuevaVenta.cantidad;
    
    cout << "Venta registrada exitosamente con ID: " << nuevaVenta.id << endl;
    cout << "Stock actualizado. Nuevo stock de " << tienda->productos[indiceProducto].nombre 
         << ": " << tienda->productos[indiceProducto].stock << endl;
}

void cancelarTransaccion(Tienda* tienda) {
    if (tienda->numTransacciones == 0) {
        cout << "No hay transacciones para cancelar.\n";
        return;
    }
    
    int id;
    if (!validarInt("Ingrese ID de la transaccion a cancelar (o 'cancelar' para volver): ", id)) {
        return;
    }
    
    int indice = buscarIndiceTransaccionPorId(tienda, id);
    if (indice == -1) {
        cout << "Transaccion con ID " << id << " no encontrada.\n";
        return;
    }
    
    Transaccion& t = tienda->transacciones[indice];
    
    cout << "\n=== DETALLE DE TRANSACCION A CANCELAR ===\n";
    cout << "ID: " << t.id << endl;
    cout << "Tipo: " << t.tipo << endl;
    
    int idxProducto = buscarIndiceProductoPorId(tienda, t.idProducto);
    if (idxProducto != -1) {
        cout << "Producto: " << tienda->productos[idxProducto].nombre << " (ID: " << t.idProducto << ")" << endl;
        cout << "Stock actual del producto: " << tienda->productos[idxProducto].stock << endl;
    }
    
    if (strcmp(t.tipo, "COMPRA") == 0) {
        int idxProv = buscarIndiceProveedorPorId(tienda, t.idRelacionado);
        if (idxProv != -1) {
            cout << "Proveedor: " << tienda->proveedores[idxProv].nombre << " (ID: " << t.idRelacionado << ")" << endl;
        }
        cout << "Cantidad comprada: " << t.cantidad << endl;
        cout << "ADVERTENCIA: Al cancelar esta compra, se RESTARAN " << t.cantidad 
             << " unidades del stock.\n";
    } else {
        int idxCli = buscarIndiceClientePorId(tienda, t.idRelacionado);
        if (idxCli != -1) {
            cout << "Cliente: " << tienda->clientes[idxCli].nombre << " (ID: " << t.idRelacionado << ")" << endl;
        }
        cout << "Cantidad vendida: " << t.cantidad << endl;
        cout << "ADVERTENCIA: Al cancelar esta venta, se SUMARAN " << t.cantidad 
             << " unidades al stock.\n";
    }
    
    cout << "Total: $" << t.total << endl;
    cout << "Fecha: " << t.fecha << endl;
    
    cout << "\nEsta seguro que desea CANCELAR esta transaccion? (s/n): ";
    string respuesta;
    getline(cin, respuesta);
    
    if (respuesta.empty() || tolower(respuesta[0]) != 's') {
        cout << "Cancelacion abortada.\n";
        return;
    }
    
    if (idxProducto != -1) {
        if (strcmp(t.tipo, "COMPRA") == 0) {
            tienda->productos[idxProducto].stock -= t.cantidad;
            if (tienda->productos[idxProducto].stock < 0) {
                tienda->productos[idxProducto].stock = 0;
            }
            cout << "Stock actualizado. Se restaron " << t.cantidad << " unidades. Nuevo stock: " 
                 << tienda->productos[idxProducto].stock << endl;
        } else {
            tienda->productos[idxProducto].stock += t.cantidad;
            cout << "Stock actualizado. Se sumaron " << t.cantidad << " unidades. Nuevo stock: " 
                 << tienda->productos[idxProducto].stock << endl;
        }
    }
    
    for (int i = indice; i < tienda->numTransacciones - 1; i++) {
        tienda->transacciones[i] = tienda->transacciones[i + 1];
    }
    tienda->numTransacciones--;
    
    cout << "Transaccion con ID " << id << " cancelada exitosamente.\n";
}

void buscarTransaccionPorId(Tienda* tienda, int id) {
    int indice = buscarIndiceTransaccionPorId(tienda, id);
    if (indice != -1) {
        Transaccion& t = tienda->transacciones[indice];
        cout << "\n=== TRANSACCION ENCONTRADA ===\n";
        cout << "ID: " << t.id << endl;
        cout << "Tipo: " << t.tipo << endl;
        cout << "ID Producto: " << t.idProducto << endl;
        cout << "ID Relacionado: " << t.idRelacionado << " (";
        if (strcmp(t.tipo, "COMPRA") == 0) {
            int idx = buscarIndiceProveedorPorId(tienda, t.idRelacionado);
            if (idx != -1) cout << tienda->proveedores[idx].nombre;
        } else {
            int idx = buscarIndiceClientePorId(tienda, t.idRelacionado);
            if (idx != -1) cout << tienda->clientes[idx].nombre;
        }
        cout << ")" << endl;
        cout << "Cantidad: " << t.cantidad << endl;
        cout << "Precio Unitario: $" << t.precioUnitario << endl;
        cout << "Total: $" << t.total << endl;
        cout << "Fecha: " << t.fecha << endl;
        cout << "Descripcion: " << t.descripcion << endl;
    } else {
        cout << "Transaccion con ID " << id << " no encontrada.\n";
    }
}

void buscarTransaccionPorProducto(Tienda* tienda, int idProducto) {
    int encontrados = 0;
    cout << "\n=== TRANSACCIONES DEL PRODUCTO ID: " << idProducto << " ===\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (tienda->transacciones[i].idProducto == idProducto) {
            Transaccion& t = tienda->transacciones[i];
            cout << "ID: " << t.id << ", Tipo: " << t.tipo 
                 << ", Cantidad: " << t.cantidad << ", Total: $" << t.total 
                 << ", Fecha: " << t.fecha << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron transacciones para el producto con ID " << idProducto << ".\n";
    } else {
        cout << "Total de transacciones encontradas: " << encontrados << endl;
    }
}

void buscarTransaccionPorCliente(Tienda* tienda, int idCliente) {
    int encontrados = 0;
    cout << "\n=== TRANSACCIONES DEL CLIENTE ID: " << idCliente << " ===\n";
    
    int idxCliente = buscarIndiceClientePorId(tienda, idCliente);
    if (idxCliente != -1) {
        cout << "Cliente: " << tienda->clientes[idxCliente].nombre << endl;
    }
    cout << "----------------------------------------\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (strcmp(tienda->transacciones[i].tipo, "VENTA") == 0 && 
            tienda->transacciones[i].idRelacionado == idCliente) {
            
            Transaccion& t = tienda->transacciones[i];
            
            int idxProducto = buscarIndiceProductoPorId(tienda, t.idProducto);
            const char* nombreProducto = (idxProducto != -1) ? tienda->productos[idxProducto].nombre : "Producto no encontrado";
            
            cout << "ID Transaccion: " << t.id << endl;
            cout << "Producto: " << nombreProducto << " (ID: " << t.idProducto << ")" << endl;
            cout << "Cantidad: " << t.cantidad << endl;
            cout << "Precio Unitario: $" << t.precioUnitario << endl;
            cout << "Total: $" << t.total << endl;
            cout << "Fecha: " << t.fecha << endl;
            cout << "Descripcion: " << t.descripcion << endl;
            cout << "----------------------------------------\n";
            
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron ventas para el cliente con ID " << idCliente << ".\n";
    } else {
        cout << "Total de ventas encontradas: " << encontrados << endl;
    }
}

void buscarTransaccionPorProveedor(Tienda* tienda, int idProveedor) {
    int encontrados = 0;
    cout << "\n=== TRANSACCIONES DEL PROVEEDOR ID: " << idProveedor << " ===\n";
    
    int idxProveedor = buscarIndiceProveedorPorId(tienda, idProveedor);
    if (idxProveedor != -1) {
        cout << "Proveedor: " << tienda->proveedores[idxProveedor].nombre << endl;
    }
    cout << "----------------------------------------\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (strcmp(tienda->transacciones[i].tipo, "COMPRA") == 0 && 
            tienda->transacciones[i].idRelacionado == idProveedor) {
            
            Transaccion& t = tienda->transacciones[i];
            
            int idxProducto = buscarIndiceProductoPorId(tienda, t.idProducto);
            const char* nombreProducto = (idxProducto != -1) ? tienda->productos[idxProducto].nombre : "Producto no encontrado";
            
            cout << "ID Transaccion: " << t.id << endl;
            cout << "Producto: " << nombreProducto << " (ID: " << t.idProducto << ")" << endl;
            cout << "Cantidad: " << t.cantidad << endl;
            cout << "Precio Unitario: $" << t.precioUnitario << endl;
            cout << "Total: $" << t.total << endl;
            cout << "Fecha: " << t.fecha << endl;
            cout << "Descripcion: " << t.descripcion << endl;
            cout << "----------------------------------------\n";
            
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron compras para el proveedor con ID " << idProveedor << ".\n";
    } else {
        cout << "Total de compras encontradas: " << encontrados << endl;
    }
}

void buscarTransaccionPorFecha(Tienda* tienda, const char* fecha) {
    int encontrados = 0;
    cout << "\n=== TRANSACCIONES DE LA FECHA: " << fecha << " ===\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (strcmp(tienda->transacciones[i].fecha, fecha) == 0) {
            Transaccion& t = tienda->transacciones[i];
            cout << "ID: " << t.id << ", Tipo: " << t.tipo 
                 << ", Producto ID: " << t.idProducto << ", Cantidad: " << t.cantidad 
                 << ", Total: $" << t.total << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron transacciones para la fecha " << fecha << ".\n";
    } else {
        cout << "Total de transacciones encontradas: " << encontrados << endl;
    }
}

void buscarTransaccionPorTipo(Tienda* tienda, const char* tipo) {
    char tipoMinusculas[20];
    convertirAminusculas(tipoMinusculas, tipo);
    
    int encontrados = 0;
    cout << "\n=== TRANSACCIONES DE TIPO: " << tipo << " ===\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++) {
        char tipoTransaccionMinusculas[20];
        convertirAminusculas(tipoTransaccionMinusculas, tienda->transacciones[i].tipo);
        
        if (strcmp(tipoTransaccionMinusculas, tipoMinusculas) == 0) {
            Transaccion& t = tienda->transacciones[i];
            cout << "ID: " << t.id << ", Producto ID: " << t.idProducto 
                 << ", Cantidad: " << t.cantidad << ", Total: $" << t.total 
                 << ", Fecha: " << t.fecha << endl;
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        cout << "No se encontraron transacciones de tipo " << tipo << ".\n";
    } else {
        cout << "Total de transacciones encontradas: " << encontrados << endl;
    }
}

void menuBuscarTransaccion(Tienda* tienda) {
    if (tienda->numTransacciones == 0) {
        cout << "No hay transacciones registradas para buscar.\n";
        return;
    }
    
    int opcion;
    do {
        cout << "===============================";
        cout << "\n=== BUSCAR TRANSACCIONES ===\n";
        cout << "===============================\n";
        cout << "1. Buscar por ID de transaccion\n";
        cout << "2. Buscar por ID de producto\n";
        cout << "3. Buscar por ID de cliente\n";
        cout << "4. Buscar por ID de proveedor\n";
        cout << "5. Buscar por fecha exacta (YYYY-MM-DD)\n";
        cout << "6. Buscar por tipo (COMPRA/VENTA)\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1: {
                int id;
                if (!validarInt("Ingrese ID de transaccion (o 'cancelar' para cancelar): ", id)) {
                    break;
                }
                buscarTransaccionPorId(tienda, id);
                break;
            }
            case 2: {
                int id;
                if (!validarInt("Ingrese ID de producto (o 'cancelar' para cancelar): ", id)) {
                    break;
                }
                buscarTransaccionPorProducto(tienda, id);
                break;
            }
            case 3: {
                int id;
                if (!validarInt("Ingrese ID de cliente (o 'cancelar' para cancelar): ", id)) {
                    break;
                }
                buscarTransaccionPorCliente(tienda, id);
                break;
            }
            case 4: {
                int id;
                if (!validarInt("Ingrese ID de proveedor (o 'cancelar' para cancelar): ", id)) {
                    break;
                }
                buscarTransaccionPorProveedor(tienda, id);
                break;
            }
            case 5: {
                char fecha[11];
                cout << "Ingrese fecha (YYYY-MM-DD) o 'cancelar' para cancelar: ";
                cin.getline(fecha, 11);
                
                if (strcmp(fecha, "cancelar") == 0) {
                    cout << "Busqueda cancelada.\n";
                    break;
                }
                
                if (validarFecha(fecha)) {
                    buscarTransaccionPorFecha(tienda, fecha);
                }
                break;
            }
            case 6: {
                char tipo[10];
                cout << "Ingrese tipo (COMPRA/VENTA) o 'cancelar' para cancelar: ";
                cin.getline(tipo, 10);
                
                if (strcmp(tipo, "cancelar") == 0) {
                    cout << "Busqueda cancelada.\n";
                    break;
                }
                
                buscarTransaccionPorTipo(tienda, tipo);
                break;
            }
            case 0:
                cout << "Volviendo...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

void menuListarTransacciones(Tienda* tienda) {
    if (tienda->numTransacciones == 0) {
        cout << "No hay transacciones registradas.\n";
        return;
    }
    cout << "===============================";
    cout << "\n=== LISTADO DE TRANSACCIONES ===\n";
    cout << "===============================\n";
    cout << "ID \tTipo \tProducto\tRelacionado\tCantidad\tTotal\tFecha\n";
    cout << "--------------------------------------------------------------------------------\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++) {
        Transaccion& t = tienda->transacciones[i];
        
        int idxProducto = buscarIndiceProductoPorId(tienda, t.idProducto);
        const char* nombreProducto = (idxProducto != -1) ? tienda->productos[idxProducto].nombre : "N/A";
        
        const char* nombreRelacionado = "N/A";
        if (strcmp(t.tipo, "COMPRA") == 0) {
            int idxProv = buscarIndiceProveedorPorId(tienda, t.idRelacionado);
            if (idxProv != -1) nombreRelacionado = tienda->proveedores[idxProv].nombre;
        } else {
            int idxCli = buscarIndiceClientePorId(tienda, t.idRelacionado);
            if (idxCli != -1) nombreRelacionado = tienda->clientes[idxCli].nombre;
        }
        
        cout << t.id << "\t" << t.tipo << "\t" << nombreProducto << "\t" 
             << nombreRelacionado << "\t" << t.cantidad << "\t$" << t.total 
             << "\t" << t.fecha << endl;
    }
    cout << "Total de transacciones: " << tienda->numTransacciones << endl;
}

void menuGestionProductos(Tienda* tienda) {
    int opcion;
    do {
        limpiarPantalla();
        cout << "================================";
        cout << "\n===== GESTION DE PRODUCTOS =====\n";
        cout << "================================\n";
        cout << "1. Crear nuevo producto\n";
        cout << "2. Listar productos\n";
        cout << "3. Buscar producto\n";
        cout << "4. Actualizar producto\n";
        cout << "5. Eliminar producto\n";
        cout << "0. Volver al menu principal\n\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
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
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

void menuGestionProveedores(Tienda* tienda) {
    int opcion;
    do {
        limpiarPantalla();
        cout << "================================";
        cout << "\n==== GESTION DE PROVEEDORES ====\n";
        cout << "================================\n";
        cout << "1. Crear nuevo proveedor\n";
        cout << "2. Listar proveedores\n";
        cout << "3. Buscar proveedor\n";
        cout << "4. Actualizar proveedor\n";
        cout << "5. Eliminar proveedor\n";
        cout << "0. Volver al menu principal\n\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
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
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

void menuGestionClientes(Tienda* tienda) {
    int opcion;
    do {
        limpiarPantalla();
        cout << "================================";
        cout << "\n=== GESTION DE CLIENTES ===\n";
        cout << "================================\n";
        cout << "1. Registrar cliente.\n";
        cout << "2. Buscar cliente.\n";
        cout << "3. Actualizar cliente.\n";
        cout << "4. Listar clientes.\n";
        cout << "5. Eliminar cliente.\n";
        cout << "0. Volver al menu principal.\n\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();

        switch (opcion) {
            case 1: 
                crearCliente(tienda);
                break;
            case 2:
                buscarCliente(tienda);
                break;
            case 3:
                actualizarCliente(tienda);
                break;
            case 4:
                listarClientes(tienda);
                break;
            case 5:
                eliminarCliente(tienda);
                break;
            case 0:
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        
    } while(opcion != 0);
}

void menuGestionTransacciones(Tienda* tienda) {
    int opcion;
    do {
        limpiarPantalla();
        cout << "================================";
        cout << "\n=== MENU DE TRANSACCIONES ===\n";
        cout << "================================\n";
        cout << "1. Registrar compra (a proveedor).\n";
        cout << "2. Registrar venta (a cliente).\n";
        cout << "3. Buscar transacciones.\n";
        cout << "4. Listar todas las transacciones\n";
        cout << "5. Cancelar/Anular transaccion\n";
        cout << "0. Volver al menu principal.\n\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();

        switch (opcion){
            case 1:
                registrarCompra(tienda);
                break;
            case 2:
                registrarVenta(tienda);
                break;
            case 3:
                menuBuscarTransaccion(tienda);
                break;
            case 4:
                menuListarTransacciones(tienda);
                break;
            case 5:
                cancelarTransaccion(tienda);
                break;
            case 0:
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while(opcion != 0);
}

void menuPrincipal(Tienda* tienda) {
    int opcion;
    do {
        limpiarPantalla();
        cout << "======================";
        cout << "\n=== MENU PRINCIPAL ===\n";
        cout << "======================\n";
        cout << "1. Gestion de productos\n";
        cout << "2. Gestion de proveedores\n";
        cout << "3. Gestion de clientes\n";
        cout << "4. Gestion de transacciones\n";
        cout << "0. Salir\n\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch(opcion) {
            case 1:
                menuGestionProductos(tienda);
                break;
            case 2:
                menuGestionProveedores(tienda);
                break;
            case 3:
                menuGestionClientes(tienda);
                break;
            case 4:
                menuGestionTransacciones(tienda);
                break;
            case 0:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opcion no valida. Por favor intente nuevamente.\n";
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