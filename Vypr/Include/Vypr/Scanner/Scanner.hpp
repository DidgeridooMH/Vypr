#pragma once

#include <string>

namespace Vypr
{
  /// @brief Retrieves characters from a source and keeps track of line and
  /// column counts.
  class Scanner
  {
  public:
    /// @brief Initializes line and column state.
    Scanner();

    virtual ~Scanner() = default;

    /// @brief Builds a string from the scanner while `condition` holds true or
    /// there is no more characters in the source.
    ///
    /// @param condition Condition function if true continues to append
    /// characters to resulting string.
    /// @returns String prefix of the  scanner source such that all resulting
    /// character hold true for `condition`.
    std::wstring NextWhile(bool (*condition)(wchar_t));

    /// @returns 1-indexed line count of the current cursor.
    inline size_t GetLine() const
    {
      return m_line;
    };

    /// @returns 1-indexed column count of the current cursor.
    inline size_t GetColumn() const
    {
      return m_column;
    };

    /// @brief Retrieves the next character from the scanner source or `EOF` if
    /// the source is empty. Increments the line and column counts accordingly.
    ///
    /// @returns Next character in the scanner source.
    wchar_t Next();

    /// @brief Retrieves `max(length,remaining)` characters from the scanner
    /// source. Increments the line and column counts accordingly.
    ///
    /// @param length Number of characters to attempt to retrieve.
    /// @returns String of the front `length` characters. The resulting string
    /// can have less than `length` characters if the source does not have that
    /// many.
    std::wstring Next(size_t length);

    /// @brief Peeks `max(length,remaining)` characters from the scanner
    /// source.
    ///
    /// @param offset Number of characters to skip when looking ahead.
    /// @param length Number of characters to attempt to retrieve.
    /// @returns String of the front `length` characters. The resulting string
    /// can have less than `length` characters if the source does not have that
    /// many.
    std::wstring LookAhead(int offset, int length);

    /// @brief Peeks the next character from the scanner source or `EOF` if
    /// the source is empty.
    ///
    /// @param offsetNumber of characters to skip when looking ahead.
    /// @returns `offset`th character in the scanner source.
    virtual wchar_t LookAhead(int offset) = 0;

    /// @returns Whether more characters are available from the scanner source.
    virtual bool Finished() = 0;

  private:
    virtual wchar_t NextInternal() = 0;

    size_t m_column;
    size_t m_line;
  };
} // namespace Vypr