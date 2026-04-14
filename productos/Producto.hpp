#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include <ctime>
#include "../persistencia/Constantes.hpp"

// =============================================================================
// Clase Producto - Representa un producto del inventario.
// =============================================================================

class Producto {
    friend class GestorArchivos;
public:
    Producto();
    Producto(const char* codigo, const char* nombre, const char* descripcion,
             int idProveedor, float precio, int stock, int stockMinimo);
    Producto(const Producto& otro);
    ~Producto();

    // -------------------------------------------------------------------------
    // Getters
    // -------------------------------------------------------------------------
    int         getId()           const;
    const char* getCodigo()       const;
    const char* getNombre()       const;
    const char* getDescripcion()  const;
    float       getPrecio()       const;
    int         getStock()        const;
    int         getIdProveedor()  const;
    int         getStockMinimo()  const;
    int         getTotalVendidos() const;
    bool        getEliminado()    const;
    time_t      getFechaCreacion() const;
    time_t      getFechaUltimaModificacion() const;

    // -------------------------------------------------------------------------
    // Setters con validacion
    // -------------------------------------------------------------------------
    bool setId(int id);
    bool setCodigo(const char* codigo);
    bool setNombre(const char* nombre);
    bool setDescripcion(const char* descripcion);
    bool setPrecio(float precio);           
    bool setStock(int stock);               
    bool setIdProveedor(int idProveedor);
    bool setStockMinimo(int stockMinimo);
    void setTotalVendidos(int total);
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void actualizarFechaModificacion();

    // -------------------------------------------------------------------------
    // Metodos de negocio
    // -------------------------------------------------------------------------
    bool tieneStockCritico()       const;   // stock <= stockMinimo
    bool tieneStockSuficiente(int cantidad) const;
    void vender(int cantidad);              
    void reabastecer(int cantidad);         
    void revertirVenta(int cantidad);       

    // -------------------------------------------------------------------------
    // Metodos de presentacion
    // -------------------------------------------------------------------------
    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();

private:
    int   id;
    char  codigo[20];
    char  nombre[100];
    char  descripcion[200];
    float precio;
    int   stock;
    int   idProveedor;
    int   stockMinimo;
    int   totalVendidos;
    bool  eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

#endif // PRODUCTO_HPP
