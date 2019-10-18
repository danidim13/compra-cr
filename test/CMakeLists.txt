
enable_testing()
find_package(GTest REQUIRED)

add_executable(AllTests validate/ValidatorTest.cpp validate/MapValidatorTest.cpp auth/PasswordHassherTest.cpp)

gtest_add_tests(TARGET AllTests)

target_link_libraries(AllTests GTest::GTest GTest::Main core)


