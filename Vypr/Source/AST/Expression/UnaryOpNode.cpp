#include "Vypr/AST/Expression/UnaryOpNode.hpp"

#include "Vypr/AST/CompileError.hpp"
#include "Vypr/AST/Expression/CastNode.hpp"
#include "Vypr/AST/Type/IntegralType.hpp"

namespace Vypr
{
  const std::unordered_map<CLangTokenType, UnaryOp> UnaryOperations = {
      {CLangTokenType::Increment, UnaryOp::Increment},
      {CLangTokenType::Decrement, UnaryOp::Decrement},
      {CLangTokenType::Subtract, UnaryOp::Negate},
      {CLangTokenType::Exclamation, UnaryOp::LogicalNot},
      {CLangTokenType::Tilde, UnaryOp::Not},
      {CLangTokenType::Star, UnaryOp::Deref},
      {CLangTokenType::And, UnaryOp::AddressOf},
      {CLangTokenType::Sizeof, UnaryOp::Sizeof}};

  const std::unordered_map<UnaryOp, std::wstring> BinaryOperationNames = {
      {UnaryOp::Increment, L"Increment"},
      {UnaryOp::Decrement, L"Decrement"},
      {UnaryOp::Negate, L"Negate"},
      {UnaryOp::LogicalNot, L"LogicalNot"},
      {UnaryOp::Not, L"Not"},
      {UnaryOp::Deref, L"Deref"},
      {UnaryOp::AddressOf, L"AddressOf"},
      {UnaryOp::Sizeof, L"Sizeof"}};

  UnaryOpNode::UnaryOpNode(UnaryOp op,
                           std::unique_ptr<ExpressionNode> &&expression,
                           size_t column, size_t line)
      : ExpressionNode(nullptr, column, line), m_op(op),
        m_expression(std::move(expression))
  {
    type = m_expression->type->Check(op);
    if (type == nullptr)
    {
      throw CompileError(CompileErrorId::InvalidOperands, column, line);
    }

    if (op == UnaryOp::LogicalNot &&
        (m_expression->type->GetType() != StorageMetaType::Integral ||
         dynamic_cast<IntegralType *>(m_expression->type.get())->integral !=
             Integral::Bool))

    {
      m_expression = std::make_unique<CastNode>(
          std::make_unique<IntegralType>(Integral::Bool, false, false, false),
          std::move(m_expression));
    }
  }

  std::wstring UnaryOpNode::PrettyPrint(int level) const
  {
    std::wstring result = ExpressionNode::PrettyPrint(level);
    result += L"UnaryOpNode(" + BinaryOperationNames.at(m_op) + L")\n";
    result += m_expression->PrettyPrint(level + 1);
    return result;
  }

  std::unique_ptr<ExpressionNode> UnaryOpNode::Parse(CLangLexer &lexer,
                                                     TypeTable &symbolTable)
  {
    CLangToken opToken = lexer.GetToken();
    return std::make_unique<UnaryOpNode>(
        UnaryOperations.at(opToken.type),
        ExpressionNode::Parse(lexer, symbolTable, 1), opToken.column,
        opToken.line);
  }
} // namespace Vypr