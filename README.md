<img src="https://64.media.tumblr.com/2e317ca5b9fb13feebde242e45f24172/73f0c4d120734f31-c8/s2048x3072/1fc0b91846abd27352cd497226c3aa07b6bd6a88.pnj">

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Descripción del Proyecto</strong></div></h1>

# Sistema de Inventario — Fase 3 (POO y Modularización)


-----------------

Sistema integral de gestión para tiendas desarrollado en C++ (Fase 3). Esta versión evoluciona la arquitectura previa (Fase 2) hacia un diseño modular orientado a objetos, separando responsabilidades, encapsulando entidades y centralizando la persistencia en un gestor de archivos genérico.

Principales cambios (Fase 2 → Fase 3)
-----------------------------------

- Migración de `struct` a `class` con encapsulamiento: atributos privados, `getters` const, `setters` con validación, constructores (por defecto, parametrizados, copia) y destructores.
- Modularización por dominios: cada entidad y su lógica de negocio están en su propio módulo (`tienda/`, `productos/`, `proveedores/`, `clientes/`, `transacciones/`, `persistencia/`, `interfaz/`, `utilidades/`). Cada módulo usa `.hpp` + `.cpp`.
- Persistencia centralizada en `GestorArchivos`: operaciones genéricas de E/S implementadas mediante plantillas (templates) para evitar duplicación. Los templates se implementan en headers para que el compilador los instancie.
- Acceso aleatorio y headers en archivos binarios: cada archivo tiene un `ArchivoHeader` con metadata (cantidad, próximoId, activos) y se calcula offset mediante `sizeof(ArchivoHeader) + indice * sizeof(T)`.
- No se cargan todos los registros en memoria: el sistema mantiene la filosofía de acceder y procesar un registro a la vez para reducir el consumo de RAM.
- Interfaz separada: la clase `Interfaz` centraliza menús y navegación; `main.cpp` queda mínimo y delega en `Interfaz::ejecutar()`.
- Borrado lógico y consistencia: se mantiene el flag `eliminado` para borrados lógicos y se actualizan contadores en el header.
- Uso de constantes y límites: `MAX_ITEMS_POR_TRANSACCION`, `MAX_PRODUCTOS_POR_PROVEEDOR`, etc., para espacios fijos y cálculo de tamaños.

Estructura del proyecto
-----------------------

Raíz del repositorio (resumen):

```
main.cpp
Makefile
persistencia/
  ├─ GestorArchivos.hpp
  ├─ GestorArchivos.cpp
  └─ Constantes.hpp
tienda/
  ├─ Tienda.hpp
  └─ Tienda.cpp
productos/
  ├─ Producto.hpp
  ├─ Producto.cpp
  └─ operacionesProductos.*
proveedores/
clientes/
transacciones/
interfaz/
utilidades/
datos/    # generados en tiempo de ejecución
```

Persistencia y `GestorArchivos`
--------------------------------

La persistencia se centraliza en la capa `persistencia/` mediante funciones template que realizan:

- escribir/leer registros por índice
- buscar índice físico por ID
- actualización de headers y conteos

Implementar plantillas permite reutilizar la lógica para `Producto`, `Proveedor`, `Cliente`, `Transaccion` y `Tienda` sin repetir código.

Compilación y ejecución
-----------------------

Makefile
- Usar `make` en entornos Unix/MSYS. En Windows con MinGW/TDM use `mingw32-make` si `make` no está disponible.

Compilación directa (ejemplo con TDM/MinGW):

```bash
g++ -std=c++11 -Wall -Wextra -g \
  main.cpp persistencia/GestorArchivos.cpp tienda/Tienda.cpp \
  productos/Producto.cpp productos/operacionesProductos.cpp \
  proveedores/Proveedor.cpp proveedores/operacionesProveedores.cpp \
  clientes/Cliente.cpp clientes/operacionesClientes.cpp \
  transacciones/Transaccion.cpp transacciones/operacionesTransacciones.cpp \
  interfaz/Interfaz.cpp utilidades/Validaciones.cpp utilidades/Formatos.cpp \
  -o inventario.exe
```

Ejecución
- En Windows: `inventario.exe`
- En Unix/macOS: `./inventario`

Notas sobre el entorno Windows
- Si usas TDM o MinGW, asegúrate de tener `g++` en el `PATH`. Si no hay `make`, usa `mingw32-make` o compila con el comando `g++` anterior.

Estado actual del repositorio
-----------------------------

- La estructura modular y las cabeceras de las clases han sido añadidas según Fase 3.
- Se centralizó la persistencia y se definieron plantillas y constantes en `persistencia/Constantes.hpp` y `GestorArchivos.hpp` (implementación parcial según desarrollo).
- Algunos módulos contienen implementaciones mínimas o `stubs` en `transacciones/` y otros archivos para permitir compilación y pruebas iniciales; la lógica completa de negocio puede necesitar más trabajo (ver comentarios en los archivos correspondientes).

Archivo clave
- `Makefile`: versión más portable y preparada para entornos MinGW/TDM (ver [Makefile](Makefile)).

Próximos pasos sugeridos
-----------------------

1. Completar la implementación de `GestorArchivos` (templates y tests de E/S).
2. Implementar la lógica completa de `operacionesTransacciones` (registro, cancelación y reportes).
3. Poblar `datos/` con ejemplos y crear scripts de prueba.
4. Añadir tests unitarios y casos de prueba para validaciones y persistencia.

Contacto y soporte
------------------

Si quieres, puedo:

- ejecutar una compilación en tu entorno (ya probé compilación localmente y se produjo un ejecutable `inventario.exe`),
- abrir un PR con pruebas iniciales o
- seguir implementando módulos pendientes.

---

Archivo actualizado para reflejar los cambios de Fase 2 → Fase 3. Revisa el contenido y dime si quieres que lo adapte (más detalle técnico, diagramas, o secciones de ejemplo). 

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
- SECCIÓN 1:  CONSTANTES Y NOMBRES DE ARCHIVOS
- SECCIÓN 2:  ESTRUCTURAS BINARIAS
- SECCIÓN 3:  UTILIDADES GENERALES
- SECCIÓN 4:  GESTIÓN DE ARCHIVOS - HEADERS Y OFFSETS
- SECCIÓN 5:  OPERACIONES CRUD GENÉRICAS
- SECCIÓN 6:  VALIDACIONES DE ENTRADA
- SECCIÓN 7:  VALIDACIONES DE UNICIDAD
- SECCIÓN 8:  TIENDA (registro único)
- SECCIÓN 9:  FUNCIONES DE PRODUCTO (MOSTRAR)
- SECCIÓN 10: CRUD DE PRODUCTOS
- SECCIÓN 11: FUNCIONES DE PROVEEDOR (MOSTRAR)
- SECCIÓN 12: CRUD DE PROVEEDORES
- SECCIÓN 13: FUNCIONES DE CLIENTE (MOSTRAR)
- SECCIÓN 14: CRUD DE CLIENTES
- SECCIÓN 15: FUNCIONES DE TRANSACCIÓN (MOSTRAR)
- SECCIÓN 16: CRUD DE TRANSACCIONES
- SECCIÓN 17: REPORTES Y MANTENIMIENTO
- SECCIÓN 18: MENÚ PRINCIPAL
- SECCIÓN 19: MAIN

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

<img src= "https://media.discordapp.net/attachments/1137458996404572283/1484361119463243796/image.png?ex=69bdf284&is=69bca104&hm=62c293a4e2aad861e5cad3e08ae8f74eeb3ab6b71f52c10652cb938659228db1&=&format=webp&quality=lossless&width=406&height=623" width="250">
