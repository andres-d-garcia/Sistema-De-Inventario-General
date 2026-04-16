#include "Validaciones.hpp"
#include <iostream>
#include <cstring>
#include <cctype>
#include <limits>
#include <string>

using namespace std;

// =============================================================================
// Validaciones de formato
// =============================================================================

bool Validaciones::validarEmail(const char* email) {
    int len = strlen(email);
    if (len < 5 || len > 100) {
        cout << "ERROR: Email debe tener entre 5 y 100 caracteres." << endl;
        return false;
    }
    const char* arroba = strchr(email, '@');
    if (!arroba || arroba == email) {
        cout << "ERROR: Email debe contener '@' y no puede estar al inicio." << endl;
        return false;
    }
    const char* punto = strchr(arroba + 1, '.');
    if (!punto || punto == arroba + 1) {
        cout << "ERROR: Email debe contener '.' despues del '@'." << endl;
        return false;
    }
    if (email[len - 1] == '.') {
        cout << "ERROR: Email no puede terminar con punto." << endl;
        return false;
    }
    if (strchr(email, ' ') != nullptr) {
        cout << "ERROR: Email no puede contener espacios." << endl;
        return false;
    }
    return true;
}

bool Validaciones::validarRIF(const char* rif) {
    int len = strlen(rif);
    if (len != 12) {
        cout << "ERROR: RIF debe tener formato J-12345678-2 (12 caracteres)." << endl;
        return false;
    }
    char tipo = toupper(rif[0]);
    if (!strchr("JGVEPCD", tipo)) {
        cout << "ERROR: Tipo de RIF invalido. Debe ser J, G, V, E, P, C o D." << endl;
        return false;
    }
    if (rif[1] != '-' || rif[10] != '-') {
        cout << "ERROR: Guiones deben estar en las posiciones 1 y 10." << endl;
        return false;
    }
    for (int i = 2; i <= 9; i++) {
        if (!isdigit(rif[i])) {
            cout << "ERROR: Posiciones 2-9 del RIF deben ser digitos." << endl;
            return false;
        }
    }
    char verificador = toupper(rif[11]);
    if (!isdigit(verificador) && verificador != 'K') {
        cout << "ERROR: Digito verificador debe ser 0-9 o K." << endl;
        return false;
    }
    return true;
}

bool Validaciones::validarCedula(const char* cedula) {
    int len = strlen(cedula);
    if (len < 10 || len > 13) {
        cout << "ERROR: Cedula debe tener entre 10 y 13 caracteres (V-12345678)." << endl;
        return false;
    }
    char tipo = toupper(cedula[0]);
    if (tipo != 'V' && tipo != 'E') {
        cout << "ERROR: Cedula debe comenzar con V (Venezolano) o E (Extranjero)." << endl;
        return false;
    }
    if (cedula[1] != '-') {
        cout << "ERROR: Formato invalido. Ejemplo: V-12345678" << endl;
        return false;
    }
    for (int i = 2; i < len; i++) {
        if (!isdigit(cedula[i])) {
            cout << "ERROR: Solo digitos despues del tipo." << endl;
            return false;
        }
    }
    return true;
}

bool Validaciones::validarFecha(const char* fecha) {
    if (strlen(fecha) != 10 || fecha[4] != '-' || fecha[7] != '-') {
        cout << "ERROR: Formato de fecha debe ser YYYY-MM-DD." << endl;
        return false;
    }
    int mes = atoi(string(fecha + 5, 2).c_str());
    int dia = atoi(string(fecha + 8, 2).c_str());
    if (mes < 1 || mes > 12 || dia < 1 || dia > 31) {
        cout << "ERROR: Fecha fuera de rango." << endl;
        return false;
    }
    return true;
}

bool Validaciones::validarRango(int valor, int min, int max) {
    return valor >= min && valor <= max;
}

// =============================================================================
// Lectura validada desde cin
// =============================================================================

bool Validaciones::leerCadena(const char* mensaje, char* destino, int tamanio) {
    cout << mensaje;
    cin.getline(destino, tamanio);
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ERROR: Entrada demasiado larga." << endl;
        return false;
    }
    if (strlen(destino) == 0) {
        cout << "ERROR: El campo no puede estar vacio." << endl;
        return false;
    }
    if (strcmp(destino, "cancelar") == 0) {
        cout << "Operacion cancelada." << endl;
        return false;
    }
    return true;
}

bool Validaciones::leerEntero(const char* mensaje, int& destino,
                               bool permitirNegativos) {
    cout << mensaje;
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (input == "cancelar") {
        cout << "Operacion cancelada." << endl;
        return false;
    }
    try {
        destino = stoi(input);
        if (!permitirNegativos && destino < 0) {
            cout << "ERROR: El valor no puede ser negativo." << endl;
            return false;
        }
        return true;
    } catch (...) {
        cout << "ERROR: Ingrese un numero entero valido." << endl;
        return false;
    }
}

bool Validaciones::leerFlotante(const char* mensaje, float& destino) {
    cout << mensaje;
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (input == "cancelar") {
        cout << "Operacion cancelada." << endl;
        return false;
    }
    try {
        destino = stof(input);
        if (destino < 0) {
            cout << "ERROR: El valor no puede ser negativo." << endl;
            return false;
        }
        return true;
    } catch (...) {
        cout << "ERROR: Ingrese un numero valido." << endl;
        return false;
    }
}
