#ifndef PRODUCTOS_H
#define PRODUCTOS_H
#include <QString>
#include <QSqlDatabase>

#include "bbdd.h"
#include "json.hpp"

using JSON = nlohmann::json;

class Productos
{

private:
    int m_idProducto;

public:
    QSqlDatabase m_db;
    Productos(QSqlDatabase&);
    std::string m_nombreProducto {""};
    QString m_fechaRegistro;
    int estado;
    /**
     * @brief Función para añadir una búsqueda a las actuales
     * @param producto Nombre del producto a tratar
     * @param ok Booleano que confirmará si se ha conectado a la base de datos
     * @return
     */
    JSON insertar(bool conex, JSON mensaje);
    /**
     * @brief Función para modificar un producto en búsqueda
     * @param producto Nombre del producto a tratar
     * @param ok Booleano que confirmará si se ha conectado a la base de datos
     * @return
     */
    JSON modificar(bool conex, JSON mensaje);
    /**
     * @brief Función para cancelar la búsqueda de un producto
     * @param producto Nombre del producto a tratar
     * @param ok Booleano que confirmará si se ha conectado a la base de datos
     * @param mensaje Es el mensaje enviado desde el cliente para interactuar con él
     */
    JSON cancelar(bool conex, JSON mensaje);

    /**
     * @brief Función para comprobar el estado de un producto
     * @param producto Nombre del producto a tratar
     * @param ok Booleano que confirmará si se ha conectado a la base de datos
     * @param mensaje Es el mensaje enviado desde el cliente para interactuar con él
     * @return Devuelve el mensaje JSON que recibirá el cliente
     */
    JSON revisar(bool conex, JSON mensaje);
};

#endif // PRODUCTOS_H
