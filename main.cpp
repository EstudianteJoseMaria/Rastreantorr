#include <QSqlDatabase>
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <iostream>
#include <stdio.h>
#include "ixwebsocket/IXWebSocketServer.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXConnectionState.h"

#include "usuarios.h"
#include "productos.h"
#include "bbdd.h"

#include "json.hpp"

using JSON = nlohmann::json;

/*! \file */

/*! \mainpage Documentación del socket
 * Programa desarrollado para poder hacer una gestión de unos productos almacenados en una base de datos  \n
 * En desarrollo
 * */

/**
 * @brief Función para comprobar la existencia de un JSON
 * @param json Es el JSON a comprobar
 * @param key
 * @return
 */
bool exists(const JSON& json, const std::string& key){
    return json.find(key) != json.end();
}

std::string mensajeRecibido(JSON js, BBDD &db, bool conectado)
{

    //Text format
    qDebug() << QObject::tr("Received message: ");
    std::cout << js << std::endl;
    //RESPUESTA
    usuarios usuario(db.m_db);
    Productos producto(db.m_db);

    if (js["action"] == "login")
    {
        JSON mensaje = usuario.revisar(conectado, js);
        qDebug() << usuario.id;
        return mensaje.dump();
    }
    else if (js["action"] == "register")
    {
        JSON mensaje = usuario.insertar(conectado, js);
        if(mensaje != "")
        {
            return mensaje.dump();
        }
    }
    else if (js["action"] == "estado")       //Comprobar producto
    {
        JSON mensaje = producto.revisar(conectado, js);
        if(mensaje != "")
        {
            return mensaje.dump();
        }
    }
    else if (js["action"] == "agregar"){     //Agregar producto
        JSON mensaje = producto.insertar(conectado, js);
        if(mensaje != "")
        {
            return mensaje.dump();
        }
    }

    else if (js["action"] == "cancelar")
    {
        JSON mensaje = producto.cancelar(conectado, js);
        if(mensaje != "")
        {
            return mensaje.dump();
        }
    }
}

/**
 * @brief main Función principal del programa donde se gestiona el comportamiento del servidor
 * @param argc
 * @param argv
 */
int main(int argc, char *argv[] )
{
    QApplication a(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load("myapp_es_ES", "../");
    a.installTranslator(&myappTranslator);

    BBDD db("../database.conf");

    bool conectado {false};
    usuarios usu(db.m_db);   //Crear el usuario que se conecta

    ix::WebSocketServer server(9990, "0.0.0.0");

    ix::SocketTLSOptions tlsOptions;

    tlsOptions.tls = true;
    tlsOptions.certFile = "./cert/localhost.crt";
    tlsOptions.keyFile = "./cert/localhost.key";
    tlsOptions.caFile = "NONE";

    if(tlsOptions.isValid()){
        std::cerr << "SSL valid" << std::endl;
    }

    //server.setTLSOptions(tlsOptions);

    server.setOnConnectionCallback(
        [&server, &db, &conectado, &usu](std::shared_ptr<ix::WebSocket> webSocket,
                  std::shared_ptr<ix::ConnectionState> connectionState)
        {
            webSocket->setOnMessageCallback(
                [webSocket, connectionState, &server, &db, &conectado, &usu](const ix::WebSocketMessagePtr msg)
                {
                    if (msg->type == ix::WebSocketMessageType::Open)
                    {
                        qDebug() << QObject::tr("New connection");

                        //Conexion base de datos

                        conectado = db.open();

                        if (conectado) qDebug() << "Base de datos abierta";
                        else qDebug() << "No se ha podido conectar";
                    }
                    else if (msg->type == ix::WebSocketMessageType::Close)
                    {
                        qDebug() << QObject::tr("Bye bye connection");
                        if (conectado){
                            db.close();
                            qDebug() << QObject::tr("BD cerrada");
                        }
                    }
                    else if (msg->type == ix::WebSocketMessageType::Message)
                    {
                        auto js = JSON::parse(msg->str);

                        std::string mensajeEnviar = mensajeRecibido(js, db, conectado);
                        webSocket->send(mensajeEnviar);
                    }
                }
            );
        }
    );

    auto res = server.listen();
    if (!res.first)
    {
        // Error handling
        return 1;
    }

    server.start();
    server.wait();
    server.stop();



}
