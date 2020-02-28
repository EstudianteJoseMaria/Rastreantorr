#ifndef APP_H
#define APP_H

#include "doctest.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include "productos.h"
#include "json.hpp"

using JSON = nlohmann::json;

class Db
{
public:
    QSqlDatabase m_db;
    Db();
    ~Db();
    bool start();
    void close();
    QSqlError lastError();
    bool insert(QString valor);
    bool select(QString valor);
    //Productos p(QSqlDatabase base);

};

class App : public QObject
{
    Q_OBJECT

    doctest::Context m_context;

public:
    App(QObject *parent = nullptr);

public slots:
    void run();
};



#endif // APP_H
