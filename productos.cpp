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
    JSON mensajeDevuelto;
    if (ok)
    {
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO productos (nombre_producto, recurso) VALUES (:nombreQuery, :recursoQuery)");

        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["product"]));
        query.bindValue(":recursoQuery", QString::fromStdString(mensaje["web"]));

        mensajeDevuelto["salida"] = query.exec();
        mensajeDevuelto["action"] = "insert";
        mensajeDevuelto["error type"] = query.lastError().text().toStdString();

        qDebug() << QObject::tr("Insert Error: ") << query.lastError().text();

    } else mensajeDevuelto["error type"] = "Failed Connection";
    //endif
    return mensajeDevuelto;
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
        qDebug() << QObject::tr("Update ") << query.lastError().text();
    }
    return mensaje;
}

/**
 * @brief Funcion para cancelar la búsqueda de un producto
 * @param producto Nombre del producto a tratar
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @param mensaje Es el mensaje enviado desde el cliente para interactuar con él
 */
///El borrado seria un boton que diga eliminar busquedas, despues se abriria una
///tabla mostrando los productos en busqueda de ese usuario, y este deberia seleccionar un
/// elemento de la lista para despues borrarlo.
JSON Productos::cancelar(bool ok, JSON mensaje)
{
    JSON mensajeDevuelto;
    if (ok)
    {
        QSqlQuery query(m_db);
        query.prepare("DELETE FROM productos WHERE nombre_producto = :nombreQuery");
        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["product"]));
        mensajeDevuelto["salida"] = query.exec();
        qDebug() << QObject::tr("Cancelar ") << query.lastError().text();

        mensajeDevuelto["action"] = "delete";
        mensajeDevuelto["errorType"] = query.lastError().text().toStdString();

            /// No mostrar en el cliente todos los elementos
            /// Tengo que hacer que el registro de usuarios tenga un registro de sus propios productos
            /// buscados para que cada uno vea solo sus productos
    } else mensajeDevuelto["salida"] = false; mensajeDevuelto["errorType"] = "No estas conectado a la bbdd";
    return mensajeDevuelto;
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
    JSON mensajeDevuelto;
    if (ok)
    {
        QSqlQuery query(m_db);
        query.prepare("SELECT id_producto, nombre_producto, estado_producto, tiempo_transcurrido, recurso FROM productos WHERE nombre_producto = :nombreproducto");
        //query.bindValue(":nombreTabla", QString::fromStdString(m_nombreProducto.c_str()));
        query.bindValue(":nombreproducto", QString::fromStdString(mensaje["product"]));

        mensajeDevuelto["salida"] = query.exec();
        //qDebug() <<

        while (query.next()) {
                QString nombre = query.value("nombre_producto").toString();
                QString estado = query.value("estado_producto").toString();
                QString tiempo = query.value("tiempo_transcurrido").toString();
                QString recurso = query.value("recurso").toString();
                qDebug() << nombre << estado << tiempo << recurso;
                mensajeDevuelto["busqueda"]["nombre"].push_back(nombre.toStdString());
                mensajeDevuelto["busqueda"]["estado"].push_back(estado.toStdString());
                mensajeDevuelto["busqueda"]["tiempo"].push_back(tiempo.toStdString());
                mensajeDevuelto["busqueda"]["recurso"].push_back(recurso.toStdString());
        }

        mensajeDevuelto["action"] = "select";
                                                    //Hay que hacer comprobacion de errores
        qDebug() << QObject::tr("Select Error:  ") << query.lastError().text();

    }
    else mensajeDevuelto["errorType"] = "No se han podido encontrar resultados."; mensajeDevuelto["salida"] = false;

    return mensajeDevuelto;
}

Productos::Productos(QSqlDatabase db)
{

    if (!QSqlDatabase::contains( "MyDb"))
    {
        m_db = QSqlDatabase::addDatabase("QPSQL", "MyDb");
    }
    else
    {
        m_db = QSqlDatabase::database("MyDb");
    } // end if
    m_db.setHostName("localhost");
    m_db.setPort(5432);
    m_db.setUserName("postgres");
    m_db.setPassword("");

    /*db.database();
    m_db.setDatabaseName(db.databaseName());
    bool ok = m_db.open();*/
    /*db.close();
    db.setDatabaseName("template1");
    bool ok = db.open();
    if (ok)
    {
        //qDebug() << "Borrando...";
        QSqlQuery q0("DROP DATABASE IF EXISTS test_sockets", db);
        if (q0.lastError().type() == QSqlError::NoError)
        {
            //qDebug() << "Creando...";
            QSqlQuery q1("CREATE DATABASE test_sockets", db);
            if (q1.lastError().type() == QSqlError::NoError)
            {
                db.close();
                db.setDatabaseName("test_sockets");
                db.open();

                /// Crea estructura en la base de datos
                QString sql {"CREATE TABLE productos ( \
                    id_producto     SERIAL, \
                    nombre_producto    varchar(40), \
                    estado_producto    varchar(20), \
                    tiempo_transcurrido varchar(20),\
                    recurso            varchar(30), \
                    PRIMARY KEY(id_producto) \
                )"};
                //qDebug() << "Iniciando...";
                QSqlQuery q2(sql, db);
                if (q2.lastError().type() == QSqlError::NoError)
                {
                    qDebug() << "Todo funcionando!!!!!!!!!!!!!";
                    m_db = db;
                } // end if
            }
            else
            {
                qDebug() << "BB:" << db.lastError().text();
            } // end if

        }
        else
        {
            qDebug() << "AA:" << db.lastError().text();
        } // end if

    } // end if
    //qDebug() << "Base iniciada";*/
}
