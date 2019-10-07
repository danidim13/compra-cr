#!/bin/sh
# Instalación de paquetes

echo "Instalando paquetes y dependencias de la aplicación"
yum update -y
yum install mariadb-server httpd -y
yum group install "Development Tools" -y
yum install cmake3 git boost-devel.x86_64 libstdc++-static.x86_64 -y

# Extras para mayor facilidad
# yum install bash-completion.noarch bash-completion-extras.noarch vim

# Directorio de instalacion
export BUILD_DIR=/opt/ecommerce

mkdir $BUILD_DIR
cd $BUILD_DIR
# Obtener mysql c++ connector
wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-1.1.12-linux-el7-x86-64bit.rpm
yum localinstall mysql-connector-c++-1.1.12-linux-el7-x86-64bit.rpm -y

# Compilar e instalar código de la aplicación
echo "Clonando repositorio"
git clone https://github.com/danidim13/compra-cr.git

echo "Compilando aplicación cgi"
cd compra-cr
cmake3 .
make app_ecommerce.cgi && make install

## Configuración de Apache

# Creación del virtual host
mkdir /etc/httpd/sites-available /etc/httpd/sites-enabled
cp compra.cr.conf /etc/httpd/sites-available/   

echo "# Include the virtual host configurations:" >> /etc/httpd/conf/httpd.conf
echo "IncludeOptional sites-enabled/*.conf" >> /etc/httpd/conf/httpd.conf
ln -s /etc/httpd/sites-available/compra.cr.conf /etc/httpd/sites-enabled/compra.cr.conf

sudo mkdir -p /var/www/compra.cr/logs
sudo mkdir -p /var/www/compra.cr/apache_logs
sudo chown root:apache /var/www/compra.cr/logs
sudo chmod g+w /var/www/compra.cr/logs

echo "Configurando SELinux"
semanage fcontext -a -t httpd_sys_ra_content_t "/var/www/compra.cr/logs(/.*)?"
semanage fcontext -a -t httpd_log_t "/var/www/compra.cr/apache_logs(/.*)?"
semanage fcontext -a -t httpd_sys_script_exec_t "/var/www/compra.cr/app_ecommerce.cgi"
restorecon -R -v /var/www/compra.cr/
setsebool -P httpd_can_network_connect_db on

echo "Configuracion de Apache finalizada, iniciando servicio"
systemctl enable httpd
systemctl restart httpd

## Configuracion de MariaDB
echo "Configurando MariaDB"
systemctl enable mariadb
systemctl start mariadb

# Crear esquema, usuario, tablas y procedimiento almacenado
mysql -u root -h "localhost" < bd/schema_and_user.sql
mysql -u root -h "localhost" < bd/table_definition.sql
mysql -u root -h "localhost" < bd/purchase_sp.sql
mysql -u root -h "localhost" < bd/data.sql

## Firewall
echo "Configurando firewall"
sudo iptables -A INPUT -m state --state NEW,ESTABLISHED -p tcp --dport 80 -j ACCEPT -m comment --comment "Apache: allow incoming http connections on port 80"
service iptables save

echo "Configuración finalizada!"
echo "Puede acceder a la aplicación en $(hostname -I | awk '{print $2}')"


