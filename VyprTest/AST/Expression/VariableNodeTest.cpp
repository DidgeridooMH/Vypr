#include "Vypr/AST/Expression/VariableNode.hpp"

#include <gtest/gtest.h>

#include "Vypr/AST/ASTContext.hpp"
#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/AST/Type/PointerType.hpp"
#include "Vypr/AST/Type/RealType.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Scanner/StringScanner.hpp"

namespace VariableNodeTest
{
  TEST(Parse, InvalidVariableName)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"34"));
    Vypr::ASTContext astContext;
    ASSERT_THROW(Vypr::VariableNode::Parse(lexer, astContext),
                 Vypr::CompileError);
  }

  TEST(Parse, UndefinedVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"alpha"));
    Vypr::ASTContext astContext;
    ASSERT_THROW(Vypr::VariableNode::Parse(lexer, astContext),
                 Vypr::CompileError);
  }

  TEST(Parse, IntegralVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"alpha"));
    Vypr::ASTContext astContext;
    astContext.typeTable.AddSymbol(
        L"alpha", std::make_shared<Vypr::IntegralType>(Vypr::Integral::Int,
                                                       false, false, true));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, astContext);

    ASSERT_EQ(variable->column, 1);
    ASSERT_EQ(variable->line, 1);
    ASSERT_EQ(variable->type->GetType(), Vypr::StorageMetaType::Integral);
    ASSERT_EQ(
        dynamic_cast<Vypr::IntegralType *>(variable->type.get())->integral,
        Vypr::Integral::Int);
    ASSERT_EQ(variable->type->isLValue, true);
    ASSERT_EQ(variable->type->qualifiers.isConst, false);
  }

  TEST(Parse, RealVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"alpha"));
    Vypr::ASTContext astContext;
    astContext.typeTable.AddSymbol(
        L"alpha",
        std::make_shared<Vypr::RealType>(Vypr::Real::Float, false, true));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, astContext);

    ASSERT_EQ(variable->column, 1);
    ASSERT_EQ(variable->line, 1);
    ASSERT_EQ(variable->type->GetType(), Vypr::StorageMetaType::Real);
    ASSERT_EQ(dynamic_cast<Vypr::RealType *>(variable->type.get())->real,
              Vypr::Real::Float);
    ASSERT_EQ(variable->type->isLValue, true);
    ASSERT_EQ(variable->type->qualifiers.isConst, false);
  }

  TEST(Parse, PointerVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"beta"));
    Vypr::ASTContext astContext;
    std::unique_ptr<Vypr::StorageType> storage =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Bool, false, false,
                                             true);
    astContext.typeTable.AddSymbol(
        L"beta", std::make_shared<Vypr::PointerType>(storage, false, true));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, astContext);

    ASSERT_EQ(variable->column, 1);
    ASSERT_EQ(variable->line, 1);
    ASSERT_EQ(variable->type->GetType(), Vypr::StorageMetaType::Pointer);
    ASSERT_EQ(variable->type->isLValue, true);
    ASSERT_EQ(variable->type->qualifiers.isConst, false);
  }

