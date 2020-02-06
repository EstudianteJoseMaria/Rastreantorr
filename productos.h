#ifndef PRODUCTOS_H
#define PRODUCTOS_H
#include <QString>

#include "json.hpp"

using JSON = nlohmann::json;

class Productos
{

private:
    int m_idProducto;

public:
    Productos();
    std::string m_nombreProducto {""};
    QString m_fechaRegistro;
    int estado;
    JSON agregar(bool conex, JSON mensaje);
    JSON modificar(bool conex, JSON mensaje);
    JSON cancelar(bool conex, JSON mensaje);
    JSON revisar(bool conex, JSON mensaje);
};

#endif // PRODUCTOS_H
