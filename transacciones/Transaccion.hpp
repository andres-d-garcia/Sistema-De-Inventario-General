#ifndef TRANSACCION_HPP
#define TRANSACCION_HPP

#include <ctime>
#include "../persistencia/Constantes.hpp"

class ItemTransaccion {
public:
    ItemTransaccion();
    ItemTransaccion(int idProducto, int cantidad, float precioUnitario);

    int   getIdProducto()     const;
    int   getCantidad()       const;
    float getPrecioUnitario() const;
    float getSubtotal()       const;

    bool setIdProducto(int id);
    bool setCantidad(int cantidad);
    bool setPrecioUnitario(float precio);

    void mostrar() const;

private:
    int   idProducto;
    int   cantidad;
    float precioUnitario;
    float subtotal;
};

class Transaccion {
    friend class GestorArchivos;
public:
    Transaccion();
    Transaccion(const char* tipo, int idCliente, int idProveedor);
    Transaccion(const Transaccion& otro);
    ~Transaccion();

    int         getId()            const;
    const char* getTipo()          const;
    int         getIdCliente()     const;
    int         getIdProveedor()   const;
    int         getCantidadItems() const;
    float       getTotal()         const;
    const char* getFecha()         const;
    const char* getDescripcion()   const;
    bool        getEliminado()     const;
    time_t      getFechaRegistro() const;
    time_t      getFechaUltimaModificacion() const;

    const ItemTransaccion& getItem(int posicion) const;

    bool setId(int id);
    bool setTipo(const char* tipo);
    bool setIdCliente(int id);
    bool setIdProveedor(int id);
    bool setDescripcion(const char* desc);
    void setFecha(const char* fecha);
    void setEliminado(bool eliminado);
    void setFechaRegistro(time_t fecha);
    void actualizarFechaModificacion();

    bool agregarItem(const ItemTransaccion& item);
    void recalcularTotal();

    bool esVenta()  const;
    bool esCompra() const;

    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();

private:
    int              id;
    char             tipo[10];
    int              idCliente;
    int              idProveedor;
    ItemTransaccion  items[MAX_ITEMS_POR_TRANSACCION];
    int              cantidadItems;
    float            total;
    char             fecha[11];
    char             descripcion[200];
    bool             eliminado;
    time_t           fechaRegistro;
    time_t           fechaUltimaModificacion;
};

#endif