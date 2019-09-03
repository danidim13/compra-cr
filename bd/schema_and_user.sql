create schema if not exists ecommerce
    default charset = utf8mb4
    default collate = utf8mb4_spanish_ci;

-- drop schema ecommerce;

create user 'ecommerce_app'@'localhost'
    identified by 'seguridad2019';
    
grant all on ecommerce.*
    to 'ecommerce_app'@'localhost';
    
