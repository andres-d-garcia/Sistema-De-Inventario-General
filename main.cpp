#include "persistencia/GestorArchivos.hpp"
#include "interfaz/Interfaz.hpp"
#include <iostream>
#include <clocale>

using namespace std;


int main() {
    setlocale(LC_ALL, "");

    if (!GestorArchivos::inicializarSistemaArchivos()) {
        cerr << "ERROR CRITICO: No se pudo inicializar el sistema de archivos." << endl;
        return 1;
    }

    Interfaz::ejecutar();
    return 0;
}
