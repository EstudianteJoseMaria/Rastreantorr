#ifndef PRODUCTOS_H
#define PRODUCTOS_H
#include <QString>
#include <QSqlDatabase>

#include "json.hpp"

using JSON = nlohmann::json;

class Productos
{

private:
    int m_idProducto;

public:
    QSqlDatabase m_db;
    Productos(QSqlDatabase);
    std::string m_nombreProducto {""};
    QString m_fechaRegistro;
    int estado;
    JSON insertar(bool conex, JSON mensaje);
    JSON modificar(bool conex, JSON mensaje);
    JSON cancelar(bool conex, JSON mensaje);
    JSON revisar(bool conex, JSON mensaje);
};

#endif // PRODUCTOS_H
