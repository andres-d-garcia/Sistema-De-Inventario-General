<img src="https://64.media.tumblr.com/2e317ca5b9fb13feebde242e45f24172/73f0c4d120734f31-c8/s2048x3072/1fc0b91846abd27352cd497226c3aa07b6bd6a88.pnj">

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Descripción del Proyecto</strong></div></h1>

Sistema integral de gestión para tiendas desarrollado en C++ que permite administrar:
- Productos
- Proveedores
- Clientes
- Transacciones (compras y ventas con múltiples productos)

### Filosofía de Diseño

**A diferencia de la versión anterior (Proyecto 1), esta implementación NO carga todos los datos en memoria RAM.** Los datos residen permanentemente en disco mediante **archivos binarios**, y solo se carga **UN registro a la vez** en memoria cuando es necesario procesarlo.

### Características Principales

- **Persistencia con archivos binarios**: 5 archivos independientes (tienda, productos, proveedores, clientes, transacciones)
- **Acceso aleatorio**: Cálculo de offsets con `sizeof(Header) + (indice * sizeof(Struct))`
- **Headers de archivo**: Metadata administrativa (cantidad de registros, próximo ID, registros activos)
- **Borrado lógico**: Flag `eliminado` que mantiene las posiciones físicas intactas
- **Transacciones con múltiples productos**: Hasta 10 productos por transacción
- **Integridad referencial**: Verificación de relaciones entre entidades
- **Backup automático**: Copia de seguridad con timestamp
- **Menú interactivo** con operaciones CRUD completas


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
### Primera Ejecución
Al iniciar el sistema por primera vez, se crearán automáticamente los archivos binarios:

- `tienda.bin`

- `productos.bin`

- `proveedores.bin`

- `clientes.bin`

- `transacciones.bin`

Se solicitará la configuración inicial de la tienda (nombre, RIF, dirección, teléfono, email).

### Navegación por el menú
El sistema utiliza un menú jerárquico. Seleccione la opción deseada ingresando el número correspondiente y presione Enter.

Para cancelar cualquier operación de ingreso de datos, escriba `cancelar`.

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Estructura del Código</strong></div></h1>

### Organización por Secciones
SECCIÓN 1:  CONSTANTES Y NOMBRES DE ARCHIVOS
SECCIÓN 2:  ESTRUCTURAS BINARIAS
SECCIÓN 3:  UTILIDADES GENERALES
SECCIÓN 4:  GESTIÓN DE ARCHIVOS - HEADERS Y OFFSETS
SECCIÓN 5:  OPERACIONES CRUD GENÉRICAS
SECCIÓN 6:  VALIDACIONES DE ENTRADA
SECCIÓN 7:  VALIDACIONES DE UNICIDAD
SECCIÓN 8:  TIENDA (registro único)
SECCIÓN 9:  FUNCIONES DE PRODUCTO (MOSTRAR)
SECCIÓN 10: CRUD DE PRODUCTOS
SECCIÓN 11: FUNCIONES DE PROVEEDOR (MOSTRAR)
SECCIÓN 12: CRUD DE PROVEEDORES
SECCIÓN 13: FUNCIONES DE CLIENTE (MOSTRAR)
SECCIÓN 14: CRUD DE CLIENTES
SECCIÓN 15: FUNCIONES DE TRANSACCIÓN (MOSTRAR)
SECCIÓN 16: CRUD DE TRANSACCIONES
SECCIÓN 17: REPORTES Y MANTENIMIENTO
SECCIÓN 18: MENÚ PRINCIPAL
SECCIÓN 19: MAIN

### Estructuras Principales (Tamaño Fijo)

| Estructura | Tamaño aprox. | Descripción |
|------------|---------------|-------------|
| `ArchivoHeader` | 16 bytes | Metadata de cada archivo |
| `Tienda` | ~160 bytes | Datos de la empresa y estadísticas |
| `Producto` | ~376 bytes | Información de productos |
| `Proveedor` | ~860 bytes | Datos de proveedores + relaciones |
| `Cliente` | ~900 bytes | Datos de clientes + historial |
| `ItemTransaccion` | 20 bytes | Línea de detalle de transacción |
| `Transaccion` | ~500 bytes | Transacción con múltiples productos |


### Funciones Genéricas (Templates)
template<typename T>
int escribirRegistroAlFinal(const char* archivo, T& registro);

template<typename T>
bool leerRegistroPorIndice(const char* archivo, int indice, T& destino);

template<typename T>
int buscarIndiceFisicoPorId(const char* archivo, int idBuscado);

template<typename T>
bool borradoLogico(const char* archivo, int indiceFisico);

### Cálculo de Offsets (Acceso Aleatorio)
template<typename T>
long calcularOffset(int indiceFisico) {
    return (long)sizeof(ArchivoHeader) + (long)(indiceFisico * sizeof(T));
}

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Funcionalidades Implementadas</strong></div></h1>

