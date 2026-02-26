<img src="https://64.media.tumblr.com/2e317ca5b9fb13feebde242e45f24172/73f0c4d120734f31-c8/s2048x3072/1fc0b91846abd27352cd497226c3aa07b6bd6a88.pnj">

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Descripción del Proyecto</strong></div></h1>

Sistema integral de gestión para tiendas desarrollado en C++ que permite administrar:
- Productos
- Proveedores
- Clientes
- Transacciones (compras y ventas)

El sistema implementa un menú interactivo con operaciones CRUD (Crear, Leer, Actualizar, Eliminar) para cada entidad, además de funcionalidades específicas como búsqueda por múltiples criterios, gestión de inventario y registro de transacciones con actualización automática de stock.

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Instrucciones de Compilación</strong></div></h1> 

### Requisitos
- Compilador de C++ (g++, MinGW, MSVC, etc.)
- Estándar C++11 o superior

### Compilación en Linux/Mac
```bash
g++ -o tienda main.cpp -std=c++11
```

### Compilación en Windows (MinGW)
```bash
g++ -o tienda.exe main.cpp -std=c++11
```

### Compilación con CMake
Crear archivo `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.10)
project(SistemaTienda)

set(CMAKE_CXX_STANDARD 11)

add_executable(tienda main.cpp)
```

Luego ejecutar:
```bash
mkdir build
cd build
cmake ..
make
```

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Instrucciones de Ejecución</strong></div></h1>

### En Linux/Mac
```bash
./tienda
```

### En Windows
```bash
tienda.exe
```

### Navegación por el menú
El sistema utiliza un menú jerárquico. Seleccione la opción deseada ingresando el número correspondiente y presione Enter.

Para cancelar cualquier operación de ingreso de datos, escriba `cancelar`.

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Estructura del Código</strong></div></h1> 

### Estructuras Principales
```
├── Producto          # Almacena información de productos
├── Proveedor         # Datos de proveedores
├── Cliente           # Datos de clientes  
├── Transaccion       # Registro de compras/ventas
├── Tienda            # Estructura principal que contiene todos los arrays
└── Cambios*          # Estructuras para control de cambios (Producto/Proveedor/Cliente)
```

### Funciones por Módulo

#### Utilidades
- `limpiarPantalla()` - Limpia la consola
- `limpiarBuffer()` - Limpia el buffer de entrada
- Funciones de validación (`validarChar`, `validarInt`, `validarFloat`, `validarEmail`, `validarFecha`)

#### Gestión de Memoria
- `inicializarTienda()` - Inicializa la estructura Tienda con arrays dinámicos
- `liberarTienda()` - Libera la memoria asignada
- `redimensionarArray*()` - Redimensiona arrays cuando se llenan

#### CRUD Productos
- `crearProducto()`
- `listarProductos()`
- `buscarProducto()` (por ID, nombre, código, proveedor)
- `actualizarProducto()` (con sistema de cambios pendientes)
- `eliminarProducto()`

#### CRUD Proveedores
- `crearProveedor()`
- `listarProveedores()`
- `buscarProveedor()` (por ID, nombre, identificación)
- `actualizarProveedor()`
- `eliminarProveedor()`

#### CRUD Clientes
- `crearCliente()`
- `listarClientes()`
- `buscarCliente()` (por ID, nombre, identificación)
- `actualizarCliente()`
- `eliminarCliente()`

#### Transacciones
- `registrarCompra()` - Compra a proveedor (aumenta stock)
- `registrarVenta()` - Venta a cliente (disminuye stock)
- `cancelarTransaccion()` - Anula transacción (revierte efecto en stock)
- `buscarTransaccion()` (por ID, producto, cliente, proveedor, fecha, tipo)
- `listarTransacciones()`

#### Menús
- `menuPrincipal()`
- `menuGestionProductos()`
- `menuGestionProveedores()`
- `menuGestionClientes()`
- `menuGestionTransacciones()`
- `menuBuscarTransaccion()`

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Funcionalidades Implementadas</strong></div></h1>

### Gestión de Productos ✓
- Crear producto con código único, nombre, descripción, proveedor, precio y stock
- Listar todos los productos
- Buscar por ID, nombre (parcial), código (parcial) o proveedor
- Actualizar producto con sistema de cambios pendientes
- Ajuste de stock (sumar/restar) sin sobrescribir
- Eliminar producto

### Gestión de Proveedores ✓
- Registrar proveedor con formato RIF (J-12345678-9)
- Identificación única
- Listar, buscar y actualizar proveedores
- Eliminar proveedor con verificación de productos asociados

### Gestión de Clientes ✓
- Registrar cliente con formato de cédula (V-12345678 o E-12345678)
- Identificación única
- Listar, buscar y actualizar clientes
- Eliminar cliente con verificación de transacciones asociadas

### Gestión de Transacciones ✓
- Registrar compras a proveedores (aumenta stock)
- Registrar ventas a clientes (disminuye stock)
- Validación de stock disponible para ventas
- Cancelación de transacciones (revierte efecto en stock)
- Búsqueda por múltiples criterios (ID, producto, cliente, proveedor, fecha, tipo)
- Listado completo de transacciones

### Validaciones ✓
- Entradas de usuario robustas
- Validación de formatos (email, fechas, identificaciones)
- Unicidad de códigos e identificaciones
- Prevención de inyección de buffer
- Opción "cancelar" en todas las entradas

### Gestión de Memoria Dinámica ✓
- Arrays redimensionables
- Liberación de memoria al finalizar

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Casos de Prueba Ejecutados</strong></div></h1> 

### Caso 1: Creación de Entidades




### Caso 2: Validación de Unicidad


### Caso 3: Búsqueda


### Caso 4: Actualización de Producto


### Caso 5: Transacciones


### Caso 6: Validación de Stock en Ventas


### Caso 7: Cancelación de Transacción con Verificación


### Caso 8: Eliminación con Verificación


### Caso 9: Redimensionamiento de Arrays


### Caso 10: Validación de Formatos


