
enable_testing()
find_package(GTest REQUIRED)

add_executable(AllTests validate/ValidatorTest.cpp validate/ValidatorTest.h validate/MapValidatorTest.cpp validate/MapValidatorTest.h)

gtest_add_tests(TARGET AllTests)

target_link_libraries(AllTests GTest::GTest GTest::Main core)


