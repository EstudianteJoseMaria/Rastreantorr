#include <QSqlDatabase>
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <iostream>
#include <stdio.h>
#include "ixwebsocket/IXWebSocketServer.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXConnectionState.h"

#include "usuario.h"
#include "productos.h"

#include "json.hpp"

using JSON = nlohmann::json;

/**
 * Funcion para comprobar la extistencia de un JSON
 */
bool exists(const JSON& json, const std::string& key){ ///Comprobador si existe el contenido de un JSON
    return json.find(key) != json.end();
}

JSON producto(){ ///Funcion que devuelve un producto
    JSON producto = {
        {"nombre", "Patatas"},
        {"precio", 10},
        {"web", "www.google.es"}
    };
    return producto;
}

/**
 * Funcion principal
 */
int main(int, char *[])
{
    /*QApplication a(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load("myapp_es_ES");
    a.installTranslator(&myappTranslator);*/

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    bool conectado {false};

    ix::WebSocketServer server(9990, "0.0.0.0");

    ix::SocketTLSOptions tlsOptions;
    
    tlsOptions.tls = true;
    tlsOptions.certFile = "./cert/localhost.crt";
    tlsOptions.keyFile = "./cert/localhost.key";
    tlsOptions.caFile = "NONE";

    if(tlsOptions.isValid()){
        std::cerr << "SSL valid" << std::endl;
    }

    server.setTLSOptions(tlsOptions);
    
    server.setOnConnectionCallback(
        [&server, &db, &conectado](std::shared_ptr<ix::WebSocket> webSocket,
                  std::shared_ptr<ix::ConnectionState> connectionState)
        {
            webSocket->setOnMessageCallback(
                [webSocket, connectionState, &server, &db, &conectado](const ix::WebSocketMessagePtr msg)
                {
                    if (msg->type == ix::WebSocketMessageType::Open)
                    {
                        std::cout << "New connection" << std::endl;

                        ///Conexion base de datos
                        db.setHostName("localhost");
                        db.setDatabaseName("sockets");
                        db.setUserName("postgres");
                        db.setPassword("");

                        conectado = db.open();
                        qDebug() << conectado;

                        //std::string messageToSend = jsonMessage1.dump();
                          //  webSocket->send(messageToSend);
                    }
                    else if (msg->type == ix::WebSocketMessageType::Close)
                    {
                        std::cout << "Bye bye connection" << std::endl;
                        if (conectado){
                            db.close();
                            qDebug() << "BBDD cerrada";
                        }
                    }
                    else if (msg->type == ix::WebSocketMessageType::Message)
                    {
                        int idServer = 0;
                        auto js = JSON::parse(msg->str);
                        if (!msg->binary)
                        {
                            ///Text format
                            std::cout << "Received message: " << js << std::endl;
                            ///RESPUESTA
                            //Usuario usuario;       //Segun lo que envie la pagina web (mensaje.action) hacer cosas sobre productos o sobre usuarios
                            Productos producto;
                            if (js["action"] == "estado")       //Comprobar producto
                            {
                                //producto.m_nombreProducto = js["name"];
                                JSON mensaje = {
                                    {"idServer", idServer++},
                                    {"idUser", js["id"]},
                                    {"product", js["name"]},
                                    {"web", js["web"]},
                                };
                                if (mensaje["action"] == "productos") mensaje = producto.revisar(conectado, mensaje); //Hacer esto

                                std::string mensajeEnviar = mensaje.dump(); ///Devuelvo el estado
                                webSocket->send(mensajeEnviar);
                            }
                            else if (js["action"] == "agregar")     //Agregar producto
                            {                                      ///Esta comentado asi para que sea mas facil verlo
                                JSON mensaje = {
                                    {"idServer", idServer++},
                                    {"idUser", js["id"]},
                                    {"product", js["name"]},
                                    {"web", js["web"]},
                                };
                                mensaje = producto.agregar(conectado, mensaje);

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
