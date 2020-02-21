#include "usuarios.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <list>

usuarios::usuarios(QString nombre, QString correo, QString contra)
{
    this->nombre = nombre;
    this->correo = correo;
    this->contra = contra;
}

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
        qDebug("No estas conectado con la base de datos"); /// Falta JSON de error
        return "";
    }
}

JSON usuarios::revisar(bool ok, JSON mensaje) //std::string email, std::string password
{
    JSON mensajeDevuelto;
    if (ok)
    {
        bool login = false;
        usuarios usuario("", "", "");
        QSqlQuery query;
        query.prepare("SELECT correo, contrasena FROM usuarios where correo = :correo"); //(contrasena = crypt(:contrasena, contrasena))

        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));
        query.exec();

        while (query.next())
        {
            if (query.value("contrasena") == QString::fromStdString(mensaje["contrasena"]))
            {
                login = true;
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
/*
void usuarios::remove(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM usuario where idusuario = :id");

    query.bindValue(":id", id);
    query.exec();
}*/
