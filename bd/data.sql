use ecommerce;

insert into users (id, username, surname, last_name, email, phone)
values (1, 'juan', 'Juan', 'Mora', 'juanperez@correo.com', '8800-2000');

insert into users (id, username, surname, last_name, email, phone)
values (2, 'mari', 'Maria', 'Rojas', 'marirj@correo.com', '2550-0552');

insert into products (id, owner_id, title, detail, unit_price, stock) values
(1, 1, 'iPad 7th Gen (nueva)', 'iPad como nueva, sin uso casi', 500, 10 ),
(2, 1, 'iPad 8th Gen (nueva)', 'iPad octava gen, uso moderado', 450, 10 ),
(3, 1, 'iPhone 7S', 'iPhone 7S 16 GB de disco y 2 GB de RAM, cámara 700 MPx', 1000, 10 ),
(4, 1, 'Olla de presión Renaware', 'La olla está nuevititica. Al chiliwili! mejor no podría. El precio es negociable', 50000.00, 15),
(5, 1, 'Parrilla pequeña acero inoxidable', 'Fácil de llevar, excelente para acampar', 25000.00, 15),
(6, 1, 'Computadora Lenovo', 'Laptop de 14 pulgadas, modelo Thinkpad X1 Carbon. Usada por 2 años', 350000.00, 15);

create temporary table sale_tmp (
  product_id int unsigned unique,
  amount int unsigned
);

insert into sale_tmp values
(2, 1),
(3, 4),
(4, 2);

call spPreprocessPurchase(2, @pid);

update purchases set receipt_code='FE-5252-632949' where id=@pid;
drop table sale_tmp;


