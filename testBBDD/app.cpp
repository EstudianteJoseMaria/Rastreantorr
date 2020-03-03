#include "app.h"
#include <QCoreApplication>
#include <iostream>
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>

App::App(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, this, SLOT(run()));
}

void App::run()
{
    qDebug() << "running...";

    m_context.run(); // run
    if (m_context.shouldExit())
    {
        QCoreApplication::quit();
    } // end if
}

Db::~Db()
{
    if (m_db.isOpen())
    {
        m_db.close();
    } // end if
}

Db::Db()
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
}

void Db::close()
{
    m_db.close();
}

bool Db::start()
{
    bool result = 0;

    m_db.close();
    m_db.setDatabaseName("template1");

    bool ok = m_db.open();

    if (ok)
    {
        qDebug() << "Borrando...";
        QSqlQuery q0("DROP DATABASE IF EXISTS test_sockets", m_db);
        if (q0.lastError().type() == QSqlError::NoError)
        {
            qDebug() << "Creando...";
            QSqlQuery q1("CREATE DATABASE test_sockets", m_db);
            if (q1.lastError().type() == QSqlError::NoError)
            {
                m_db.close();
                m_db.setDatabaseName("test_sockets");
                m_db.open();

                /// Crea estructura en la base de datos
                QString sql {"CREATE TABLE productos ( \
                    id_producto     SERIAL, \
                    nombre_producto    varchar(40), \
                    estado_producto    varchar(20), \
                    tiempo_transcurrido varchar(20),\
                    recurso            varchar(30), \
                    PRIMARY KEY(id_producto) \
                )"};
                qDebug() << "Iniciando...";
                QSqlQuery q2(sql, m_db);
                if (q2.lastError().type() == QSqlError::NoError)
                {
                    result = true;
                } // end if
            }
            else
            {
                qDebug() << "BB:" << m_db.lastError().text();
            } // end if

        }
        else
        {
            qDebug() << "AA:" << m_db.lastError().text();
        } // end if

    } // end if
    qDebug() << result;
    return result;
}

QSqlError Db::lastError()
{
    return m_db.lastError();
}

bool suma(int i, int j){
    if(i+j == 4){
        return true;
    } else return false;
}

TEST_CASE("base datos #0")
{
    JSON mensaje;
    mensaje["product"] = "Patata";
    mensaje["web"] = "amazon";
    Db db;

    if (db.start()) //p.m_db.isOpen())
    {
        qDebug() << "DB UP AND RUNNING";
        Productos p(db.m_db);
        SUBCASE("Insertar #0")
        {
            CHECK( p.insertar(true, mensaje)["salida"] == true );
        }
        SUBCASE("Cancelar #0")
        {
            CHECK( p.insertar(true, mensaje)["salida"] == true );
            CHECK( p.cancelar(true, mensaje)["salida"] == false );
        }
        SUBCASE("Select #0")
        {
            CHECK( p.insertar(true, mensaje)["salida"] == true );
            CHECK( p.revisar(true, mensaje)["salida"] == true );
        }
        SUBCASE("Cancelar #1")
        {
            qDebug() << "Deberia dar error porque la conex a la bbdd es false";
            CHECK( p.cancelar(false, mensaje)["salida"] == false );
        }
    }
    else
    {
        qDebug() << db.lastError().text();

    } // end if

}

TEST_CASE("producto #1")
{

    JSON mensaje;
    mensaje["product"] = "Patata";
    mensaje["web"] = "amazon";
    Db db;

    if (true)
    {
        Productos p(db.m_db);       //Se está ejecutando 2 veces y no deberia
        qDebug() << "DB UP AND RUNNING";

        SUBCASE("Insertar #1")
        {
            CHECK( p.insertar(true, mensaje)["salida"] == true );
        }
        SUBCASE("Cancelar #1")
        {
            CHECK( p.cancelar(true, mensaje)["salida"] == false );
        }
    }
    else
    {
        qDebug() << db.lastError().text();

    } // end if

}
