#!/bin/sh
# Instalación de paquetes

set -e

# Directorio de instalacion
export BUILD_DIR=/opt/ecommerce
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

#----------------Preconfiguración e instalación de paquetes -------------------#
function packages {
    cd ${BUILD_DIR} &&
    
    echo "Instalando paquetes y dependencias de la aplicación" &&
    yum update -y &&
    yum install mariadb-server httpd mod_ssl gtest -y &&
    yum group install "Development Tools" -y &&
    yum install cmake3 git openssl-devel boost-devel.x86_64 libstdc++-static.x86_64 -y &&

    # Extras para mayor facilidad
    yum install bash-completion.noarch bash-completion-extras.noarch vim &&

    # Version mas reciente de gcc y g++
    sudo yum install devtoolset-4-gcc devtoolset-4-gcc-c++ &&
    sudo update-alternatives --install /usr/bin/g++-5.3 g++-5.3 /opt/rh/devtoolset-4/root/usr/bin/g++ 10 &&
    sudo update-alternatives --install /usr/bin/gcc-5.3 gcc-5.3 /opt/rh/devtoolset-4/root/usr/bin/gcc 10 &&

    # Obtener mysql c++ connector
    wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-1.1.12-linux-el7-x86-64bit.rpm &&
    yum localinstall mysql-connector-c++-1.1.12-linux-el7-x86-64bit.rpm -y &&
    return 0 || return 1
}

#----------------------Compilación-----------------------#
function compile {
    cd ${BUILD_DIR}
    # Compilar e instalar código de la aplicación
    echo "Clonando repositorio" &&
    git clone https://github.com/danidim13/compra-cr.git &&

    echo "Compilando aplicación cgi" &&
    cd compra-cr &&
    mkdir build && cd build &&
    cmake3 -D CMAKE_C_COMPILER=/usr/bin/gcc-5.3 -D CMAKE_CXX_COMPILER=/usr/bin/g++-5.3 .. &&
	
	
    make app_ecommerce.cgi && make install &&
    cd .. &&
    return 0 || return 1
}

## Configuración de Apache

function apache_conf {
    cd ${BUILD_DIR} && cd compra-cr &&
    # Creación del virtual host
    mkdir /etc/httpd/sites-available /etc/httpd/sites-enabled &&
    cp etc/compra.cr.conf /etc/httpd/sites-available/ &&

    echo "# Include the virtual host configurations:" >> /etc/httpd/conf/httpd.conf &&
    echo "IncludeOptional sites-enabled/*.conf" >> /etc/httpd/conf/httpd.conf &&
    ln -s /etc/httpd/sites-available/compra.cr.conf /etc/httpd/sites-enabled/compra.cr.conf &&

    sudo mkdir -p /var/www/compra.cr/logs &&
    sudo mkdir -p /var/www/compra.cr/apache_logs &&
    sudo chown root:apache /var/www/compra.cr/logs &&
    sudo chmod g+w /var/www/compra.cr/logs &&
    return 0 || return 1
}

function selinux_conf {
    echo "Configurando SELinux"  &&
    semanage fcontext -a -t httpd_sys_ra_content_t  "/var/www/compra.cr/logs(/.*)?"  &&
    semanage fcontext -a -t httpd_log_t             "/var/www/compra.cr/apache_logs(/.*)?"  &&
    semanage fcontext -a -t httpd_sys_script_exec_t "/var/www/compra.cr/app_ecommerce.cgi"  &&
    semanage fcontext -a -t httpd_sys_content_t     "/var/www/compra.cr/webroot(/.*)?"  &&
    restorecon -R -v /var/www/compra.cr/  &&
    setsebool -P httpd_can_network_connect_db on  &&
    echo "Configuracion de Apache finalizada, iniciando servicio" &&
    systemctl enable httpd &&
    systemctl restart httpd &&
    return 0 || return 1
}

function mysql_conf {
    ## Configuracion de MariaDB
    cd ${BUILD_DIR} && cd compra-cr &&
    systemctl stop mariadb &&
    echo "Configurando MariaDB" &&
    cp etc/my.cnf /etc/my.cnf &&
    systemctl enable mariadb &&
    systemctl start mariadb &&

    # Crear esquema, usuario, tablas y procedimiento almacenado
    mysql -u root -h "localhost" < bd/schema_and_user.sql &&
    mysql -u root -h "localhost" < bd/table_definition.sql &&
    mysql -u root -h "localhost" < bd/purchase_sp.sql &&
    mysql -u root -h "localhost" < bd/data.sql &&
    return 0 || return 1
}

function firewall_conf {
    ## Firewall
    echo "Configurando firewall" &&
    sudo iptables -A INPUT -m state --state NEW,ESTABLISHED -p tcp --dport 80 -j ACCEPT -m comment --comment "Apache: allow incoming http connections on port 80" &&
    sudo iptables -A INPUT -m state --state NEW,ESTABLISHED -p tcp --dport 443 -j ACCEPT -m comment --comment "Apache:allow incoming https connections on port 443" &&
    service iptables save &&
    return 0 || return 1
}

function error_exit {
    RED='\033[0;31m'
    NC='\033[0m'
    echo -e "${RED}ERROR en paso $1 del proceso ${NC}"
    exit 1
}

packages     || error_exit "configuración de paquetes"
compile      || error_exit "compilación del código"
apache_conf  || error_exit "configuración de apache"
selinux_conf || error_exit "configuración de selinux"
mysql_conf   || error_exit "configuración de mysql"

echo "Configuración finalizada!"
echo "Recuerde modificar el archivo /etc/my.conf"
echo "Puede acceder a la aplicación en $(hostname -I | awk '{print $2}')"


