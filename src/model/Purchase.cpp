//
// Created by daniel on 14/09/19.
//

#include "Purchase.h"
#include "ConnectionHandler.h"

std::pair<bool, std::string> model::Purchase::processPurchase(std::vector<model::Product> products, unsigned int buyer) {

    std::pair<bool, std::string> result;

    std::unique_ptr<sql::Connection> conn(ConnectionHandler::tranBegin());

    // Crear y poblar la tabla temporal
    {
        ConnectionHandler::tranExecute(conn.get(), "CREATE TEMPORARY TABLE sale_tmp ("
                                                   "    product_id INT UNSIGNED UNIQUE,"
                                                   "    amount INT UNSIGNED"
                                                   ")");

        std::ostringstream insert;

        insert << "INSERT INTO sale_tmp VALUES ";
        for (auto it = products.begin(); it != products.end(); ) {
            insert << "(" << it->id() << ", " << it->amount() << ")";
            if (++it != products.end()) {
                insert << ", ";
            }
        }
        ConnectionHandler::tranExecute(conn.get(), insert.str());
    }

    Purchase purchase;

    // Llamado a procedimiento almacenado
    {
        std::ostringstream call;
        call << "CALL spPreprocessPurchase(" << std::to_string(buyer) << ", @pid)";
        ConnectionHandler::tranExecute(conn.get(), call.str());

        std::unique_ptr<sql::ResultSet> res(ConnectionHandler::tranExecuteQuery(conn.get(), "SELECT * FROM purchases WHERE id = @pid"));
        if (res->next()) {
            purchase.set_from_row(res.get());
            if (res->next()) {
                log_warning(NULL, (char*) "Mas resultados de lo esperado en compra");
            }
        }
    }

    bool transactionSuccess = true;

    // TODO: Simular compra con tarjeta

    if (transactionSuccess) {
        ConnectionHandler::tranCommit(conn.get());
        result.first = true;
    } else {
        ConnectionHandler::tranRollback(conn.get());
        result.first = false;
        result.second = "Pago denegado";
    }

    return result;
}
