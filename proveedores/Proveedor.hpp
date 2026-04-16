#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include <ctime>
#include "../persistencia/Constantes.hpp"

class Proveedor {
    friend class GestorArchivos;
public:
    Proveedor();
    Proveedor(const char* nombre, const char* identificacion,
              const char* telefono, const char* email, const char* direccion);
    Proveedor(const Proveedor& otro);
    ~Proveedor();

    int         getId()             const;
    const char* getNombre()         const;
    const char* getDireccion()      const;
    const char* getTelefono()       const;
    const char* getEmail()          const;
    const char* getIdentificacion() const;
    int         getCantidadProductos() const;
    bool        getEliminado()      const;
    time_t      getFechaRegistro()  const;
    time_t      getFechaUltimaModificacion() const;

    int getProductoId(int posicion) const;

    bool setId(int id);
    bool setNombre(const char* nombre);
    bool setDireccion(const char* direccion);
    bool setTelefono(const char* telefono);
    bool setEmail(const char* email);
    bool setIdentificacion(const char* identificacion);
    void setEliminado(bool eliminado);
    void setFechaRegistro(time_t fecha);
    void actualizarFechaModificacion();

    bool agregarProductoId(int idProducto);   
    bool eliminarProductoId(int idProducto);  

    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();

private:
    int   id;
    char  nombre[100];
    char  direccion[200];
    char  telefono[20];
    char  email[100];
    char  identificacion[20];
    int   productosIDs[MAX_PRODUCTOS_POR_PROVEEDOR];
    int   cantidadProductos;
    bool  eliminado;
    time_t fechaRegistro;
    time_t fechaUltimaModificacion;
};

#endif 
