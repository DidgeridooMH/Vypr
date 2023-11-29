include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(VYPR_TEST_SOURCE
  Lexer/CLangLexerTest.cpp
)

add_executable(vyprtest ${VYPR_TEST_SOURCE})
target_link_libraries(vyprtest libvypr GTest::gtest_main)

add_test(UnitTest vyprtest)

include(GoogleTest)
gtest_discover_tests(vyprtest)