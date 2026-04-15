#ifndef TIENDA_HPP
#define TIENDA_HPP

#include <ctime>

// =============================================================================
// Clase Tienda - Representa los datos de la empresa y contadores globales.
// Contiene un unico registro en tienda.bin (indice fisico 0).
// =============================================================================

class Tienda {
    friend class GestorArchivos;
public:
    // -------------------------------------------------------------------------
    // Constructores y destructor
    // -------------------------------------------------------------------------
    Tienda();                        // Constructor por defecto (campos en cero)
    Tienda(const char* nombre,
           const char* rif,
           const char* direccion,
           const char* telefono,
           const char* email);       // Constructor parametrizado
    Tienda(const Tienda& otro);      // Constructor de copia
    ~Tienda();

    // -------------------------------------------------------------------------
    // Getters (const - no modifican el objeto)
    // -------------------------------------------------------------------------
    int         getId()           const;
    const char* getNombre()       const;
    const char* getRif()          const;
    const char* getDireccion()    const;
    const char* getTelefono()     const;
    const char* getEmail()        const;
    int         getTotalProductos()    const;
    int         getTotalProveedores()  const;
    int         getTotalClientes()     const;
    int         getTotalTransacciones() const;
    float       getTotalVentas()   const;
    float       getTotalCompras()  const;
    bool        getEliminado()    const;
    time_t      getFechaCreacion() const;
    time_t      getFechaUltimaModificacion() const;

    // -------------------------------------------------------------------------
    // Setters (con validacion interna)
    // -------------------------------------------------------------------------
    bool setId(int id);
    bool setNombre(const char* nombre);
    bool setRif(const char* rif);
    bool setDireccion(const char* direccion);
    bool setTelefono(const char* telefono);
    bool setEmail(const char* email);
    void setTotalProductos(int total);
    void setTotalProveedores(int total);
    void setTotalClientes(int total);
    void setTotalTransacciones(int total);
    void setTotalVentas(float total);
    void setTotalCompras(float total);
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void actualizarFechaModificacion();   // Asigna time(nullptr) automaticamente

    // -------------------------------------------------------------------------
    // Metodos de negocio
    // -------------------------------------------------------------------------
    void incrementarProductos();
    void decrementarProductos();
    void incrementarProveedores();
    void decrementarProveedores();
    void incrementarClientes();
    void decrementarClientes();
    void agregarVenta(float monto);
    void revertirVenta(float monto);
    void agregarCompra(float monto);
    void revertirCompra(float monto);

    // -------------------------------------------------------------------------
    // Metodos de presentacion
    // -------------------------------------------------------------------------
    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    // -------------------------------------------------------------------------
    // Metodo estatico de utilidad
    // -------------------------------------------------------------------------
    // Retorna sizeof(Tienda) — util para calcular offsets en GestorArchivos
    static int obtenerTamano();

private:
    // Atributos privados (equivalentes al struct del Proyecto 2)
    int   id;
    char  nombre[100];
    char  rif[20];
    char  direccion[200];
    char  telefono[20];
    char  email[100];
    int   totalProductos;
    int   totalProveedores;
    int   totalClientes;
    int   totalTransacciones;
    float totalVentas;
    float totalCompras;
    bool  eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

#endif // TIENDA_HPP
