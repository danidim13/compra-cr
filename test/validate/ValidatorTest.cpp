#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-use-equals-delete"
#pragma ide diagnostic ignored "modernize-use-equals-delete"
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by daniel on 15/10/19.
//

#include "gtest/gtest.h"
#include "../../src/validate/Validator.h"

namespace {

    class ValidatorTest : public ::testing::Test {

    };

    TEST(ValidateRegex, SimplePatterns) {
        EXPECT_TRUE(validate::Validator::regex("hola", std::regex("^.*$")));
        EXPECT_TRUE(validate::Validator::regex("hola", std::regex("^[a-z]*$")));
        EXPECT_TRUE(validate::Validator::regex("hola", std::regex("^[a-z]*$")));
        EXPECT_FALSE(validate::Validator::regex("hOla", std::regex("^[a-z]*$")));
        EXPECT_TRUE(validate::Validator::regex("hOla", std::regex("^[a-zA-Z]*$")));
    }

    TEST(ValidateRegexDecimal, WholeNumbers) {
        EXPECT_TRUE(validate::Validator::regex("10", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::Validator::regex("0", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::Validator::regex("900", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::Validator::regex("1234567890", validate::REGEX_DECIMAL));
    }
    TEST(ValidateRegexDecimal, RationalNumbers) {
        EXPECT_TRUE(validate::Validator::regex("10.10", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::Validator::regex("-1430.143223", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::Validator::regex("1320.1324", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::Validator::regex("0.0", validate::REGEX_DECIMAL));
    }
    TEST(ValidateRegexDecimal, NonNumbers) {
        EXPECT_FALSE(validate::Validator::regex("10b", validate::REGEX_DECIMAL));
        EXPECT_FALSE(validate::Validator::regex("FE10", validate::REGEX_DECIMAL));
        EXPECT_FALSE(validate::Validator::regex("500 000", validate::REGEX_DECIMAL));
    }

    TEST(ValidateRegexSpanishNames, ValidNames) {
        EXPECT_TRUE(validate::Validator::regex("Gabriel García Márquez", validate::REGEX_SPANISH_NAMES));
        EXPECT_TRUE(validate::Validator::regex("José Feliciano", validate::REGEX_SPANISH_NAMES));
        EXPECT_TRUE(validate::Validator::regex("Álvaro de la Ossa Osegueda", validate::REGEX_SPANISH_NAMES));
    }

    TEST(ValidateRegexSpanishSentence, ValidSentence) {
        EXPECT_TRUE(validate::Validator::regex("Una ollita muy bonita.", validate::REGEX_SPANISH_SENTENCE));
        EXPECT_TRUE(validate::Validator::regex("Mae, ¡¿qué PUTAS está haciendo?!", validate::REGEX_SPANISH_SENTENCE));
        EXPECT_TRUE(validate::Validator::regex("e la vara s qe entonses ya ndie sab para k kieren las cosas.",
                                    validate::REGEX_SPANISH_SENTENCE));
    }

    TEST(ValidateRegexSpanishSentence, InvalidSql) {
        EXPECT_FALSE(validate::Validator::regex("olla%'", validate::REGEX_SPANISH_SENTENCE));
        EXPECT_FALSE(validate::Validator::regex("bla' --", validate::REGEX_SPANISH_SENTENCE));
        EXPECT_FALSE(validate::Validator::regex("bla\'\\* *\\", validate::REGEX_SPANISH_SENTENCE));
    }

    TEST(ValidateRegexEmail, ValidEmail) {
        EXPECT_TRUE(validate::Validator::regex("daniel.diazmolina@ucr.ac.cr", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("daniel.diazmolina@ucr.ac.cr", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("feoli19@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("erick32hb@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("gabjimenez93@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("jesua1394@hotmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("jgmz94_19@hotmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("karolyn.maclaren7@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("laumr18@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("lucia.leon.ch@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("bajist4@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("natimog@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("rossyescalona@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("rolo946@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("ssaenzcr@gmail.com", validate::REGEX_EMAIL));
        EXPECT_TRUE(validate::Validator::regex("willy59861@gmail.com", validate::REGEX_EMAIL));
    }

    TEST(ValidateRegexCardHolder, ValidCardHolder) {
        EXPECT_TRUE(validate::Validator::regex("DANIEL DIAZ MOLINA", validate::REGEX_CARD_HOLDER));
        EXPECT_TRUE(validate::Validator::regex("JOHN DOE", validate::REGEX_CARD_HOLDER));
        EXPECT_TRUE(validate::Validator::regex("maria parajeles", validate::REGEX_CARD_HOLDER));
        EXPECT_TRUE(validate::Validator::regex("FRANCIS VAN DER VAART", validate::REGEX_CARD_HOLDER));
    }

    TEST(ValidateRegexCardHolder, InvalidCardHolder) {
        EXPECT_FALSE(validate::Validator::regex("<script>alert(\"holi\")</script>", validate::REGEX_CARD_HOLDER));
        EXPECT_FALSE(validate::Validator::regex("1503", validate::REGEX_CARD_HOLDER));
        EXPECT_FALSE(validate::Validator::regex(" asd   sadf", validate::REGEX_CARD_HOLDER));
        EXPECT_FALSE(validate::Validator::regex(" daNiel D  D ", validate::REGEX_CARD_HOLDER));
    }


    TEST(ValidateRegexCardHolder, ValidCreditCard) {
        EXPECT_TRUE(validate::Validator::regex("5000-4000-3000-2000", validate::REGEX_CREDIT_CARD));
        EXPECT_TRUE(validate::Validator::regex("5002-3995-2998-1999", validate::REGEX_CREDIT_CARD));
    }

    TEST(ValidateRegexCardHolder, InvalidCreditCard) {
        EXPECT_FALSE(validate::Validator::regex("5000400030002000", validate::REGEX_CREDIT_CARD));
        EXPECT_FALSE(validate::Validator::regex("5002-3995-2998", validate::REGEX_CREDIT_CARD));
    }

    TEST(ValidateRegexExpire, ValidExpire) {
        EXPECT_TRUE(validate::Validator::regex("01/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("02/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("03/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("04/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("05/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("05/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("06/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("07/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("08/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("09/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("10/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("11/20", validate::REGEX_EXPIRE));
        EXPECT_TRUE(validate::Validator::regex("12/20", validate::REGEX_EXPIRE));
    }

    TEST(ValidateRegexExpire, InvalidExpire) {
        EXPECT_FALSE(validate::Validator::regex("00/20", validate::REGEX_EXPIRE));
        EXPECT_FALSE(validate::Validator::regex("13/20", validate::REGEX_EXPIRE));
        EXPECT_FALSE(validate::Validator::regex("56/20", validate::REGEX_EXPIRE));
        EXPECT_FALSE(validate::Validator::regex("blas", validate::REGEX_EXPIRE));
    }

    TEST(ValidateRegexPhone, ValidPhone) {
        EXPECT_TRUE(validate::Validator::regex("+(506) 8707-8691", validate::REGEX_PHONE));
        EXPECT_TRUE(validate::Validator::regex("+(1) 8-707-8691", validate::REGEX_PHONE));
        EXPECT_TRUE(validate::Validator::regex("+506 8707-8691", validate::REGEX_PHONE));
        EXPECT_TRUE(validate::Validator::regex("8707-8691", validate::REGEX_PHONE));
        EXPECT_TRUE(validate::Validator::regex("87078691", validate::REGEX_PHONE));
        EXPECT_TRUE(validate::Validator::regex("+50687078691", validate::REGEX_PHONE));
    }

    TEST(ValidateRegexPhone, InvalidPhone) {
        EXPECT_FALSE(validate::Validator::regex("+506 8707-8691-", validate::REGEX_PHONE));
        EXPECT_FALSE(validate::Validator::regex(" 8707-8691", validate::REGEX_PHONE));
        EXPECT_FALSE(validate::Validator::regex("-87078691", validate::REGEX_PHONE));
    }
}

#pragma clang diagnostic pop