#include "GestorArchivos.hpp"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>

// Definir MKDIR portable: _mkdir en Windows, mkdir en Unix
#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#define MKDIR(dir) mkdir((dir), 0755)
#endif

using namespace std;

// =============================================================================
// Inicializacion del sistema
// =============================================================================

bool GestorArchivos::inicializarSistemaArchivos() {
    // Crear carpeta datos/ si no existe
    MKDIR("datos");

    bool ok = true;
    ok &= inicializarArchivo(RUTA_TIENDA);
    ok &= inicializarArchivo(RUTA_PRODUCTOS);
    ok &= inicializarArchivo(RUTA_PROVEEDORES);
    ok &= inicializarArchivo(RUTA_CLIENTES);
    ok &= inicializarArchivo(RUTA_TRANSACCIONES);

    if (ok) {
        cout << "Sistema de archivos inicializado correctamente." << endl;
    }
    return ok;
}

bool GestorArchivos::inicializarArchivo(const char* ruta) {
    // Verificar si ya existe abriendo en modo lectura
    fstream prueba(ruta, ios::in | ios::binary);
    if (prueba.is_open()) {
        prueba.close();
        return true;  // Ya existe, no sobreescribir
    }

    // No existe: crear y escribir header inicial en cero
    fstream archivo(ruta, ios::out | ios::binary);
    if (!archivo.is_open()) {
        setColor(COLOR_ROJO);
        cerr << "ERROR: No se pudo crear el archivo " << ruta << endl;
        setColor(COLOR_RESET);
        return false;
    }

    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoId         = 1;
    header.registrosActivos  = 0;
    header.version           = VERSION_SISTEMA;

    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

// =============================================================================
// Operaciones de Header
// =============================================================================

ArchivoHeader GestorArchivos::leerHeader(const char* ruta) {
    // Valor por defecto si el archivo no se puede abrir
    ArchivoHeader header = {0, 1, 0, VERSION_SISTEMA};

    fstream archivo(ruta, ios::in | ios::binary);
    if (!archivo.is_open()) return header;

    // El header siempre esta en los primeros 16 bytes (offset 0)
    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return header;
}

bool GestorArchivos::actualizarHeader(const char* ruta, ArchivoHeader header) {
    fstream archivo(ruta, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) return false;

    // Sobreescribir SOLO los primeros 16 bytes
    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

// =============================================================================
// Utilidades de backup
// =============================================================================

bool GestorArchivos::copiarArchivo(const char* origen, const char* destino) {
    fstream src(origen, ios::in | ios::binary);
    if (!src.is_open()) return false;

    fstream dst(destino, ios::out | ios::binary);
    if (!dst.is_open()) { src.close(); return false; }

    // Copia byte a byte usando buffer de 4KB
    char buffer[4096];
    while (src.read(buffer, sizeof(buffer))) {
        dst.write(buffer, src.gcount());
    }
    // Escribir los bytes restantes (ultimo bloque incompleto)
    if (src.gcount() > 0) dst.write(buffer, src.gcount());

    src.close();
    dst.close();
    return true;
}

void GestorArchivos::crearBackup() {
    cout << "=== CREAR BACKUP ===" << endl;

    // Generar prefijo con fecha y hora actual
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char prefijo[40];
    strftime(prefijo, sizeof(prefijo), "datos/backup_%Y%m%d_%H%M%S_", lt);

    const char* archivos[] = {
        RUTA_TIENDA, RUTA_PRODUCTOS, RUTA_PROVEEDORES,
        RUTA_CLIENTES, RUTA_TRANSACCIONES
    };
    const int NUM_ARCHIVOS = 5;

    int exitosos = 0;
    for (int i = 0; i < NUM_ARCHIVOS; i++) {
        // Construir nombre del archivo de backup
        // Extraer solo el nombre del archivo (sin "datos/")
        const char* nombreBase = archivos[i] + 6; // Saltar "datos/"
        char destino[100];
        strcpy(destino, prefijo);
        strcat(destino, nombreBase);

        cout << "  Copiando " << archivos[i] << " -> " << destino << " ... ";
        if (copiarArchivo(archivos[i], destino)) {
            setColor(COLOR_VERDE);
            cout << "OK" << endl;
            setColor(COLOR_RESET);
            exitosos++;
        } else {
            setColor(COLOR_ROJO);
            cout << "FALLO (archivo puede no existir)" << endl;
            setColor(COLOR_RESET);
        }
    }

    cout << "----------------------------------------" << endl;
    cout << "Backup completado: " << exitosos << "/" << NUM_ARCHIVOS
         << " archivos copiados." << endl;
}
