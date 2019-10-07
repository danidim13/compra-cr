# Tarea Seguridad

Aplicación de compra y venta de bienes

## Instalación y Configuración 

### Servidor de producción CentOS

Se incluye el script de instalación `install.sh` para el servidor de producción.
El mismo incluye toda la instalación de paquetes, configuración de SELinux, Apache y MariaDB.

### Instalación manual en Ubuntu

Instalar herramientas de compilación

```
apt-get install build-essentials cmake
```

Instalar los siguientes paquetes:


```
apt-get install libmysqlclient mysqlclient-dev
apt-get install libmysqlcppconn libmysqlcppconn-dev
```

Desde la carpeta raíz del proyecto correr los siguientes comandos

```
cmake .
make && sudo make install
```

El programa se instalará en la ruta `/var/www/compra.cr`, luego se deben crear los directorios de logs y
abrir los permisos

```
sudo mkdir /var/www/compra.cr/logs
sudo chmod 775 /var/www/compra.cr/logs
```

### Configuración de Apache

Habilitar mod_rewrite y mod_cgi

```apacheconfig
sudo a2enmod rewrite cgi
sudo systemctl restart apache2 
```

Agregar configuración del sitio en `/etc/apache2/sites-available`

```apacheconfig
sudo cp compra.cr.conf /etc/apache2/sites-available/
```


Habilitar el sitio del archivo recién copiado y deshabilitar el default.

```apacheconfig
sudo a2dissite 000-default.conf
sudo a2ensite compra.cr.conf
```

### Configuración de la base de datos

Se requiere contar con una instalación de MariaDB. Luego se deben correr los scripts de creación que se encuentran en el
directorio `bd`, en el siguiente orden

```
schema_and_user.sql
table_definition.sql
purchase_sp.sql
data.sql
```


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

Funcionalidad             | Rutas                                    | Done
--------------------------|------------------------------------------|--------
Registro (1.1)            | /user/add:GET, /user/add:POST            |  [X]
Logout (2.5)              | /user/logout:GET                         |  [X]
Login (2.1)               | /user/login:GET, /user/login:POST        |  [X]
Consultar Productos (1.2) | /product/list:GET                        |  [X]
Home page                 | /:GET -> /product/list:GET caso especial |  [X]
Agregar artículo (2.2)    | /product/add:GET, /product/add:POST      |  [X]
Finalizar compra (2.4)    | /checkout:GET, /checkout:POST            |  [X]
Agregar al carrito (2.3)  | /cart/add (agregar cookie)               |  [X]
Comentarios (1.3)         | /comments:GET, comments:POST             |  [ ]

