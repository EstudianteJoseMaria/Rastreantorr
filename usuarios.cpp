#include "usuarios.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <list>

/*! \file */

usuarios::usuarios(QSqlDatabase &db)
{
    m_db = db;
}

JSON usuarios::insertar(bool ok, JSON mensaje)
{
    if (ok)
    {
        JSON mensajeEnviar;
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO public.usuarios (nombre_usuario, contrasena, correo) VALUES (:nombre_usuario, :contrasena, :correo)"); //crypt(:pass, gen_salt('bf'))
        //PGP_SYM_ENCRYPT('1234','AES_KEY')
        query.bindValue(":nombre_usuario", QString::fromStdString(mensaje["usuario"]));//m_nombre
        query.bindValue(":contrasena", QString::fromStdString(mensaje["contra"]));//m_contra
        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));//m_email
        query.exec();
        qDebug() << m_db.hostName();
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
        qDebug() << QObject::tr("No estas conectado con la base de datos");// Falta JSON de error
        return "";
    }
}

JSON usuarios::revisar(bool ok, JSON mensaje) //std::string email, std::string password
{
    JSON mensajeDevuelto;
    if (ok)
    {
        bool login = false;
        QSqlQuery query(m_db);
        query.prepare("SELECT correo, contrasena FROM public.usuarios where correo = :correo"); //(contrasena = crypt(:contrasena, contrasena))
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
    return mensajeDevuelto;
}

JSON usuarios::cancelar(bool ok, JSON mensaje)
{
    if(ok)
    {
        QSqlQuery query(m_db);
        query.prepare("DELETE FROM usuario where idusuario = :id");

        query.bindValue(":id", id);
        query.exec();
    }

    return mensaje;
}
