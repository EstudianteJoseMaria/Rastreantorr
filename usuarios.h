#ifndef USUARIOS_H
#define USUARIOS_H

#include <QString>

#include "json.hpp"

using JSON = nlohmann::json;

class usuarios
{
public:
    usuarios();//std::string nombre, std::string mail, std::string contra

    JSON insertar(bool conex, JSON mensaje);
    JSON modificar(bool conex, JSON mensaje);
    JSON cancelar(bool conex, JSON mensaje);
    JSON revisar(bool conex, JSON mensaje);

private:
    int id;
    std::string nombre;
    std::string mail;
    std::string contra;
};

#endif // USUARIOS_H
