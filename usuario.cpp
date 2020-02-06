#include "usuario.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <list>

/*! \file */

/**
 * @brief Función para añadir un usuario a las actuales
 * @param mensaje JSON del mensaje
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @return
 */
JSON Usuario::agregar(bool ok, JSON mensaje)
{
    if (ok)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO usuarios (nombre_usuario) VALUES (:nombreQuery)");
        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["product"]));
        query.exec();
        mensaje["action"] = "insert";
        mensaje["error type"] = "null";
        qDebug() << "Insert " << query.lastError().text();
    } else mensaje["error type"] = "failed conection";
    return mensaje;
}

/**
 * @brief Funcion para modificar un usuario existente
 * @param mensaje JSON del mensaje
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @return
 */
JSON Usuario::modificar(bool ok, JSON mensaje) ///Aqui modificaria si el usuario tiene avisos pendientes
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
 * @brief Funcion para eliminar un usuario
 * @param mensaje JSON del mensaje
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 */
JSON Usuario::cancelar(bool ok, JSON mensaje)
{
    if (ok)
    {
        QSqlQuery select;           ///El borrado seria un boton que diga eliminar busquedas, despues se abriria una
                                    ///tabla mostrando los productos en busqueda de ese usuario, y este deberia seleccionar un
                                    /// elemento de la lista para despues borrarlo.
        select.prepare("SELECT id_usuario, nombre_usuario FROM usuario WHERE nombre_usuario = :nombreusuario");
        select.bindValue(":nombreusuario", QString::fromStdString(mensaje["product"]));
        select.exec();
        std::list<QString> lista; ///Lista donde almacenaré los datos buscados
        while (select.next()) {
            lista.push_back(select.value("nombre_usuario").toString());

            qDebug() << select.value("nombre_usuario").toString();
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
        query.prepare("DELETE FROM usuario WHERE nombre_usuario = :nombreQuery");
        qDebug() << select.value("nombre_usuario").toString();
        query.bindValue(":nombreQuery", select.value("nombre_usuario").toString());
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
JSON Usuario::revisar(bool ok, JSON mensaje) ///Funcion para revisar los datos de un usuario
{
    if (ok)
    {
        QSqlQuery query;
        query.prepare("SELECT id_usuario, nombre_usuario, avisos, id_producto FROM usuario WHERE nombre_usuario = :nombreusuario");
        //query.bindValue(":nombreTabla", QString::fromStdString(m_nombreProducto.c_str()));
        query.bindValue(":nombreusuario", QString::fromStdString(mensaje["product"]));
        qDebug() << query.exec();
        while (query.next()) {
                QString nombre = query.value("nombre_usuario").toString();
                QString avisos = query.value("avisos").toString();
                QString recurso = query.value("id_producto").toString();
                qDebug() << nombre << estado << avisos << recurso;
                mensaje["busqueda"]["nombre"].push_back(nombre.toStdString());
                mensaje["busqueda"]["avisos"].push_back(avisos.toStdString());
                mensaje["busqueda"]["id_producto"].push_back(recurso.toStdString());
        }
        mensaje["action"] = "select";
        ///Hay que hacer comprobacion de errores
        qDebug() << "Select " << query.lastError().text();
        //qDebug() << QString::fromStdString(mensaje.dump());

        return mensaje;
    }
    else return "No se han podido encontrar resultados.";
}

Usuario::Usuario()
{

}
