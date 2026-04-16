#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

// =============================================================================
// Clase Interfaz - Centraliza todos los menus y la navegacion del sistema.
// =============================================================================

class Interfaz {
public:
  
    static void ejecutar();

private:
    static void menuProductos();
    static void menuProveedores();
    static void menuClientes();
    static void menuTransacciones();
    static void menuReportes();
    static void menuConfigurarTienda();

   
    static void mostrarEncabezado();

    static int leerOpcion();
};

#endif 