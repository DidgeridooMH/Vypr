include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(VYPR_TEST_SOURCE
  "Lexer/CLangLexerTest.cpp"
  "Scanner/StringScannerTest.cpp"
  "AST/Expression/ConstantNodeTest.cpp"
  "AST/Expression/CastNodeTest.cpp"
  "AST/Expression/VariableNodeTest.cpp"
  "AST/Type/StorageTypeTest.cpp"
  "AST/Type/IntegralTypeTest.cpp"
  "AST/Type/PointerTypeTest.cpp"
)

add_executable(vyprtest ${VYPR_TEST_SOURCE})
target_link_libraries(vyprtest libvypr GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(vyprtest)

if (ENABLE_COVERAGE)
  set(COVERAGE_EXCLUDES "build/" "VyprTest/" "Vypr/Include/")
  setup_target_for_coverage_gcovr_html(
    NAME coverage
    EXECUTABLE ctest -j ${n_cores}
    DEPENDENCIES vyprtest
  )
endif (ENABLE_COVERAGE)
