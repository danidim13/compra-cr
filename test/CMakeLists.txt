
enable_testing()
find_package(GTest REQUIRED)

add_executable(AllTests validate/ValidatorTest.cpp validate/ValidatorTest.h)

gtest_add_tests(TARGET AllTests)

target_link_libraries(AllTests GTest::GTest GTest::Main core)


