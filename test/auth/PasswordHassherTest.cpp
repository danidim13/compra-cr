//
// Created by daniel on 17/10/19.
//

#include "gtest/gtest.h"
#include "../../src/auth/PasswordHasher.h"


namespace {

    using namespace auth;
    using namespace std;

    TEST(PasswordHassherTest, CorrectPasswords ) {
        auth::PasswordHasher hasher;

        string p1("admin");
        string p2("root");
        string p3("contrasena");
        string p4("esto.!df34");
        string p5("1243ASDFasdf");
        string p6("mysql");
        string p7("fasdfasdfsadfsadf3242143rer.124312341asdf");

        string h1(hasher.passwordHash(p1));
        string h2(hasher.passwordHash(p2));
        string h3(hasher.passwordHash(p3));
        string h4(hasher.passwordHash(p4));
        string h5(hasher.passwordHash(p5));
        string h6(hasher.passwordHash(p6));
        string h7(hasher.passwordHash(p7));

        ASSERT_TRUE(hasher.passwordVerify(p1, h1));
        ASSERT_TRUE(hasher.passwordVerify(p2, h2));
        ASSERT_TRUE(hasher.passwordVerify(p3, h3));
        ASSERT_TRUE(hasher.passwordVerify(p4, h4));
        ASSERT_TRUE(hasher.passwordVerify(p5, h5));
        ASSERT_TRUE(hasher.passwordVerify(p6, h6));
        ASSERT_TRUE(hasher.passwordVerify(p7, h7));
    }

    TEST(PasswordHassherTest, IncorrectPasswords ) {
        auth::PasswordHasher hasher;

        string p1("admin");
        string p2("root");
        string p3("contrasena");
        string p4("esto.!df34");
        string p5("1243ASDFasdf");
        string p6("mysql");
        string p7("fasdfasdfsadfsadf3242143rer.124312341asdf");

        string pp1("awmin");
        string pp2("raot");
        string pp3("contraséna");
        string pp4("esto.df34");
        string pp5("1243ASaDFasdf");
        string pp6("mysql!");
        string pp7("fasdf.asdfsadfsadf3242143rer.124312341asdf");


        string h1(hasher.passwordHash(p1));
        string h2(hasher.passwordHash(p2));
        string h3(hasher.passwordHash(p3));
        string h4(hasher.passwordHash(p4));
        string h5(hasher.passwordHash(p5));
        string h6(hasher.passwordHash(p6));
        string h7(hasher.passwordHash(p7));

        ASSERT_FALSE(hasher.passwordVerify(pp1, h1));
        ASSERT_FALSE(hasher.passwordVerify(pp2, h2));
        ASSERT_FALSE(hasher.passwordVerify(pp3, h3));
        ASSERT_FALSE(hasher.passwordVerify(pp4, h4));
        ASSERT_FALSE(hasher.passwordVerify(pp5, h5));
        ASSERT_FALSE(hasher.passwordVerify(pp6, h6));
        ASSERT_FALSE(hasher.passwordVerify(pp7, h7));
    }
}
