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

![caso](https://cdn.discordapp.com/attachments/1466517733909729312/1475971176097382421/image.png?ex=69a20fc6&is=69a0be46&hm=d010a2c5d8129a56b5187388a0e08cfeef0a9560aaddc717062f02238d360cda&)
![caso](https://cdn.discordapp.com/attachments/1466517733909729312/1475971978220404871/Screenshot_2026-02-24_174427.png?ex=69a21085&is=69a0bf05&hm=b4195b91169f121b2af43441812e8d8499b6463678fbd8a3e85364d9d994ce51&)
![caso](https://cdn.discordapp.com/attachments/1466517733909729312/1475975504044621844/image.png?ex=69a213ce&is=69a0c24e&hm=b178882cde1eef21a3053fbf6f77f61fef9348db55219892382431d049aba4ce&)
![caso](https://cdn.discordapp.com/attachments/1466517733909729312/1475975803656470618/image.png?ex=69a21415&is=69a0c295&hm=a857fcf29198775981e8e46b550e3321a14e6b9742f995ad3697e3f3068a5fae&)


### Caso 2: Validación de Unicidad

![caso2](https://cdn.discordapp.com/attachments/1466517733909729312/1475982310691049484/image.png?ex=69a21a25&is=69a0c8a5&hm=adb1ca33955ef9dadf171fc0b9a1f1a0d35e599193c10f15e3b8add24ea58ba6&)
![caso2](https://cdn.discordapp.com/attachments/1466517733909729312/1475982545890840857/image.png?ex=69a21a5d&is=69a0c8dd&hm=a7e2ab1eefe0bfd1e353fcc96ac9171c5e4953a70e527e30555c8a7f3c5ec757&)


### Caso 3: Búsqueda

![caso3](https://cdn.discordapp.com/attachments/1466517733909729312/1475987659502391376/image.png?ex=69a17660&is=69a024e0&hm=f87ac8a4cd5e7f0b9234b6ef500ce5ea7a52c7d57a1ca2386ec0c4a0c7ff5e51&)
![caso3](https://cdn.discordapp.com/attachments/1466517733909729312/1475990052902273147/image.png?ex=69a1789b&is=69a0271b&hm=0e6a648a7f35600d70b803f3d17a6e17a65cc556c826e3e94283777653bc1077&)
![caso3](https://cdn.discordapp.com/attachments/1466517733909729312/1476000799124488336/image.png?ex=69a1829d&is=69a0311d&hm=9d8edd0c452700e26cd14cf9bdcbba868a8a9347e7cad95ec3982675afcb2b1e&)


### Caso 4: Actualización de Producto

![caso4](https://cdn.discordapp.com/attachments/1466517733909729312/1476722416402169987/Screenshot_2026-02-26_192721.png?ex=69a2286c&is=69a0d6ec&hm=bb9a3660c3bf05f2d5e0f179137f99d18a91b3196575f3ed782dc6a59891309f&)
![caso4](https://media.discordapp.net/attachments/1466517733909729312/1476722416884383835/Screenshot_2026-02-26_192643.png?ex=69a2286c&is=69a0d6ec&hm=a12c295f2b429eacc121293d9891545620532d698f355cff5227272e30c36a86&=&format=webp&quality=lossless&width=733&height=511)

### Caso 5: Transacciones

![caso5](https://cdn.discordapp.com/attachments/1466517733909729312/1476723505570517182/image.png?ex=69a2296f&is=69a0d7ef&hm=9057c496fbf24b8f8423c1b6f7d22957fa052d437598a02e7eb3c939433a4748&)
![caso5](https://cdn.discordapp.com/attachments/1466517733909729312/1476723889676619930/image.png?ex=69a229cb&is=69a0d84b&hm=4343b8a724607f59575a65d3d702d233e71010f3c9b326ad81181b33de4c1b84&)
![caso5](https://cdn.discordapp.com/attachments/1466517733909729312/1476725803063709818/image.png?ex=69a22b93&is=69a0da13&hm=7b9d271233f7331e5fbe9f918fbbe5226c6c99b2be8e87c5070dbca268021615&)
![caso5](https://cdn.discordapp.com/attachments/1466517733909729312/1476726371332915242/image.png?ex=69a22c1b&is=69a0da9b&hm=89e8c8c4deb3a2449297649bb1e840dd3fba553b52ee542dad8d87c8523178ca&)
![caso5](https://cdn.discordapp.com/attachments/1466517733909729312/1476726399304990851/Screenshot_2026-02-26_194225.png?ex=69a22c21&is=69a0daa1&hm=b312c93eca1163e18f25af21817d2ede30c4141a6251b839238de1333b97a18b&)


### Caso 6: Validación de Stock en Ventas

![caso6](https://cdn.discordapp.com/attachments/1466517733909729312/1476727026206511316/image.png?ex=69a22cb7&is=69a0db37&hm=ce54ecd368e7f67c9a0b5d421386e95e763e0b22ed71b4f20f1e79c49634e875&)
![caso6](https://cdn.discordapp.com/attachments/1466517733909729312/1476727499701616712/image.png?ex=69a22d28&is=69a0dba8&hm=7b5d026b1f9c3f50457462f2f8fbb79e2cebf7c4a4960b380e04e10fb2a5c613&)


### Caso 7: Cancelación de Transacción con Verificación

![caso7](https://cdn.discordapp.com/attachments/1466517733909729312/1476728023419064390/Screenshot_2026-02-26_194918.png?ex=69a22da5&is=69a0dc25&hm=4d5dd3ca75c9a5edd4e96bdeae728f208bd91782f55934f0122df95c9888dd1a&)
![caso7](https://cdn.discordapp.com/attachments/1466517733909729312/1476728023708602471/Screenshot_2026-02-26_194910.png?ex=69a22da5&is=69a0dc25&hm=811f8abad44b2f117a7f3fc46204d38ed616e9ea22b8f6c10b390bd920bd2979&)
