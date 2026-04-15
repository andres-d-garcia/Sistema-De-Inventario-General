#ifndef GESTOR_ARCHIVOS_HPP
#define GESTOR_ARCHIVOS_HPP

// =============================================================================
// GestorArchivos - Centraliza TODAS las operaciones de archivos binarios.
// =============================================================================

#include <fstream>
#include <cstring>
#include <ctime>
#include "Constantes.hpp"

// -----------------------------------------------------------------------------
// ArchivoHeader: metadata al inicio de cada archivo binario (16 bytes fijos)
// -----------------------------------------------------------------------------
struct ArchivoHeader {
    int cantidadRegistros;  
    int proximoId;          
    int registrosActivos;   
    int version;            
};

// -----------------------------------------------------------------------------
// GestorArchivos: todos sus metodos son estaticos, no requiere instancia.
// -----------------------------------------------------------------------------
class GestorArchivos {
public:

    // -------------------------------------------------------------------------
    // Inicializacion del sistema
    // -------------------------------------------------------------------------

    static bool inicializarSistemaArchivos();

    static bool inicializarArchivo(const char* ruta);

    // -------------------------------------------------------------------------
    // Operaciones de Header
    // -------------------------------------------------------------------------

    static ArchivoHeader leerHeader(const char* ruta);

    static bool actualizarHeader(const char* ruta, ArchivoHeader header);

    // -------------------------------------------------------------------------
    // CRUD generico con templates
    // -------------------------------------------------------------------------

    template<typename T>
    static int guardarRegistro(const char* ruta, T& registro) {
        ArchivoHeader header = leerHeader(ruta);
        int idAsignado = header.proximoId;

        registro.id = idAsignado;

        std::fstream archivo(ruta, std::ios::in | std::ios::out | std::ios::binary);
        if (!archivo.is_open()) return -1;

        long offset = calcularOffset<T>(header.cantidadRegistros);
        archivo.seekp(offset, std::ios::beg);

        archivo.write(reinterpret_cast<char*>(&registro), sizeof(T));
        archivo.close();

        header.cantidadRegistros++;
        header.proximoId++;
        if (!registro.eliminado) header.registrosActivos++;
        actualizarHeader(ruta, header);

        return idAsignado;
    }

    template<typename T>
    static bool leerRegistroPorIndice(const char* ruta, int indiceFisico, T& destino) {
        std::fstream archivo(ruta, std::ios::in | std::ios::binary);
        if (!archivo.is_open()) return false;

        long offset = calcularOffset<T>(indiceFisico);
        archivo.seekg(offset, std::ios::beg);
        archivo.read(reinterpret_cast<char*>(&destino), sizeof(T));

        bool ok = !archivo.fail();
        archivo.close();
        return ok;
    }

    template<typename T>
    static bool actualizarRegistro(const char* ruta, int indiceFisico, T& registro) {
        std::fstream archivo(ruta, std::ios::in | std::ios::out | std::ios::binary);
        if (!archivo.is_open()) return false;

        long offset = calcularOffset<T>(indiceFisico);
        archivo.seekp(offset, std::ios::beg);
        archivo.write(reinterpret_cast<char*>(&registro), sizeof(T));

        archivo.close();
        return true;
    }

    template<typename T>
    static int buscarIndicePorId(const char* ruta, int idBuscado) {
        ArchivoHeader header = leerHeader(ruta);
        if (header.cantidadRegistros == 0) return -1;

        std::fstream archivo(ruta, std::ios::in | std::ios::binary);
        if (!archivo.is_open()) return -1;

        T registro;
        for (int i = 0; i < header.cantidadRegistros; i++) {
            long offset = calcularOffset<T>(i);
            archivo.seekg(offset, std::ios::beg);
            archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

            if (!archivo.fail() && registro.id == idBuscado && !registro.eliminado) {
                archivo.close();
                return i;
            }
        }

        archivo.close();
        return -1;
    }

    template<typename T>
    static bool borradoLogico(const char* ruta, int indiceFisico) {
        T registro;
        if (!leerRegistroPorIndice<T>(ruta, indiceFisico, registro)) return false;

        registro.eliminado = true;
        registro.fechaUltimaModificacion = time(nullptr);

        if (!actualizarRegistro<T>(ruta, indiceFisico, registro)) return false;

        ArchivoHeader header = leerHeader(ruta);
        if (header.registrosActivos > 0) header.registrosActivos--;
        actualizarHeader(ruta, header);

        return true;
    }

    // -------------------------------------------------------------------------
    // Utilidades de backup
    // -------------------------------------------------------------------------

    static bool copiarArchivo(const char* origen, const char* destino);

    static void crearBackup();

private:

    
    template<typename T>
    static long calcularOffset(int indiceFisico) {
        return (long)sizeof(ArchivoHeader) + (long)(indiceFisico * (long)sizeof(T));
    }
};

#endif // GESTOR_ARCHIVOS_HPP
