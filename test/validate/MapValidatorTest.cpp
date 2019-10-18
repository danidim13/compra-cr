//
// Created by daniel on 15/10/19.
//

#include <map>
#include "gtest/gtest.h"
#include "../../src/validate/StringValidator.h"
#include "../../src/validate/MapValidator.h"


namespace {

    using namespace validate;
    using namespace std;

    TEST(MapValidatorTest, NormalInputs) {

//        shared_ptr<Validator> base(new StringValidator(validate::REGEX_SPANISH_SENTENCE, 1, 50));
//        std::map<string, shared_ptr<Validator>> map = {{"bla", make_shared<StringValidator>(validate::REGEX_SPANISH_SENTENCE, 1, 50)}};


        validate::MapValidator v({
            {"title", make_shared<StringValidator>(validate::REGEX_SPANISH_SENTENCE, 1, 50)},
            {"detail", make_shared<StringValidator>(validate::REGEX_SPANISH_SENTENCE, 1, 500)},
            {"cost", make_shared<StringValidator>(validate::REGEX_DECIMAL, 1, 50)},
            {"optional", make_shared<StringValidator>(validate::REGEX_DECIMAL, 0, 50)}
        });

        std::map<string, string> data1 = {
            {"title", R"(olla%') OR 1=1 -- )"},
            {"detail", R"(<script>console.log("hola")</script>)"},
        };

        std::map<string, string> data2 = {
                {"title", R"(Olla a presión Renaware)"},
                {"detail", R"(Un detalle. Otra cosa interesante. blablabal!)"},
                {"cost", "10000.00"},
                {"optional", ""},
        };

        std::map<string, string> data3 = {
                {"title", R"(Olla a presión Renaware)"},
                {"detail", R"(Un detalle. Otra cosa interesante. blablabal!)"},
                {"extraField", "blabla"}
        };

        std::map<string, string> data4 = {
                {"title", R"(Olla a presión Renaware)"},
                {"detail", R"(Un detalle. Otra cosa interesante. blablabal!)"},
        };


        ASSERT_FALSE(v.validate(data1).valid);
        ASSERT_TRUE(v.validate(data2).valid);
        ASSERT_FALSE(v.validate(data3).valid);
        ASSERT_FALSE(v.validate(data4).valid);

        cerr << "Errors on data1" << endl;
        for (const auto& err : v.validate(data1).errors ) {
            cerr << err.first << ": " << err.second << endl;
        }

        cerr << "Errors on data3" << endl;
        for (const auto& err : v.validate(data3).errors ) {
            cerr << err.first << ": " << err.second << endl;
        }

        cerr << "Errors on data4" << endl;
        for (const auto& err : v.validate(data4).errors ) {
            cerr << err.first << ": " << err.second << endl;
        }


    }
}
