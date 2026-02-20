#include <iostream>
#include <cstring>
#include <ctime>
using namespace std;

// sistema de inventario general
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
    char tipo[20]; // "compra" o "venta"
    int idProducto;
    int idRelacionado; // Puede ser idProveedor o idCliente dependiendo del tipo de transacción
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

void inicializarTienda(Tienda* tienda, const char* nombre = "Mi Tienda", const char* rif ="J-12345678-9"){
    cout << "Inicializando tienda..." << endl;
    
    // Copiar nombre y RIF a la estructura
    strcpy(tienda->nombre, nombre);
    strcpy(tienda->rif, rif);
    
    cout << "Nombre: " << tienda->nombre << endl;
    cout << "RIF: " << tienda->rif << endl;
    
    //array dinámico para productos
    tienda->productos = new Producto[5];
    tienda->numProductos = 0;
    tienda->capacidadProductos = 5;
    
    //array dinámico para proveedores
    tienda->proveedores = new Proveedor[5];
    tienda->numProveedores = 0;
    tienda->capacidadProveedores = 5;
    
    //array dinámico para clientes
    tienda->clientes = new Cliente[5];
    tienda->numClientes = 0;
    tienda->capacidadClientes = 5;
    
    //array dinámico para transacciones
    tienda->transacciones = new Transaccion[5];
    tienda->numTransacciones = 0;   
    tienda->capacidadTransacciones = 5;
    
    //inicializar IDs
    tienda->siguienteIdProducto = 1;
    tienda->siguienteIdProveedor = 1;
    tienda->siguienteIdCliente = 1;
    tienda->siguienteIdTransaccion = 1;
    
    cout << "Tienda inicializada correctamente." << endl;
};

void liberarTienda(Tienda* tienda) {
    delete[] tienda->productos;
    delete[] tienda->proveedores;
    delete[] tienda->clientes;
    delete[] tienda->transacciones;
    cout << "Memoria de la tienda liberada correctamente." << endl;
}

void crearProducto(Tienda* tienda){
    Producto nuevoProducto;
    
    cout << "Desea agregar un nuevo producto? (s/n): ";
    char respuesta;
    cin >> respuesta;
    cin.ignore();
    if (respuesta != 's' && respuesta != 'S') {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    
    cout << "Ingrese el código del producto (o 'cancelar' para cancelar): ";
    cin.getline(nuevoProducto.codigo, 50); 
    
    if (strcmp(nuevoProducto.codigo, "cancelar") == 0) {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    
    // verificacion de codigo unico
    for (int i = 0; i < tienda->numProductos; i++) {
        if (strcmp(tienda->productos[i].codigo, nuevoProducto.codigo) == 0) {
            cout << "Error: El código del producto ya existe. Creación de producto cancelada." << endl;
            return;
        }
    } 
    
    cout << "Ingrese el nombre del producto (o 'cancelar' para cancelar): ";
    cin.getline(nuevoProducto.nombre, 100);
    
    if (strcmp(nuevoProducto.nombre, "cancelar") == 0) {
        cout << "Creación de producto cancelada." << endl;
        return;
    }

    cout << "Ingrese la descripción del producto (o 'cancelar' para cancelar): ";
    cin.getline(nuevoProducto.descripcion, 200);
    
    if (strcmp(nuevoProducto.descripcion, "cancelar") == 0) {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    
    cout << "Ingrese el ID del proveedor (o -1 para cancelar): ";
    cin >> nuevoProducto.idProveedor;
    
    if (nuevoProducto.idProveedor == -1) {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    
    cout << "Ingrese el precio del producto (o -1 para cancelar): ";
    cin >> nuevoProducto.precio;
    
    if (nuevoProducto.precio == -1) {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    
    cout << "Ingrese el stock del producto (o -1 para cancelar): ";
    cin >> nuevoProducto.stock;
    
    if (nuevoProducto.stock == -1) {
        cout << "Creación de producto cancelada." << endl;
        return;
    }
    
    cin.ignore(); // Limpiar buffer antes de leer fecha
    
    cout << "Tomando fecha del sistema... ";
    time_t t = time(nullptr);
    strftime(nuevoProducto.fechaRegistro, sizeof(nuevoProducto.fechaRegistro), "%Y-%m-%d", localtime(&t));
    cout << "Fecha de registro: " << nuevoProducto.fechaRegistro << endl;
    
    // asignar ID
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
    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Creación de producto cancelada." << endl;
        return; // se sale de la funcion sin agregar el producto
    }

    // Verificar si necesitamos expandir el array
    if (tienda->numProductos >= tienda->capacidadProductos) {
        cout << "Advertencia: Capacidad de productos alcanzada." << endl;
        return;
    }
    
    // agregar producto a la tienda
    tienda->productos[tienda->numProductos] = nuevoProducto;
    tienda->numProductos++;
    
    cout << "Producto creado exitosamente con ID: " << nuevoProducto.id << endl; 
};

int main() {
    setlocale(LC_ALL, "");
    Tienda miTienda;
    inicializarTienda(&miTienda, "Garibaldi", "J-67676767");
    crearProducto(&miTienda); 
    crearProducto(&miTienda); 
    
    
    if (miTienda.numProductos <= 0) {
        cout << "No se han agregado productos a la tienda." << endl;
    } else {
        // cout << "\n--- Resumen ---" << endl;
        // cout << "Último producto agregado:" << endl;
        cout << "ID: " << miTienda.productos[miTienda.numProductos-1].id << endl;
        cout << "Código: " << miTienda.productos[miTienda.numProductos-1].codigo << endl;
        cout << "Nombre: " << miTienda.productos[miTienda.numProductos-1].nombre << endl;
    }
    

    liberarTienda(&miTienda);
    return 0;
}