#define GENCODE_INT_TEST(typeName, bitWidth, testValue)                        \
  TEST(GenerateCode, typeName##Variable)                                       \
  {                                                                            \
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"beta"));    \
    Vypr::ASTContext astContext;                                               \
    astContext.typeTable.AddSymbol(                                            \
        L"beta", std::make_shared<Vypr::IntegralType>(                         \
                     Vypr::Integral::typeName, false, false, true));           \
    Vypr::Context context("module");                                           \
                                                                               \
    llvm::Function *function = llvm::Function::Create(                         \
        llvm::FunctionType::get(context.builder.getVoidTy(), false),           \
        llvm::Function::ExternalLinkage, "TestFunction", context.module);      \
    llvm::BasicBlock *block =                                                  \
        llvm::BasicBlock::Create(context.context, "entry", function);          \
    context.builder.SetInsertPoint(block);                                     \
    llvm::AllocaInst *allocation = context.builder.CreateAlloca(               \
        context.builder.getInt##bitWidth##Ty(), nullptr, "beta");              \
    context.symbolTable.AddSymbol(L"beta", allocation);                        \
    context.builder.CreateStore(context.builder.getInt##bitWidth(testValue),   \
                                context.symbolTable.GetSymbol(L"beta"));       \
                                                                               \
    std::unique_ptr<Vypr::VariableNode> variable =                             \
        Vypr::VariableNode::Parse(lexer, astContext);                          \
                                                                               \
    llvm::Value *variableValue = variable->GenerateCode(context);              \
                                                                               \
    ASSERT_TRUE(variableValue->getType()->isIntegerTy(bitWidth));              \
  }

  GENCODE_INT_TEST(Bool, 1, true);
  GENCODE_INT_TEST(Byte, 8, 14);
  GENCODE_INT_TEST(Short, 16, 0x3444);
  GENCODE_INT_TEST(Int, 32, 0x10000);
  GENCODE_INT_TEST(Long, 64, 0x100000000);

  TEST(GenerateCode, FloatVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"beta"));
    Vypr::ASTContext astContext;
    astContext.typeTable.AddSymbol(
        L"beta",
        std::make_shared<Vypr::RealType>(Vypr::Real::Float, false, true));
    Vypr::Context context("module");

    llvm::Function *function = llvm::Function::Create(
        llvm::FunctionType::get(context.builder.getVoidTy(), false),
        llvm::Function::ExternalLinkage, "TestFunction", context.module);
    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(context.context, "entry", function);
    context.builder.SetInsertPoint(block);
    llvm::AllocaInst *allocation = context.builder.CreateAlloca(
        context.builder.getFloatTy(), nullptr, "beta");
    context.symbolTable.AddSymbol(L"beta", allocation);
    context.builder.CreateStore(
        llvm::ConstantFP::get(llvm::Type::getFloatTy(context.context), 2.0f),
        context.symbolTable.GetSymbol(L"beta"));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, astContext);

    llvm::Value *variableValue = variable->GenerateCode(context);

    ASSERT_TRUE(variableValue->getType()->isFloatTy());
  }

  TEST(GenerateCode, DoubleVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"beta"));
    Vypr::ASTContext astContext;
    astContext.typeTable.AddSymbol(
        L"beta",
        std::make_shared<Vypr::RealType>(Vypr::Real::Double, false, true));
    Vypr::Context context("module");

    llvm::Function *function = llvm::Function::Create(
        llvm::FunctionType::get(context.builder.getVoidTy(), false),
        llvm::Function::ExternalLinkage, "TestFunction", context.module);
    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(context.context, "entry", function);
    context.builder.SetInsertPoint(block);
    llvm::AllocaInst *allocation = context.builder.CreateAlloca(
        context.builder.getDoubleTy(), nullptr, "beta");
    context.symbolTable.AddSymbol(L"beta", allocation);
    context.builder.CreateStore(
        llvm::ConstantFP::get(llvm::Type::getDoubleTy(context.context), 2.0),
        context.symbolTable.GetSymbol(L"beta"));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, astContext);

    llvm::Value *variableValue = variable->GenerateCode(context);

    ASSERT_TRUE(variableValue->getType()->isDoubleTy());
  }

  TEST(GenerateCode, PointerVariable)
  {
    Vypr::CLangLexer lexer(std::make_unique<Vypr::StringScanner>(L"beta"));
    Vypr::ASTContext astContext;
    std::unique_ptr<Vypr::StorageType> storage =
        std::make_unique<Vypr::IntegralType>(Vypr::Integral::Int, false, false,
                                             true);
    astContext.typeTable.AddSymbol(
        L"beta", std::make_shared<Vypr::PointerType>(storage, false, true));
    Vypr::Context context("module");

    llvm::Function *function = llvm::Function::Create(
        llvm::FunctionType::get(context.builder.getVoidTy(), false),
        llvm::Function::ExternalLinkage, "TestFunction", context.module);
    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(context.context, "entry", function);
    context.builder.SetInsertPoint(block);
    llvm::AllocaInst *allocation = context.builder.CreateAlloca(
        context.builder.getInt32Ty()->getPointerTo(), nullptr, "beta");
    context.symbolTable.AddSymbol(L"beta", allocation);
    context.builder.CreateStore(
        llvm::ConstantPointerNull::get(
            context.builder.getInt32Ty()->getPointerTo()),
        context.symbolTable.GetSymbol(L"beta"));

    std::unique_ptr<Vypr::VariableNode> variable =
        Vypr::VariableNode::Parse(lexer, astContext);

    llvm::Value *variableValue = variable->GenerateCode(context);

    ASSERT_TRUE(variableValue->getType()->isPointerTy());
  }
} // namespace VariableNodeTest
