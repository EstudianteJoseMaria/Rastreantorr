 ****************************
 ****Proyecto Rastreantor****
 ****************************
 
 RASTREANTOR
 
 **URL GitHub**
 
 
 
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
     
     
 **Problemas que han surgido: **
    
    - Han surgido bastantes problemas durante la creación dle programa, ya que al principio, no entendía bien el
    funcionamiento de los mensajes Json y su estructura. 
    - En el momento de crear los certificados digitales del cliente salían problemas constantes y tardé demasiado en
    corregirlos todos. 
    
 **Mejoras posibles: **
    
    - Me hubiera gustado preparar los crawlers para que rastrearan la página web que pusiera el usuario, y poder automatizarlo.
    - También que los clientes pudieran interactuar con los productos que estaban en busqueda.
    
    
 **Base de datos**
 
    - El servidor usa una base de datos PostgreSQl llamada sockets. 
    
  **Usuarios de prueba**

    - Se puede entrar en el programa con un usuario de prueba: usuario: usuario, contraseña: usuario.
    
 **LICENCIAS**
 
    Licencia MIT
    Copyright 2020 José María Puigserver

    Por la presente se concede permiso, libre de cargos, a cualquier persona que obtenga una copia de este software y de los archivos de documentación asociados (el "Software"), a utilizar el Software sin restricción, incluyendo sin limitación los derechos a usar, copiar, modificar, fusionar, publicar, distribuir, sublicenciar, y/o vender copias del Software, y a permitir a las personas a las que se les proporcione el Software a hacer lo mismo, sujeto a las siguientes condiciones:

    El aviso de copyright anterior y este aviso de permiso se incluirán en todas las copias o partes sustanciales del Software.
    EL SOFTWARE SE PROPORCIONA "COMO ESTÁ", SIN GARANTÍA DE NINGÚN TIPO, EXPRESA O IMPLÍCITA, INCLUYENDO PERO NO LIMITADO A GARANTÍAS DE COMERCIALIZACIÓN, IDONEIDAD PARA UN PROPÓSITO PARTICULAR E INCUMPLIMIENTO. EN NINGÚN CASO LOS AUTORES O PROPIETARIOS DE LOS DERECHOS DE AUTOR SERÁN RESPONSABLES DE NINGUNA RECLAMACIÓN, DAÑOS U OTRAS RESPONSABILIDADES, YA SEA EN UNA ACCIÓN DE CONTRATO, AGRAVIO O CUALQUIER OTRO MOTIVO, DERIVADAS DE, FUERA DE O EN CONEXIÓN CON EL SOFTWARE O SU USO U OTRO TIPO DE ACCIONES EN EL SOFTWARE.
    Si se necesita comercializar o modificar el contenido del producto, se puede contactar con el autor: josemariapuigserver@estudiante.edib.es.
    
    José María Puigserver Sastre
    02/2020
