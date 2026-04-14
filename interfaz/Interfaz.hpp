#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

// =============================================================================
// Clase Interfaz - Centraliza todos los menus y la navegacion del sistema.
//
// @brief Esta clase se encarga de:
//   - Mostrar el menu principal y submenus de cada modulo.
//   - Leer y validar la opcion seleccionada por el usuario.
//   - Delegar cada accion a las funciones de operacionesXXX.
//   - Contener el loop principal del programa (ejecutar()).
//
// main.cpp SOLO llama a Interfaz::ejecutar(). Nada mas.
// =============================================================================

class Interfaz {
public:
  
    static void ejecutar();

private:
    // Submenus de cada modulo
    static void menuProductos();
    static void menuProveedores();
    static void menuClientes();
    static void menuTransacciones();
    static void menuReportes();
    static void menuConfigurarTienda();

   
    static void mostrarEncabezado();

    // Lee una opcion entera desde cin, retorna -1 si falla
    static int leerOpcion();
};

#endif 