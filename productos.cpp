#include "productos.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <list>

//TRUNCATE TABLE nombre_de_la_tabla RESTART IDENTITY;

/*! \file */

/**
 * @brief Función para añadir una búsqueda a las actuales
 * @param producto Nombre del producto a tratar
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @return
 */
JSON Productos::insertar(bool ok, JSON mensaje)
{
    if (ok)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO productos (nombre_producto, recurso) VALUES (:nombreQuery, :recursoQuery)");
        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["product"]));
        query.bindValue(":recursoQuery", QString::fromStdString(mensaje["web"]));
        query.exec();
        mensaje["action"] = "insert";
        mensaje["error type"] = "null";
        qDebug() << "Insert " << query.lastError().text();
    } else mensaje["error type"] = "failed conection";
    return mensaje;
}

/**
 * @brief Funcion para modificar un producto en búsqueda
 * @param producto Nombre del producto a tratar
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @return
 */
JSON Productos::modificar(bool ok, JSON mensaje)
{
    if (ok)
    {
        QSqlQuery query;
        query.prepare("UPDATE ");// FROM Productos WHERE :nombreTabla == :nombreQuery");
        query.bindValue(":nombreTabla", QString::fromStdString(m_nombreProducto));
        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["product"]));
        query.exec();
        qDebug() << "Update " << query.lastError().text();
    }
    return mensaje;
}

/**
 * @brief Funcion para cancelar la búsqueda de un producto
 * @param producto Nombre del producto a tratar
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @param mensaje Es el mensaje enviado desde el cliente para interactuar con él
 */
JSON Productos::cancelar(bool ok, JSON mensaje)
{
    if (ok)
    {
        QSqlQuery select;            ///El borrado seria un boton que diga eliminar busquedas, despues se abriria una
                                    ///tabla mostrando los productos en busqueda de ese usuario, y este deberia seleccionar un
                                    /// elemento de la lista para despues borrarlo.

        select.prepare("SELECT id_producto, nombre_producto FROM productos WHERE nombre_producto = :nombreproducto");
        select.bindValue(":nombreproducto", QString::fromStdString(mensaje["product"]));
        select.exec();
        std::list<QString> lista; ///Lista donde almacenaré los datos buscados
        while (select.next()) {
            lista.push_back(select.value("nombre_producto").toString());

            qDebug() << select.value("nombre_producto").toString();
        }

        if (lista.size()>1)
        {
            mensaje["action"] = "delete";
            mensaje["errorType"] = "false";
            /// No mostrar en el cliente todos los elementos
            /// Tengo que hacer que el registro de usuarios tenga un registro de sus propios productos
            /// buscados para que cada uno vea solo sus productos

            qDebug() << "Hay mas de un elemento";

            ///LLamar a mensaje javascript para pedir confirmacion
        }

        QSqlQuery query;
        query.prepare("DELETE FROM productos WHERE nombre_producto = :nombreQuery");
        query.bindValue(":nombreQuery", select.value("nombre_producto").toString());
        query.exec();
        qDebug() << "Cancelar " << query.lastError().text();
    }
    return mensaje;
}

/**
 * @brief Funcion para comprobar el estado de un producto
 * @param producto Nombre del producto a tratar
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @param mensaje Es el mensaje enviado desde el cliente para interactuar con él
 * @return
 */
JSON Productos::revisar(bool ok, JSON mensaje) ///Aqui estarian las comprobaciones del estado del producto
{
    if (ok)
    {
        QSqlQuery query;
        query.prepare("SELECT id_producto, nombre_producto, estado_producto, tiempo_transcurrido, recurso FROM productos WHERE nombre_producto = :nombreproducto");
        //query.bindValue(":nombreTabla", QString::fromStdString(m_nombreProducto.c_str()));
        query.bindValue(":nombreproducto", QString::fromStdString(mensaje["product"]));
        qDebug() << query.exec();
        while (query.next()) {
                QString nombre = query.value("nombre_producto").toString();
                QString estado = query.value("estado_producto").toString();
                QString tiempo = query.value("tiempo_transcurrido").toString();
                QString recurso = query.value("recurso").toString();
                qDebug() << nombre << estado << tiempo << recurso;
                mensaje["busqueda"]["nombre"].push_back(nombre.toStdString());
                mensaje["busqueda"]["estado"].push_back(estado.toStdString());
                mensaje["busqueda"]["tiempo"].push_back(tiempo.toStdString());
                mensaje["busqueda"]["recurso"].push_back(recurso.toStdString());
        }

        mensaje["action"] = "select";
                                                    //Hay que hacer comprobacion de errores
        qDebug() << "Select " << query.lastError().text();
        //qDebug() << QString::fromStdString(mensaje.dump());

        return mensaje;
    }
    else return "No se han podido encontrar resultados.";
}

Productos::Productos()
{

}
