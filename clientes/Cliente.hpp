#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <ctime>
#include "../persistencia/Constantes.hpp"

class Cliente {
    friend class GestorArchivos;
public:
    Cliente();
    Cliente(const char* nombre, const char* identificacion,
            const char* telefono, const char* email, const char* direccion);
    Cliente(const Cliente& otro);
    ~Cliente(); // Destructor para liberar recursos si es necesario

    int         getId()             const;
    const char* getNombre()         const;
    const char* getDireccion()      const;
    const char* getTelefono()       const;
    const char* getEmail()          const;
    const char* getIdentificacion() const;
    int         getCantidadCompras() const;
    float       getTotalGastado()   const;
    bool        getEliminado()      const;
    time_t      getFechaRegistro()  const;
    time_t      getFechaUltimaModificacion() const;
    int         getCompraId(int posicion) const;

    bool setId(int id);
    bool setNombre(const char* nombre);
    bool setDireccion(const char* direccion);
    bool setTelefono(const char* telefono);
    bool setEmail(const char* email);
    bool setIdentificacion(const char* identificacion);
    void setTotalGastado(float total);
    void setEliminado(bool eliminado);
    void setFechaRegistro(time_t fecha);
    void actualizarFechaModificacion();

    bool  agregarCompraId(int idTransaccion);
    void  registrarCompra(int idTransaccion, float monto);
    void  revertirCompra(float monto);

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
    int   comprasIDs[MAX_TRANSACCIONES_POR_ENTIDAD];
    int   cantidadCompras;
    float totalGastado;
    bool  eliminado;
    time_t fechaRegistro;
    time_t fechaUltimaModificacion;
};

#endif 
