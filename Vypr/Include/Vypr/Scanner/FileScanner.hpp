#pragma once

#include <fstream>

#include "Vypr/Scanner/Scanner.hpp"

namespace Vypr
{
  /// @brief Exception thrown when a file cannot be found.
  struct FileNotFoundException : public std::runtime_error
  {
    FileNotFoundException() : std::runtime_error("File not found.")
    {
    }
  };

  /// @brief Scanner that retrieves characters from a file.
  class FileScanner : public Scanner
  {
  public:
    /// @brief Constructs a scanner from the file at `filename`.
    ///
    /// @param filename Path to the file to retrieve characters from.
    /// @throws FileNotFoundException if the file cannot be opened.
    FileScanner(const std::wstring &filename);

    /// @brief Peeks the next character from the scanner source or `EOF` if
    /// the source is empty.
    ///
    /// @param offset Number of characters to skip when looking ahead.
    /// @returns `offset`th character in the scanner source.
    wchar_t LookAhead(int offset) override;

    /// @returns Whether more characters are available from the scanner source.
    bool Finished() override;

  protected:
    /// @brief Retrieves the next character from the scanner source.
    /// @return The next character from the scanner source or `EOF` if the
    /// source is empty.
    wchar_t NextInternal() override;

  private:
    /// @brief Size of the internal read buffer.
    static constexpr size_t InternalBufferSize = 1024;

    /// @brief Fills the internal buffer from the file.
    /// @returns Whether any characters were read into the buffer.
    bool FillBuffer();

    std::wifstream m_file;
    std::array<wchar_t, InternalBufferSize> m_buffer;
    size_t m_bufferPosition;
    size_t m_bufferSize;
  };
} // namespace Vypr