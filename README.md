 ****************************
 ****Proyecto Rastreantor****
 ****************************
 
 RASTREANTOR
 
 **Presentación del programa**
    
    El programa está diseñado para poder hacer un control de productos que introduces en una base de datos y poder
    monitorizar y gestionar el seguimiento de éstos. 
    Se basa en la recepción y envío  de mensajes entre un servidor y el cliente a través de web sockets y mensajes Json. 
    
 **Herramientas utilizadas: ** 
    
    Programa desarrollado con los lenguajes C++, JavaScript y HTML.
    El entorno de programación usado para compilar el código ha sido Qt5.
 
 **Funcionamiento del programa **
    
     - El programa funciona con una servidor alojado en un PC el cual, al estar en funcinamiento, recibe los mensajes
     que los clientes a través de un navegador y web sockets le envían para poder interactuar con el programa.
     - Todos los clientes pueden introducir el producto a buscar y éste se almacenará en la base de datos para su
     posterior uso para procesar los datos y ir actualizándose gracias a su respectiva dirección web. 
     - Todos los usuarios podrán ver los productos que están en procesamiento para ver el estado del producto, es decir,
     si el producto que buscan ha subido de precio, si el usuario ha cancelado su búsqueda, etc...
     
     
 **Problemas que hayan surgido: **
    
    - Han surgido bastantes problemas durante la creación dle programa, ya que al principio, no entendía bien el
    funcionamiento de los mensajes Json y su estructura. 
    - En el momento de crear los certificados digitales del cliente salían problemas constantes y tardé demasiado en
    corregirlos todos. 
    
 **Opciones que hubieramos implementado: **
    
    Me hubiera gustado preparar los crawlers para que rastrearan la página web que pusiera el usuario, y poder automatizarlo.
    También que los clientes pudieran interactuar con los productos que estaban en busqueda.
    
    
 **Base de datos**
 
    El servidor usa una base de datos PostgreSQl llamada sockets. 
    
  **Usuarios de prueba**
  
    Se puede entrar en el programa con un usuario de prueba: usuario: usuario, contraseña: usuario.
    
 **LICENCIAS**
 
    
    Creative Commons (CC - NC) 2020
    Esta obra está bajo una Licencia Creative Commons No-Comercial. 
    La comercialización de cualquier software  o derivado de éste puede ser castigado y se aplicarán las leyes penales
    del país de origen. Qualquiera puede acceder, ver y modificar el contenido del producto, pero en ningún caso puede
    distribuirlo ni comercializarlo. 
    
    José María Puigserver Sastre
    02/2020
