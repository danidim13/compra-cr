//
// Created by daniel on 15/10/19.
//

#include "ValidatorTest.h"
#include "gtest/gtest.h"
#include "../../src/validate/Validator.h"

namespace {

    class ValidatorTest : public ::testing::Test {

    };

    TEST(ValidateRegex, SimplePatterns) {
        EXPECT_TRUE(validate::validate_regex("hola", "^.*$"));
        EXPECT_TRUE(validate::validate_regex("hola", "^[a-z]*$"));
        EXPECT_FALSE(validate::validate_regex("hOla", "^[a-z]*$"));
        EXPECT_TRUE(validate::validate_regex("hOla", "^[a-zA-Z]*$"));
    }

    TEST(ValidateRegexDecimal, WholeNumbers) {
        EXPECT_TRUE(validate::validate_regex("10", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::validate_regex("0", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::validate_regex("900", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::validate_regex("1234567890", validate::REGEX_DECIMAL));
    }
    TEST(ValidateRegexDecimal, RationalNumbers) {
        EXPECT_TRUE(validate::validate_regex("10.10", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::validate_regex("-1430.143223", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::validate_regex("1320.1324", validate::REGEX_DECIMAL));
        EXPECT_TRUE(validate::validate_regex("0.0", validate::REGEX_DECIMAL));
    }
    TEST(ValidateRegexDecimal, NonNumbers) {
        EXPECT_FALSE(validate::validate_regex("10b", validate::REGEX_DECIMAL));
        EXPECT_FALSE(validate::validate_regex("FE10", validate::REGEX_DECIMAL));
        EXPECT_FALSE(validate::validate_regex("500 000", validate::REGEX_DECIMAL));
    }

    TEST(ValidateRegexSpanishNames, NonNumbers) {
        EXPECT_TRUE(validate::validate_regex("Gabriel García Márquez", validate::REGEX_SPANISH_NAMES));
        EXPECT_TRUE(validate::validate_regex("José Feliciano", validate::REGEX_SPANISH_NAMES));
        EXPECT_TRUE(validate::validate_regex("Álvaro de la Ossa Osegueda", validate::REGEX_SPANISH_NAMES));
    }
}
