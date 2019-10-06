USE ecommerce;
/**
 * El procedimiento asume que existe una tabla temporal
 * de la siguiente forma
create temporary table sale_tmp (
    product_id int unsigned unique,
    amount int unsigned
);
*/
    
-- call spCheckout(1, @purchasen);
-- select @purchasen;
-- set SQL_SAFE_UPDATES=0;
drop procedure if exists spPreprocessPurchase;

DELIMITER //
CREATE PROCEDURE spPreprocessPurchase (
    IN user_id INT UNSIGNED ,
    OUT purchase_number INT UNSIGNED)
    COMMENT 'Compra de productos a partir de la tabla temporal sale_tmp'
BEGIN
      
    IF NOT EXISTS (SELECT t1.id FROM products AS t1 INNER JOIN sale_tmp AS t2 ON t1.id = t2.product_id WHERE t1.stock < t2.amount) THEN
        BEGIN
            -- Crear la nueva compra
            INSERT INTO purchases (cost_raw, cost_tax, cost_total, buyer_id, order_date)
            SELECT SUM(t3.raw_cost) AS subtotal, SUM(t3.tax) AS subtotal_tax, SUM(t3.raw_cost) + SUM(t3.tax) AS total, user_id, NOW()
            FROM (
                SELECT t1.id, t2.amount, t1.unit_price, t1.iva, t1.unit_price*t2.amount AS raw_cost, t1.unit_price*t2.amount*t1.iva AS tax
                FROM products AS t1 INNER JOIN sale_tmp AS t2 ON t1.id = t2.product_id
                ) AS t3;
                
            SET purchase_number := LAST_INSERT_ID();
            
            -- Crear el detalle
            INSERT INTO purchases_products (purchase_id, product_id, amount, unit_price, iva, service_rate, seller_id)
            SELECT purchase_number, t1.id, t2.amount, t1.unit_price, t1.iva, t1.service_rate, t1.owner_id
            FROM products AS t1 INNER JOIN sale_tmp AS t2 ON t1.id = t2.product_id;
            
            -- Reducir el stock
            UPDATE products AS t1 INNER JOIN sale_tmp AS t2 ON t1.id = t2.product_id
            SET t1.stock = (t1.stock - t2.amount);
            
            
            -- Limpiar la tabla temporal
            DELETE FROM sale_tmp;
            
        END;
    ELSE
        BEGIN
            SET purchase_number := 0;
        END;
    END IF;

END //

DELIMITER ;
