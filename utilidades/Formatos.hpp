#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <ctime>

// =============================================================================
// Formatos - Metodos estaticos para formateo de salida y utilidades de consola.
// =============================================================================

class Formatos {
public:
    // Formatea un time_t como "DD/MM/AAAA" en el buffer dado
    static void formatearFecha(time_t tiempo, char* buffer, int tamanio);

    // Formatea un float como moneda: "$1,234.56"
    static void formatearMoneda(float valor, char* buffer, int tamanio);

    // Convierte una cadena a minusculas en destino
    static void convertirMinusculas(char* destino, const char* origen);

    // Convierte una cadena a mayusculas en destino
    static void convertirMayusculas(char* destino, const char* origen);

    // Verifica si 'sub' esta contenido en 'str' (sin distincion de mayusculas)
    static bool contieneSubstring(const char* str, const char* sub);

    // Obtiene la fecha actual como "YYYY-MM-DD" en el buffer dado
    static void obtenerFechaActual(char* buffer, int tamanio);

    // Imprime una linea de 'ancho' caracteres 'c'
    static void imprimirLinea(int ancho = 70, char c = '-');

    // Limpia el buffer de cin
    static void limpiarBuffer();

    // Limpia la pantalla (multiplataforma)
    static void limpiarPantalla();

    // Pausa hasta que el usuario presione Enter
    static void pausar();
};

#endif // FORMATOS_HPP
