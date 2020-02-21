#ifndef USUARIOS_H
#define USUARIOS_H

#include <QString>

#include "json.hpp"

using JSON = nlohmann::json;

class usuarios
{
public:
    usuarios(QString nombre, QString correo, QString contra);

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
