#ifndef USUARIOS_H
#define USUARIOS_H

#include <QString>
#include <QSqlDatabase>

#include "json.hpp"

using JSON = nlohmann::json;

class usuarios
{
public:
    QSqlDatabase m_db;
    usuarios(QSqlDatabase);
    JSON insertar(bool conex, JSON mensaje);
    JSON modificar(bool conex, JSON mensaje);
    JSON cancelar(bool conex, JSON mensaje);
    JSON revisar(bool conex, JSON mensaje);
    int id;

private:

    QString nombre;
    QString correo;
    QString contra;
};

#endif // USUARIOS_H
