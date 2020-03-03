#ifndef USUARIOS_H
#define USUARIOS_H

#include <QString>
#include <QSqlDatabase>

#include "json.hpp"
#include "bbdd.h"

using JSON = nlohmann::json;

class usuarios
{
public:
    QSqlDatabase m_db;
    usuarios(QSqlDatabase&);
    /**
     * @brief Función para añadir un usuario a las actuales
     * @param mensaje JSON del mensaje
     * @param ok Booleano que confirmará si se ha conectado a la base de datos
     * @return
     */
    JSON insertar(bool conex, JSON mensaje);

    /**
     * @brief Función para eliminar un usuario
     * @param mensaje JSON del mensaje
     * @param ok Booleano que confirmará si se ha conectado a la base de datos
     */
    JSON cancelar(bool conex, JSON mensaje);
    /**
     * @brief Función para comprobar si existe el usuario
     * @param ok Booleano que confirmará si se ha conectado a la base de datos
     * @param mensaje Es el mensaje enviado desde el cliente para interactuar con él
     * @return
     */
    JSON revisar(bool conex, JSON mensaje);
    int id;

private:

    QString nombre;
    QString correo;
    QString contra;
};

#endif // USUARIOS_H
