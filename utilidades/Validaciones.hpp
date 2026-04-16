#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

// =============================================================================
// Validaciones - Metodos estaticos para validar formatos de datos.
//
// DIFERENCIA CLAVE con los setters de las clases:
//   - Validaciones: reglas generales de formato. ej: "¿es un email valido?"
//   - Setters de entidad: reglas de negocio especificas. ej: "¿precio > 0?"
// =============================================================================

class Validaciones {
public:
    // Valida que el email tenga '@' y '.' despues del '@'
    static bool validarEmail(const char* email);

    // Valida formato de RIF venezolano: J-12345678-2 (12 caracteres)
    static bool validarRIF(const char* rif);

    // Valida formato de cedula: V-12345678 o E-12345678
    static bool validarCedula(const char* cedula);

    // Valida formato de fecha: YYYY-MM-DD
    static bool validarFecha(const char* fecha);

    // Valida que un entero este dentro de un rango [min, max]
    static bool validarRango(int valor, int min, int max);

    // Lee y valida un string desde cin. Retorna false si esta vacio o es "cancelar"
    static bool leerCadena(const char* mensaje, char* destino, int tamanio);

    // Lee y valida un entero desde cin. permitirNegativos por defecto es false.
    static bool leerEntero(const char* mensaje, int& destino,
                           bool permitirNegativos = false);

    // Lee y valida un float desde cin. Rechaza valores negativos.
    static bool leerFlotante(const char* mensaje, float& destino);
};

#endif // VALIDACIONES_HPP
