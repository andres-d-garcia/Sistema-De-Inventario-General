
<img src="https://64.media.tumblr.com/2e317ca5b9fb13feebde242e45f24172/73f0c4d120734f31-c8/s2048x3072/1fc0b91846abd27352cd497226c3aa07b6bd6a88.pnj">

<div><h1><img src= "https://64.media.tumblr.com/40936113bcf631fdeeda308c7afc3642/d5b3859a68122916-75/s100x200/aa3229ee2858071816fb6b5d6673cb31065385cf.gifv" width="50"><strong>Descripción del Proyecto</strong></div></h1>

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
  ├─ operacionesProductos.cpp
  └─ operacionesProductos.hpp
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

# Sistema de Inventario - Distribución y build

Resumen breve
- Ejecutable recomendado: `inventario.exe` (ya generado en este repositorio).
- Carpeta de datos: `datos/` — contiene los archivos binarios usados por el programa.

Ejecución (Windows)
- Copia `inventario.exe` y la carpeta `datos/` a la máquina destino.
- Si usaste MinGW, puede que necesites incluir las DLLs: `libstdc++-6.dll`, `libgcc_s_seh-1.dll`, `libwinpthread-1.dll`.
- Ejecuta desde PowerShell o CMD:

```
./inventario.exe
```

Distribución portátil recomendada
- Crear un ZIP que contenga:
  - `inventario.exe`
  - Las DLLs necesarias (si aplican)
  - Carpeta `datos/` (vacía o con archivos iniciales)
  - Este `README.md` con instrucciones

Compilar desde el código
- Requisitos: MinGW/GCC o similar (Windows) o GCC en Linux/macOS.
- Opciones:
  - Usar el Makefile (si tienes `make`):

    ```
    make all
    ```

  - Compilar manualmente (PowerShell example):

    ```powershell
    g++ (Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }) -Wall -Wextra -g3 -finput-charset=UTF-8 -fexec-charset=UTF-8 -o inventario.exe
    ```

  - Desde VS Code: la tarea por defecto `Compilar proyecto (g++)` compila todo y genera `inventario.exe`.

Estático vs Dinámico
- Para evitar depender de DLLs, puedes intentar enlazar estáticamente (aumenta tamaño):

```
g++ ... -static -static-libgcc -static-libstdc++ -o inventario.exe
```

Advertencias y notas
- Si compartes sólo el `.exe`, algunos usuarios pueden necesitar las DLLs o permisos de ejecución.
- Si quieres que otros compilen, documenta la versión de GCC/MinGW recomendada.

Contribuir
- Pull requests y issues son bienvenidos. Para compilar localmente revisa `Makefile` y `.vscode/tasks.json`.

Licencia
- Incluye la licencia del proyecto si aplica (no incluida aquí por defecto).

Estado actual del repositorio
-----------------------------

- La estructura modular y las cabeceras de las clases han sido añadidas según Fase 3.
- Se centralizó la persistencia y se definieron plantillas y constantes en `persistencia/Constantes.hpp` y `GestorArchivos.hpp` (implementación parcial según desarrollo).
- Algunos módulos contienen implementaciones mínimas o `stubs` en `transacciones/` y otros archivos para permitir compilación y pruebas iniciales; la lógica completa de negocio puede necesitar más trabajo (ver comentarios en los archivos correspondientes).
- `Makefile`: versión más portable y preparada para entornos MinGW/TDM (ver [Makefile](Makefile)).



