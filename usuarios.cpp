#include "usuarios.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <list>

/*! \file */

/**
 * @brief Constructor de usuarios
 * @return
 */
usuarios::usuarios(QSqlDatabase db)
{
    this->nombre = nombre;
    this->correo = correo;
    this->contra = contra;
}



/**
 * @brief Función para añadir un usuario a las actuales
 * @param mensaje JSON del mensaje
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @return
 */
JSON usuarios::insertar(bool ok, JSON mensaje)
{
    if (ok)
    {
        JSON mensajeEnviar;
        QSqlQuery query;
        query.prepare("INSERT INTO usuarios (nombre_usuario, contrasena, correo) VALUES (:nombre_usuario, :contrasena, :correo);"); //crypt(:pass, gen_salt('bf'))
        //PGP_SYM_ENCRYPT('1234','AES_KEY')
        query.bindValue(":nombre_usuario", QString::fromStdString(mensaje["usuario"]));//m_nombre
        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));//m_email
        query.bindValue(":contrasena", QString::fromStdString(mensaje["contra"]));//m_contra
        query.exec();

        QString error = query.lastError().text();

        if (error == " ")
        { 
            QSqlRecord rec = query.record();

            int id = query.lastInsertId().toInt();
            this->id = id;
            qDebug() << "Insertado con éxito";
            mensajeEnviar["action"] = id;
            return mensajeEnviar;
        }else
        {
            qDebug() << error;
            return "";
        }

    }
    else
    {
        qDebug() << QObject::tr("No estas conectado con la base de datos");/// Falta JSON de error
        return "";
    }
}

/**
 * @brief Funcion para comprobar si existe el usuario
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 * @param mensaje Es el mensaje enviado desde el cliente para interactuar con él
 * @return
 */
JSON usuarios::revisar(bool ok, JSON mensaje) //std::string email, std::string password
{
    JSON mensajeDevuelto;
    if (ok)
    {
        bool login = false;
        usuarios usuario(m_db);
        QSqlQuery query;
        query.prepare("SELECT correo, contrasena FROM usuarios where correo = :correo"); //(contrasena = crypt(:contrasena, contrasena))

        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));
        query.exec();

        while (query.next())
        {
            if (query.value("contrasena") == QString::fromStdString(mensaje["contrasena"]))
            {
                login = true;
                qDebug() << "Login correcto";
            } //endif
            else
            {
                mensajeDevuelto["error"] = "Contraseña incorrecta";
            }
        }

        if  (login)
        {
            query.prepare("SELECT * FROM usuarios WHERE correo = :correo AND contrasena = :contra");
            query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));
            query.bindValue(":contra", QString::fromStdString(mensaje["contrasena"]));
            query.exec();

            //QSqlRecord rec = query.record();
            while (query.next())
            {
                mensajeDevuelto["action"] = "id";
                /*QString nombre = query.value("nombre_usuario").toString();
                QString correo = query.value("correo").toString();
                QString contra = query.value("contrasena").toString();
                usuarios user(nombre, correo, contra);*/

                this->id = query.value("id_usuario").toInt();
                mensajeDevuelto["nombre_usuario"] = query.value("nombre_usuario").toString().toStdString();
                return mensajeDevuelto;
            }
        }
        else
        {

            mensajeDevuelto = "";
            return mensajeDevuelto;
        } // end if
    }
    else
    {
        mensajeDevuelto["error"]="Fallo con el servidor";
        return mensajeDevuelto;
    }

}

/**
 * @brief Funcion para eliminar un usuario
 * @param mensaje JSON del mensaje
 * @param ok Booleano que confirmará si se ha conectado a la base de datos
 */
/*
void usuarios::remove(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM usuario where idusuario = :id");

    query.bindValue(":id", id);
    query.exec();
}*/
