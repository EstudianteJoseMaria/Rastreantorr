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

    BBDD db("../config.prop");
    //QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
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

                        qDebug() << conectado;
                    }
                    else if (msg->type == ix::WebSocketMessageType::Close)
                    {
                        qDebug() << QObject::tr("Bye bye connection");
                        if (conectado){
                            db.close();
                            qDebug() << QObject::tr("BBDD cerrada");
                        }
                    }
                    else if (msg->type == ix::WebSocketMessageType::Message)
                    {
                        int idServer = 0;
                        auto js = JSON::parse(msg->str);
                        if (!msg->binary)
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
                                std::string mensajeEnviar = mensaje.dump();
                                webSocket->send(mensajeEnviar);
                                qDebug() << usuario.id;
                            }
                            else if (js["action"] == "register")
                            {
                                JSON mensaje = usuario.insertar(conectado, js);
                                if(mensaje != "")
                                {
                                    std::string mensajeEnviar = mensaje.dump();
                                    webSocket->send(mensajeEnviar);
                                }
                            }
                            else if (js["action"] == "estado")       //Comprobar producto
                            {
                                JSON mensaje = {
                                    {"idServer", idServer++},
                                    {"idUser", js["id"]},
                                    {"product", js["name"]},
                                    {"web", js["web"]},
                                };
                                mensaje = producto.revisar(conectado, mensaje);

                                std::string mensajeEnviar = mensaje.dump();
                                webSocket->send(mensajeEnviar);
                            }
                            else if (js["action"] == "agregar"){     //Agregar producto
                                JSON mensaje = {
                                    {"product", js["name"]},
                                };
                                mensaje = producto.insertar(conectado, mensaje);

                                std::string mensajeEnviar = mensaje.dump();
                                webSocket->send(mensajeEnviar);
                            }

                            else if (js["action"] == "cancelar")
                            {
                                JSON mensaje = {
                                    {"idServer", idServer++},
                                    {"idUser", js["id"]},
                                    {"product", js["name"]},
                                };
                                mensaje = producto.cancelar(conectado, mensaje);

                                std::string mensajeEnviar = mensaje.dump();
                                webSocket->send(mensajeEnviar);
                            }

                        }

                        //webSocket->send(msg->str, msg->binary);
                        //auto receivedObject = js.get<std::int32_t>();
                        //std::cout << receivedObject << std::endl;
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
