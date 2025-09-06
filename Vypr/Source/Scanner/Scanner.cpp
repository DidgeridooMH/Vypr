#include "Vypr/Scanner/Scanner.hpp"

namespace Vypr
{
  Scanner::Scanner() : m_column(1), m_line(1)
  {
  }

  std::wstring Scanner::NextWhile(bool (*condition)(wchar_t))
  {
    std::wstring buffer;
    while (!Finished() && condition(LookAhead(0)))
    {
      buffer += Next();
    }
    return buffer;
  }

  wchar_t Scanner::Next()
  {
    if (Finished())
    {
      return std::char_traits<wchar_t>::eof();
    }

    wchar_t nextCharacter = NextInternal();
    if (nextCharacter == '\n')
    {
      m_line += 1;
      m_column = 1;
    }
    else
    {
      m_column += 1;
    }
    return nextCharacter;
  }

  std::wstring Scanner::Next(size_t length)
  {
    std::wstring buffer;
    while (!Finished() && length > 0)
    {
      buffer += Next();
      length -= 1;
    }
    return buffer;
  }

  std::wstring Scanner::LookAhead(int step, int length)
  {
    std::wstring buffer;
    for (int i = 0; i < length; i++)
    {
      buffer += LookAhead(step + i);
    }
    return buffer;
  }
} // namespace Vypr