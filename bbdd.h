#ifndef BBDD_H
#define BBDD_H

#include <QSqlDatabase>
#include <iostream>
/**
 * @brief The BBDD class
 *
 */
class BBDD
{
public:
    /**
     * @brief BBDD
     *
     * @param rutaArchivo
     */
    BBDD(std::string rutaArchivo);
    void close();
    bool open();
    QSqlDatabase m_db;
void leerArchivo(std::string archivo);
private:

};

#endif // BBDD_H
