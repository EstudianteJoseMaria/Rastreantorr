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
    if(ok)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO usuarios (nombre_usuario, contrasena, correo) VALUES (:nombre_usuario, :contrasena, :correo);"); //crypt(:pass, gen_salt('bf'))
        //PGP_SYM_ENCRYPT('1234','AES_KEY')
        query.bindValue(":nombre_usuario", QString::fromStdString(mensaje["nombre_user"]));//m_nombre
        query.bindValue(":contrasena", QString::fromStdString(mensaje["contra"]));//m_contra
        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));//m_email
        query.exec();

        QString error = query.lastError().text();

        if(error == " ")
        {
            query.prepare("SELECT * FROM usuarios ORDER BY id_usuario DESC LIMIT 1;");
            query.exec();
            QSqlRecord rec = query.record();
            while (query.next())
            {
                int id = query.value("id_usuario").toInt();
                this->id = id;
            }
        }else
        {
            qDebug() << error;
        }
    }
    else
    {
        qDebug("No estas conectado con la base de datos"); /// Falta JSON de error
    }

}

JSON usuarios::revisar(bool ok, JSON mensaje) //std::string email, std::string password
{
    if(ok)
    {
        bool login = false;
        usuarios usuario("", "", "");
        qDebug() << QString::fromStdString(mensaje["correo"]);
        QSqlQuery query;
        query.prepare("SELECT correo, contrasena FROM usuarios where correo = :correo;"); //(contrasena = crypt(:contrasena, contrasena))

        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));
        query.exec();
        qDebug() << "Select Hecha";

        while (query.next())
        {
            if (query.value("contrasena") == QString::fromStdString(mensaje["contrasena"]))
            {
                    login = true;
                    qDebug() << login;
            }
            //endif
        }

        if(login)
        {
            query.prepare("SELECT * FROM usuarios WHERE correo = :correo AND contrasena = :contra");
            query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));
            query.bindValue("contra", QString::fromStdString(mensaje["contrasena"]));
            query.exec();

            //QSqlRecord rec = query.record();
            while (query.next())        //Esto aun no funciona
            {
                qDebug() << query.value("nombre_usuario").toString();
                QString nombre = query.value("nombre_usuario").toString();
                QString correo = query.value("correo").toString();
                QString contra = query.value("contrasena").toString();

                usuarios user(nombre, correo, contra);
                this->id = query.value("id_usuario").toInt();
                qDebug() << user.contra;
                return mensaje;
            }

        } // end if


        return mensaje;
    }
    else
    {
        mensaje["errorType"]="Fallo con el servidor";
        return mensaje;
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