### Gestión de Productos ✓
- Crear producto con código único, nombre, descripción, proveedor, precio, stock y stock mínimo
- Validación de existencia de proveedor antes de crear
- Listar productos con información del proveedor (nombre, RIF, teléfono)
- Buscar por ID o nombre (coincidencia parcial)
- Actualizar producto (código, nombre, descripción, proveedor, precio, stock, stock mínimo)
- Ajuste de stock (sumar/restar) sin sobrescribir
- Eliminar producto (borrado lógico con flag `eliminado`)

### Gestión de Proveedores ✓
- Registrar proveedor con validación de RIF (formato J-12345678-9)
- Identificación única (no duplicados)
- Listar proveedores con cantidad de productos asociados
- Buscar por ID o nombre
- Actualizar proveedor (nombre, dirección, teléfono, email, RIF)
- Eliminar proveedor con verificación de productos asociados

### Gestión de Clientes ✓
- Registrar cliente con validación de cédula (V-12345678 o E-12345678)
- Identificación única
- Listar clientes con total gastado
- Buscar por ID o nombre
- Actualizar cliente
- Eliminar cliente con verificación de transacciones asociadas

### Gestión de Transacciones ✓
- **Ventas**: Registrar ventas a clientes con múltiples productos
  - Validación de stock disponible antes de procesar
  - Precio configurable (usar precio del producto o ingresar otro)
  - Actualización automática de stock y estadísticas del cliente
- **Compras**: Registrar compras a proveedores con múltiples productos
  - Aumento automático de stock
  - Precio de compra independiente del precio de venta
- **Cancelación**: Anular transacciones (revierte efecto en stock y estadísticas)
- **Listado**: Visualización formateada con información de contraparte
- **Historial de cliente**: Todas las compras de un cliente específico

### Reportes y Mantenimiento ✓
- **Resumen general**: Estadísticas de todas las entidades
- **Stock crítico**: Productos con stock ≤ stock mínimo
- **Integridad referencial**: Verificación de relaciones entre archivos
  - Productos → Proveedores
  - Transacciones → Clientes/Proveedores
  - Transacciones → Productos
- **Backup**: Copia de seguridad de todos los archivos .bin con timestamp

### Validaciones ✓
- Entradas de usuario robustas con `limpiarBuffer()`
- Validación de formatos:
  - Email (usuario@dominio.extension)
  - RIF (J-12345678-9)
  - Cédula (V-12345678 o E-12345678)
  - Fechas (YYYY-MM-DD)
- Unicidad de códigos e identificaciones
- Prevención de inyección de buffer
- Opción "cancelar" en todas las entradas

### Persistencia y Acceso a Datos ✓
- **Headers de archivo**: Metadata para acceso rápido
- **Borrado lógico**: Los registros eliminados mantienen su posición
- **IDs autoincrementales**: Por archivo, gestionados en el header
- **Relaciones**: Arrays fijos de IDs para relaciones 1:N
- **Estadísticas globales**: Actualización automática en Tienda

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Arquitectura de Archivos Binarios</strong></div></h1> 

### Estructura de Cada Archivo
[HEADER: 16 bytes] → ArchivoHeader (cantidadRegistros, proximoId, registrosActivos, version)
[REGISTRO 0]       → sizeof(Struct) bytes
[REGISTRO 1]       → sizeof(Struct) bytes
...
[REGISTRO N]       → sizeof(Struct) bytes

### Cálculo de Posiciones
Para acceder al registro en la posición física `i`:

offset = sizeof(ArchivoHeader) + (i * sizeof(Struct))

### Borrado Lógico

Los registros no se eliminan físicamente del archivo. Se marca `eliminado = true` y se actualiza el contador `registrosActivos` en el header. Esto mantiene todas las posiciones físicas intactas y permite cálculos de offset consistentes.

### IDs Auto-incrementales

Cada archivo mantiene su propio contador de IDs en el header (`proximoId`). Al crear un nuevo registro:

1. Se lee el header para obtener `proximoId`
2. Se asigna ese ID al nuevo registro
3. Se incrementa `proximoId` en el header
4. Se guarda el header actualizado

### Relaciones entre Archivos

Las relaciones 1:N se implementan mediante arrays fijos de IDs:

- **Proveedor → Productos**: `int productosIDs[MAX_PRODUCTOS_POR_PROVEEDOR]`
- **Cliente → Transacciones**: `int comprasIDs[MAX_TRANSACCIONES_POR_ENTIDAD]`
- **Transacción → Productos**: `ItemTransaccion items[MAX_ITEMS_POR_TRANSACCION]`

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Diagrama de Clases (Estructuras)</strong></div></h1> 

