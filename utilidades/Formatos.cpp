#include "Formatos.hpp"
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <limits>

using namespace std;

void Formatos::formatearFecha(time_t tiempo, char* buffer, int tamanio) {
    tm* lt = localtime(&tiempo);
    strftime(buffer, tamanio, "%d/%m/%Y", lt);
}

void Formatos::formatearMoneda(float valor, char* buffer, int tamanio) {
    snprintf(buffer, tamanio, "$%.2f", valor);
}

void Formatos::convertirMinusculas(char* destino, const char* origen) {
    int i = 0;
    while (origen[i]) { destino[i] = tolower(origen[i]); i++; }
    destino[i] = '\0';
}

void Formatos::convertirMayusculas(char* destino, const char* origen) {
    int i = 0;
    while (origen[i]) { destino[i] = toupper(origen[i]); i++; }
    destino[i] = '\0';
}

bool Formatos::contieneSubstring(const char* str, const char* sub) {
    char a[300], b[300];
    convertirMinusculas(a, str);
    convertirMinusculas(b, sub);
    return strstr(a, b) != nullptr;
}

void Formatos::obtenerFechaActual(char* buffer, int tamanio) {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    strftime(buffer, tamanio, "%Y-%m-%d", lt);
}

void Formatos::imprimirLinea(int ancho, char c) {
    for (int i = 0; i < ancho; i++) cout << c;
    cout << endl;
}

void Formatos::limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Formatos::limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Formatos::pausar() {
    cout << "\nPresione Enter para continuar...";
    // Si hay algo en el buffer lo descartamos primero
    if (cin.peek() != '\n') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.get();
}
