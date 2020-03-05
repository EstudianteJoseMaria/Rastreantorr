#include "bbdd.h"
#include <fstream>
#include <sstream>
#include <QDebug>

/*! \file */

/**
 * @brief Cierra la base de datos
 */
void BBDD::close()
{
    m_db.close();
}

/**
 * @brief Abre la base de datos
 */
bool BBDD::open()
{
    return m_db.open();
}

/**
 * @brief Sirve para enviarle a la bd un archivo de configuración
 * @param archivo Archivo de configuración
 */
void BBDD::leerArchivo(std::string archivo)
{
    std::ifstream fin(archivo);
    if(fin.is_open())
    {
        qDebug() << "Config correcto";
        std::string line;
        std::istringstream sin;
        while (std::getline(fin, line)) {
            sin.str(line.substr(line.find(":")+1));
            if (line.find("host") != std::string::npos) {
             std::cout<<"host: "<<sin.str()<<std::endl;
             m_db.setHostName(QString::fromStdString(sin.str()));
            } //endif
            else if (line.find("port") != std::string::npos) {
             //std::cout<<"port "<<sin.str()<<std::endl;
             m_db.setPort(std::stoi(sin.str()));
            } // endif
            else if (line.find("user") != std::string::npos) {
             //std::cout<<"user "<<sin.str()<<std::endl;
             m_db.setUserName(QString::fromStdString(sin.str()));
            } // endif
            else if (line.find("password") != std::string::npos) {
             //std::cout<<"pass "<<sin.str()<<std::endl;
             m_db.setPassword(QString::fromStdString(sin.str()));
            } // endif
            else if (line.find("database") != std::string::npos) {
             //std::cout<<"pass "<<sin.str()<<std::endl;
             m_db.setDatabaseName(QString::fromStdString(sin.str()));
            } // endif
            sin.clear();
        }
    } // endif
    else qDebug() << "No se ha encontrado el archivo";
}

BBDD::BBDD(std::string rutaArchivo)
{

    if (!QSqlDatabase::contains( "MyDb"))
    {
        m_db = QSqlDatabase::addDatabase("QPSQL", "MyDb");
    }
    else
    {
        m_db = QSqlDatabase::database("MyDb");
    } // end if

    leerArchivo(rutaArchivo);
}

