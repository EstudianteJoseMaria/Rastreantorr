#include "productos.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <list>


//TRUNCATE TABLE nombre_de_la_tabla RESTART IDENTITY;

/*! \file */

JSON Productos::insertar(bool ok, JSON mensaje)
{
    JSON mensajeDevuelto;
    if (ok)
    {
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO productos (nombre_producto, estado_producto, recurso, id_usuario) VALUES (:nombreQuery, :estado, :recursoQuery, :idusuario)");

        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["product"]));
        query.bindValue(":estado", "Procesando");
        query.bindValue(":recursoQuery", QString::fromStdString(mensaje["web"]));
        query.bindValue(":idusuario", QString::fromStdString(mensaje["iduser"]));

        qDebug() << query.exec();
        if(m_db.databaseName() == "test_sockets")
        {
            QSqlQuery testQuery(m_db);
            testQuery.prepare("SELECT id_producto, nombre_producto, recurso FROM productos WHERE id_producto = :idproducto");
            testQuery.bindValue(":idproducto", query.lastInsertId().toString());
            mensajeDevuelto["salida"] = testQuery.exec();
            qDebug() << "El insert ha salido exitoso";
        }
        mensajeDevuelto["action"] = "insert";
        mensajeDevuelto["error type"] = query.lastError().text().toStdString();

        qDebug() << QObject::tr("Insert Error: ") << query.lastError().text();

    } else mensajeDevuelto["error type"] = "Failed Connection";
    //endif
    return mensajeDevuelto;
}

JSON Productos::modificar(bool ok, JSON mensaje)
{
    if (ok)
    {
        QSqlQuery query(m_db);
        query.prepare("UPDATE ");// FROM Productos WHERE :nombreTabla == :nombreQuery");
        query.bindValue(":nombreTabla", QString::fromStdString(m_nombreProducto));
        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["product"]));
        query.exec();
        qDebug() << QObject::tr("Update ") << query.lastError().text();
    }
    return mensaje;
}

//El borrado seria un boton que diga eliminar busquedas, despues se abriria una
//tabla mostrando los productos en busqueda de ese usuario, y este deberia seleccionar un
// elemento de la lista para despues borrarlo.
JSON Productos::cancelar(bool ok, JSON mensaje)
{
    JSON mensajeDevuelto;
    if (ok)
    {
        QSqlQuery query(m_db);
        query.prepare("DELETE FROM productos WHERE id_producto = :nombreQuery");
        query.bindValue(":nombreQuery", QString::fromStdString(mensaje["productId"]));
        query.exec();
        qDebug() << QObject::tr("Cancelar ") << query.lastError().text();
        if(m_db.databaseName() == "test_sockets")
        {
            qDebug() << "Si devuelve false, se ha borrado con éxito porque no encuentra el dato";
            QSqlQuery testQuery(m_db);
            testQuery.prepare("SELECT id_producto, nombre_producto, recurso FROM productos WHERE nombre_producto = :nombreproducto");
            testQuery.bindValue(":nombreproducto", QString::fromStdString(mensaje["product"]));
            testQuery.exec();
            mensajeDevuelto["salida"] = testQuery.next();
            if( testQuery.next() == false) qDebug() << "Borrado con éxito";
            else qDebug() << "Sigue en la bd";
        }
        mensajeDevuelto["action"] = "delete";
        mensajeDevuelto["errorType"] = query.lastError().text().toStdString();

            // No mostrar en el cliente todos los elementos
            // Tengo que hacer que el registro de usuarios tenga un registro de sus propios productos
            // buscados para que cada uno vea solo sus productos
    } else
    {
        mensajeDevuelto["salida"] = false;
        mensajeDevuelto["errorType"] = "No estas conectado a la bbdd";
    }
    return mensajeDevuelto;
}

JSON Productos::revisar(bool ok, JSON mensaje) //Aqui estarian las comprobaciones del estado del producto
{
    JSON mensajeDevuelto;
    if (ok)
    {
        QSqlQuery query(m_db);
        query.prepare("SELECT id_producto, nombre_producto, estado_producto, tiempo_transcurrido, recurso FROM productos WHERE id_usuario = :idusuario");
        //query.bindValue(":nombreTabla", QString::fromStdString(m_nombreProducto.c_str()));
        //query.bindValue(":nombreproducto", QString::fromStdString(mensaje["product"]));
        query.bindValue(":idusuario", QString::fromStdString(mensaje["iduser"]));
        query.exec();
        if(m_db.databaseName() == "test_sockets")
        {
            qDebug() << QString::fromStdString(mensaje["product"]);
            qDebug() << query.next();
            if(query.value("nombre_producto") == QString::fromStdString(mensaje["product"]))
            {
                mensajeDevuelto["salida"] = true;
            }
        }

        while (query.next()) {
                QString id = query.value("id_producto").toString();
                QString nombre = query.value("nombre_producto").toString();
                QString estado = query.value("estado_producto").toString();
                QString tiempo = query.value("tiempo_transcurrido").toString();
                QString recurso = query.value("recurso").toString();
                qDebug() << nombre << estado << tiempo << recurso;
                mensajeDevuelto["busqueda"]["id"].push_back(id.toStdString());
                mensajeDevuelto["busqueda"]["nombre"].push_back(nombre.toStdString());
                mensajeDevuelto["busqueda"]["estado"].push_back(estado.toStdString());
                mensajeDevuelto["busqueda"]["tiempo"].push_back(tiempo.toStdString());
                mensajeDevuelto["busqueda"]["recurso"].push_back(recurso.toStdString());
        }

        mensajeDevuelto["action"] = "select";
                                                    //Hay que hacer comprobacion de errores
        qDebug() << QObject::tr("Select Error:  ") << query.lastError().text();

    }
    else
    {
        mensajeDevuelto["errorType"] = "No se han podido encontrar resultados.";
        mensajeDevuelto["salida"] = false;
    }

    return mensajeDevuelto;
}

Productos::Productos(QSqlDatabase &db)
{
    m_db = db;

    /*if (!QSqlDatabase::contains( "MyDb"))
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
    m_db.setPassword("");*/

}
