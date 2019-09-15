# Tarea Seguridad

Aplicación de compra y venta de bienes

## Instalación y Configuración 

TODO

libmysqlclient mysqlclient-dev
libmysqlcppconn libmysqlcppconn-dev

Habilitar mod_rewrite y mod_cgi

sudo a2enmod rewrite cgi
sudo systemctl restart apache2 

Agregar configuración del sitio en /etc/apache2/sites-available

## Features

### 1. Usuario visitante

1. Página de registro: debe solicitar al menos el nombre completo, usuario, email, teléfono, así
  como otros datos que considere pertinentes.
2. Consultar productos para la venta en el sitio, incluya alguna funcionalidad básica de búsqueda.
3. Formulario de consultas, retroalimentación, reclamos.

### 2. Usuario registrado

1. Ingresar al sistema proporcionando alguna identificación.
2. Agregar información de un artículo para ponerlo en venta.
3. Agregar un artículo al carrito de compras.
4. Concretar/finalizar el proceso de compra/entrega.
5. Salir del sistema (logout)

### Mapeo a rutas y acciones

Funcionalidad             | Rutas
--------------------------|---------------------------
Registro (1.1)            | /user/add:GET, /user/add:POST
Logout (2.5)              | /user/logout:GET
Login (2.1)               | /user/login:GET, /user/login:POST
Consultar Productos (1.2) | /product/list:GET
Home page                 | /:GET -> /product/list:GET caso especial
Agregar artículo (2.2)    | /product/add:GET, /product/add:POST
Finalizar compra (2.4)    | /checkout:GET, /checkout:POST
Agregar al carrito (2.3)  | cookie? desde product/list
Comentarios (1.3)         | /comments:GET, comments:POST

