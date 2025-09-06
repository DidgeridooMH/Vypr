#include <iostream>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <string>

#include "Vypr/AST/ASTContext.hpp"
#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Expression/ExpressionNode.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"
#include "Vypr/CodeGen/Context.hpp"
#include "Vypr/Lexer/CLangLexer.hpp"
#include "Vypr/Scanner/FileScanner.hpp"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: vypr [test_file.c]" << std::endl;
    return 1;
  }

  auto context = std::make_unique<Vypr::Context>("module");

  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto filename = std::wstring(argv[1], argv[1] + std::strlen(argv[1]));
  Vypr::CLangLexer lexer(std::make_unique<Vypr::FileScanner>(filename));

  try
  {
    Vypr::ASTContext astContext;
    astContext.typeTable.AddSymbol(
        L"var", std::make_shared<Vypr::IntegralType>(Vypr::Integral::Int, false,
                                                     false, true));

    auto expression = Vypr::ExpressionNode::Parse(lexer, astContext);
    if (expression == nullptr)
    {
      std::wcerr << L"Failed to parse expression." << std::endl;
      return 1;
    }
    std::wcout << expression->PrettyPrint(0) << std::endl;

    // Temp
    llvm::Function *function = llvm::Function::Create(
        llvm::FunctionType::get(expression->type->GetIRType(*context), false),
        llvm::Function::ExternalLinkage, "TestFunction", context->module);
    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(context->context, "entry", function);
    context->builder.SetInsertPoint(block);

    llvm::GlobalVariable *variable = new llvm::GlobalVariable(
        context->module, context->builder.getInt32Ty(), false,
        llvm::GlobalValue::InternalLinkage, context->builder.getInt32(42));
    context->builder.CreateStore(context->builder.getInt32(42), variable);
    context->symbolTable.AddSymbol(L"var", variable);
    // Temp

    llvm::Value *ret = expression->GenerateCode(*context);

    // Temp
    if (ret != llvm::UndefValue::get(context->builder.getVoidTy()))
    {
      context->builder.CreateRet(ret);
    }
    else
    {
      context->builder.CreateRetVoid();
    }
    // Temp

    if (!llvm::verifyFunction(*function, &llvm::errs()) && context->Verify())
    {
      context->PrettyPrint();
      context->GenerateObjectFile("module.o");
    }
  }
  catch (Vypr::CompileError &e)
  {
    std::wcerr << e.what() << std::endl;
  }

  return 0;
}
