use ecommerce;
-- Creación de tablas



-- Tabla de usuarios
create table users
    (
    id int unsigned not null auto_increment primary key,
    username varchar(50) not null unique,
    surname varchar(50) not null,
    last_name varchar(50) null,
    email varchar(256) not null unique,
    phone varchar(50) null,
    password char(77) not null
    );
    
-- Tabla para productos en venta
create table products
    (
    id int unsigned not null auto_increment primary key,
    owner_id int unsigned not null,
    title varchar(50) not null,
    detail varchar(500) null,
    iva decimal(5,4) not null default 0.13,
    service_rate decimal(5,4) not null default 0.15,
    unit_price decimal(12,2) not null,
    stock int unsigned not null
    );
    
-- Tabla para identificar recibos/facturas/ordenes de compra
create table purchases
    (
    id int unsigned not null auto_increment primary key,
    order_date datetime not null,
    buyer_id int unsigned not null,
    cost_tax decimal(12,2) not null,
    cost_raw decimal(12,2) not null,
    cost_total decimal(12,2) not null,
    receipt_code char(14) null,
    payment_state enum('retenido','confirmado','reclamo pendiente','devolución') not null default 'retenido'
    );

-- Desglose de compras    
create table purchases_products
    (
    purchase_id int unsigned not null,
    product_id int unsigned not null,
    amount int unsigned not null,
    unit_price decimal(12,2) not null,
    iva decimal(5,4) not null default 0.13,
    service_rate decimal(5,4) not null default 0.15,
    seller_id int unsigned not null,
    primary key(purchase_id, product_id)
    );
    
-- Tags para clasificar productos y búsqueda por categorias
# create table tags
#     (
#     label_es varchar(50) not null primary key
#     );
#
# -- Tags por producto
# create table products_tags
#     (
#     product_id int unsigned not null,
#     tag_label varchar(50) not null,
#     primary key(product_id, tag_label)
#     );
    
    
-- Foreign Key constraints

alter table products
    add foreign key (owner_id) references users(id);
    
    
alter table purchases
    add foreign key (buyer_id) references users(id);
    
alter table purchases_products
    add foreign key (purchase_id) references purchases(id),
    add foreign key (product_id) references products(id),
    add foreign key (seller_id) references users(id);
    
# alter table products_tags
#     add foreign key (product_id) references products(id),
#     add foreign key (tag_label) references tags(label_es);
    
    
-- Limpiar base de datos    
/*
alter table products drop foreign key (owner_id);
alter table purchases drop foreign key (buyer_id);
alter table purchases_products drop foreign key (purchase_id), drop foreign key (product_id), drop foreign key (seller_id);
alter table products_tags drop foreign key (product_id), drop foreign key (tag_label);
*/

/*
drop table products;
drop table products_tags;
drop table purchases;
drop table purchases_products;
drop table tags;
drop table users;
*/

commit;