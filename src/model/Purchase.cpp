//
// Created by daniel on 14/09/19.
//

#include "Purchase.h"
#include "ConnectionHandler.h"
#include "CardPayment.h"
#include "../validate/StringValidator.h"

model::Purchase::Purchase()
{
    table = "purchases";
    m_cols = {
            {"id", EntityAttr(0u)},
            {"buyer_id", EntityAttr(0u)},
            {"order_date", EntityAttr("")},
            {"cost_tax", EntityAttr("")},
            {"cost_raw", EntityAttr("")},
            {"cost_total", EntityAttr("")},
            {"payment_state", EntityAttr("")},
            {"receipt_code", EntityAttr("")},
    };

}

std::pair<bool, std::string> model::Purchase::processPurchase(std::vector<model::Product> products, unsigned int buyer, std::map<std::string, std::string> card_data) {

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

    std::pair<bool, std::string> paymentResult = model::CardPayment::process(purchase.cost_total(), card_data);
    // TODO: Simular compra con tarjeta

    if (paymentResult.first) {

        std::ostringstream update;
        update << "UPDATE purchases SET receipt_code = \"" << paymentResult.second << "\" WHERE id = " << purchase.id();
        ConnectionHandler::tranExecute(conn.get(), update.str());

        ConnectionHandler::tranCommit(conn.get());

        std::ostringstream log_msg;
        log_msg << "Pago exitoso para compra con id=" << purchase.id() << " realizada por usuario con id="  << buyer;
        log_msg << " por concepto de ₡ " << purchase.cost_total() << " y número de factura " << paymentResult.second;
        log_info(NULL, (char*)log_msg.str().c_str());

    } else {
        ConnectionHandler::tranRollback(conn.get());
    }

    return paymentResult;
}

unsigned int model::Purchase::id() {
    return m_cols["id"].attrValue.u;
}

unsigned int model::Purchase::buyer_id() {
    return m_cols["buyer_id"].attrValue.u;
}

std::string model::Purchase::order_date() {
    return m_cols["order_date"].attrStr;
}

std::string model::Purchase::cost_tax() {
    return m_cols["cost_tax"].attrStr;
}

std::string model::Purchase::cost_raw() {
    return m_cols["cost_raw"].attrStr;
}

std::string model::Purchase::cost_total() {
    return m_cols["cost_total"].attrStr;
}

std::string model::Purchase::payment_state() {
    return m_cols["payment_state"].attrStr;
}

validate::MapValidator model::Purchase::CardValidator() {
    return validate::MapValidator({
        {"card_holder", std::make_shared<validate::StringValidator>(validate::REGEX_CARD_HOLDER, 1, 50)},
        {"card_number", std::make_shared<validate::StringValidator>(validate::REGEX_CREDIT_CARD, 19, 19)},
        {"ccv", std::make_shared<validate::StringValidator>(validate::REGEX_CCV, 3, 5)},
        {"expiration_date", std::make_shared<validate::StringValidator>(validate::REGEX_EXPIRE, 5, 5)},
    });
}
