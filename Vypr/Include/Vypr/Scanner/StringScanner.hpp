#pragma once

#include "Vypr/Scanner/Scanner.hpp"

namespace Vypr
{
  class StringScanner : public Scanner
  {
  public:
    /// @brief Constructs a scanner from the source string. The string is copied
    /// to an internal buffer.
    ///
    /// @param source String to retrieve characters from.
    StringScanner(std::wstring source = L"");

    /// @brief Peeks the next character from the scanner source or `EOF` if
    /// the source is empty.
    ///
    /// @param offsetNumber of characters to skip when looking ahead.
    /// @returns `offset`th character in the scanner source.
    wchar_t LookAhead(int offset) override;

    /// @returns Whether more characters are available from the scanner source.
    bool Finished() override;

  protected:
    wchar_t NextInternal() override;

  private:
    std::wstring m_buffer;
    size_t m_index;
  };
} // namespace Vypr