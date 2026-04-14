#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

#include <cstdio>
#include <sys/stat.h>

// =============================================================================
// Constantes globales del sistema
// =============================================================================

// --- Rutas de archivos binarios ---
const char* const RUTA_TIENDA        = "datos/tienda.bin";
const char* const RUTA_PRODUCTOS     = "datos/productos.bin";
const char* const RUTA_PROVEEDORES   = "datos/proveedores.bin";
const char* const RUTA_CLIENTES      = "datos/clientes.bin";
const char* const RUTA_TRANSACCIONES = "datos/transacciones.bin";

// --- Limites de arreglos fijos (deben coincidir con los campos de las clases) ---
const int MAX_PRODUCTOS_POR_PROVEEDOR   = 100;
const int MAX_TRANSACCIONES_POR_ENTIDAD = 200;
const int MAX_ITEMS_POR_TRANSACCION     = 50;

// --- Version del sistema de archivos ---
const int VERSION_SISTEMA = 1;

// =============================================================================
// Colores de consola multiplataforma
// =============================================================================

#ifdef _WIN32
    #include <windows.h>

    // Codigos de color para Windows (SetConsoleTextAttribute)
    #define COLOR_RESET    7
    #define COLOR_ROJO     12
    #define COLOR_VERDE    10
    #define COLOR_AZUL     9
    #define COLOR_AMARILLO 14
    #define COLOR_CYAN     11
    #define COLOR_MAGENTA  13
    #define COLOR_BLANCO   15
    #define COLOR_GRIS     8

    inline void setColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

#else
    // Codigos ANSI para Linux / macOS
    #define COLOR_RESET    0
    #define COLOR_ROJO     31
    #define COLOR_VERDE    32
    #define COLOR_AZUL     34
    #define COLOR_AMARILLO 33
    #define COLOR_CYAN     36
    #define COLOR_MAGENTA  35
    #define COLOR_BLANCO   37
    #define COLOR_GRIS     90

    inline void setColor(int color) {
        if (color == COLOR_RESET)
            printf("\033[0m");
        else
            printf("\033[%dm", color);
    }

#endif

#endif // CONSTANTES_HPP
