#include "usuarios.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <list>

usuarios::usuarios()
{
    /*this->nombre = nombre;
    this->mail = mail;
    this->contra = contra;*/
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
        usuarios usuario();//"", "", ""
        qDebug() << QString::fromStdString(mensaje["correo"]);
        QSqlQuery query;
        query.prepare("SELECT correo FROM usuarios where correo = :correo;"); //(contrasena = crypt(:contrasena, contrasena))

        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));
        query.exec();
        qDebug() << "Select Hecha";
        while (query.next())
        {
            if (query.value("correo") == QString::fromStdString(mensaje["correo"]))
            {
                qDebug() << "Existe en la bbdd";    //Ahora existe el usuario, ahora hay que comprobar su contraseña aqui dentro
            }
            //endif
        }
        if (!query.next())
        {
            qDebug() << "No existe en la bbdd";
        }

        //query;
        query.prepare("SELECT contrasena FROM usuarios where correo = :correo;"); //(contrasena = crypt(:contrasena, contrasena))

        query.bindValue(":contrasena", QString::fromStdString(mensaje["contra"]));
        query.bindValue(":correo", QString::fromStdString(mensaje["correo"]));
        query.exec();

        bool passCorrect = false;
        QSqlRecord rec = query.record();
        while (query.next())
        {
            passCorrect = QVariant(query.value("contrasena")).toBool();
        }

        if(passCorrect)
        {

            query.prepare("SELECT * from usuarios where correo = :correo");
            //query.bindValue(":email", eml);
            query.exec();

            QSqlRecord rec = query.record();
            while (query.next())
            {
                QString nombre = query.value("nombre").toString();
                QString pass = query.value("password").toString();
                QString email = query.value("email").toString();

                usuarios user();//nombre.toUtf8().constData(), pass.toUtf8().constData(), email.toUtf8().constData()
                //user.id = query.value("idusuario").toInt();
                return mensaje;//user;
            }

        } // end if


        return mensaje; //usuario
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