┌─────────────────┐      ┌─────────────────┐
│    Producto     │      │   Proveedor     │
├─────────────────┤      ├─────────────────┤
│ id              │      │ id              │
│ codigo[20]      │      │ nombre[100]     │
│ nombre[100]     │      │ direccion[200]  │
│ descripcion[200]│      │ telefono[20]    │
│ precio          │      │ email[100]      │
│ stock           │◄─────┤ identificacion  │
│ idProveedor     │      │ productosIDs[]  │
│ stockMinimo     │      │ cantidadProductos│
│ totalVendidos   │      └─────────────────┘
│ eliminado       │             ▲
└─────────────────┘             │
        ▲                       │
        │                       │
┌───────┴───────┐      ┌────────┴────────┐
│  Transaccion  │      │     Cliente     │
├───────────────┤      ├─────────────────┤
│ id            │      │ id              │
│ tipo[10]      │      │ nombre[100]     │
│ idCliente     │─────►│ direccion[200]  │
│ idProveedor   │      │ telefono[20]    │
│ items[]       │      │ email[100]      │
│ cantidadItems │      │ identificacion  │
│ total         │      │ comprasIDs[]    │
│ fecha[11]     │      │ cantidadCompras │
│ eliminado     │      │ totalGastado    │
└───────────────┘      └─────────────────┘

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Manual de Usuario Rápido</strong></div></h1> 

==========================================================
   SISTEMA DE INVENTARIO | NOMBRE_TIENDA | RIF: J-XXXXXXXX-X
==========================================================
1. Gestión de Productos
2. Gestión de Proveedores
3. Gestión de Clientes
4. Gestión de Transacciones
5. Reportes y Mantenimiento
6. Configurar Tienda
0. Salir
==========================================================
Opcion: 

### Gestión de Productos
- **Crear producto**: Ingresar código único, nombre, descripción, proveedor, precio, stock y stock mínimo. Valida existencia del proveedor antes de crear.
- **Listar productos**: Muestra todos los productos con información del proveedor (nombre, RIF, teléfono) en formato tabular.
- **Buscar producto**: Por ID (detalle completo) o por nombre (coincidencia parcial insensible a mayúsculas).
- **Actualizar producto**: Modificar campos individualmente (código, nombre, descripción, proveedor, precio, stock, stock mínimo).
- **Ajustar stock**: Sumar o restar cantidad sin sobrescribir el valor actual.
- **Eliminar producto**: Borrado lógico con flag `eliminado` y confirmación del usuario.

### Gestión de Proveedores
- **Registrar proveedor**: Validación de formato RIF (J-12345678-9) y unicidad de identificación.
- **Listar proveedores**: Muestra todos los proveedores con la cantidad de productos asociados a cada uno.
- **Buscar proveedor**: Por ID o por nombre (coincidencia parcial).
- **Actualizar proveedor**: Modificar nombre, dirección, teléfono, email o RIF.
- **Eliminar proveedor**: Verificación de productos asociados antes de permitir la eliminación (borrado lógico).

### Gestión de Clientes
- **Registrar cliente**: Validación de formato de cédula (V-12345678 o E-12345678) y unicidad de identificación.
- **Listar clientes**: Muestra todos los clientes con el total gastado en sus compras.
- **Buscar cliente**: Por ID o por nombre (coincidencia parcial).
- **Actualizar cliente**: Modificar datos personales del cliente.
- **Eliminar cliente**: Verificación de transacciones asociadas antes de permitir la eliminación (borrado lógico).
- **Historial de compras**: Visualización de todas las transacciones realizadas por un cliente específico.

### Gestión de Transacciones
- **Registrar venta**:
  - Selección de cliente existente
  - Agregar hasta 10 productos por transacción
  - Validación de stock disponible antes de procesar
  - Precio configurable (usar precio del producto o ingresar otro)
  - Actualización automática de stock y estadísticas del cliente
- **Registrar compra**:
  - Selección de proveedor existente
  - Agregar múltiples productos con sus cantidades
  - Precio de compra independiente del precio de venta
  - Aumento automático de stock
- **Cancelar transacción**:
  - Anulación de venta o compra existente
  - Reversión automática del efecto en stock
  - Actualización de estadísticas del cliente/proveedor
- **Listar transacciones**: Visualización formateada con información de la contraparte (cliente o proveedor)
- **Buscar transacciones**: Por ID, tipo (compra/venta), fecha o entidad involucrada

### Reportes y Mantenimiento
- **Resumen general**: Estadísticas globales del sistema mostrando:
  - Total de productos, proveedores, clientes y transacciones
  - Valor total del inventario
  - Total de ventas y compras realizadas
- **Stock crítico**: Listado de productos con stock ≤ stock mínimo, alertando sobre necesidad de reabastecimiento
- **Integridad referencial**: Verificación automática de relaciones entre archivos:
  - Productos huerfanos (sin proveedor válido)
  - Transacciones con clientes o proveedores inexistentes
  - Transacciones con productos no registrados
- **Backup**: Creación de copia de seguridad de todos los archivos .bin con timestamp en el nombre