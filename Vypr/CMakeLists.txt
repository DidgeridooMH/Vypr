set(LIBVYPR_SOURCE
  "Source/AST/CompileError.cpp"
  "Source/AST/Expression/ConstantNode.cpp"
  "Source/AST/Expression/CastNode.cpp"
  "Source/AST/Expression/ExpressionNode.cpp"
  "Source/AST/Expression/UnaryOpNode.cpp"
  "Source/AST/Expression/BinaryOpNode.cpp"
  "Source/AST/Expression/PostfixOpNode.cpp"
  "Source/AST/Expression/VariableNode.cpp"
  "Source/AST/Type/StorageType.cpp"
  "Source/AST/Type/PointerType.cpp"
  "Source/AST/Type/IntegralType.cpp"
  "Source/AST/Type/RealType.cpp"
  "Source/AST/Type/ArrayType.cpp"
  "Source/AST/SymbolTable.cpp"
  "Source/CodeGen/Context.cpp"
  "Source/Lexer/CLangKeywords.cpp"
  "Source/Lexer/CLangLexer.cpp"
  "Source/Lexer/CLangPunctuators.cpp"
  "Source/Lexer/CLangTypePrefixes.cpp"
  "Source/Scanner/Scanner.cpp"
  "Source/Scanner/StringScanner.cpp"
  "Source/Scanner/FileScanner.cpp"
)

set(LIBVYPR_HEADER
  "Include/Vypr/AST/CompileError.hpp"
  "Include/Vypr/AST/ASTContext.hpp"
  "Include/Vypr/AST/Expression/ConstantNode.hpp"
  "Include/Vypr/AST/Expression/CastNode.hpp"
  "Include/Vypr/AST/Expression/ExpressionNode.hpp"
  "Include/Vypr/AST/Expression/UnaryOpNode.hpp"
  "Include/Vypr/AST/Expression/BinaryOp.hpp"
  "Include/Vypr/AST/Expression/BinaryOpNode.hpp"
  "Include/Vypr/AST/Expression/PostfixOpNode.hpp"
  "Include/Vypr/AST/Expression/VariableNode.hpp"
  "Include/Vypr/AST/Type/StorageType.hpp"
  "Include/Vypr/AST/Type/IntegralType.hpp"
  "Include/Vypr/AST/Type/RealType.hpp"
  "Include/Vypr/AST/Type/PointerType.hpp"
  "Include/Vypr/AST/Type/ArrayType.hpp"
  "Include/Vypr/AST/SymbolTable.hpp"
  "Include/Vypr/CodeGen/Context.hpp"
  "Include/Vypr/Lexer/CLangLexer.hpp"
  "Include/Vypr/Lexer/CLangToken.hpp"
  "Include/Vypr/Lexer/CLangTokenMap.hpp"
  "Include/Vypr/Lexer/CLangTokenType.hpp"
  "Include/Vypr/Scanner/Scanner.hpp"
  "Include/Vypr/Scanner/StringScanner.hpp"
  "Include/Vypr/Scanner/FileScanner.hpp"
  "Include/Vypr/Util/Overload.hpp"
)

add_library(libvypr ${LIBVYPR_SOURCE} ${LIBVYPR_HEADER})

target_include_directories(libvypr PUBLIC Include/ SYSTEM ${LLVM_INCLUDE_DIRS})

target_link_libraries(libvypr PUBLIC ${LLVM_LIBS})

if (NOT MSVC)
target_compile_options(libvypr PRIVATE "-Wno-deprecated")
endif ()

set(VYPR_SOURCE
  Source/CLI/Main.cpp
)
set(BUILD_SHARED_LIBS OFF)

add_executable(vypr ${VYPR_SOURCE})
target_link_libraries(vypr PRIVATE libvypr)
