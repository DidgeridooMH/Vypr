#include "Vypr/Scanner/StringScanner.hpp"

namespace Vypr
{
  StringScanner::StringScanner(std::wstring source)
      : m_buffer(source), m_index(0)
  {
  }

  wchar_t StringScanner::NextInternal()
  {
    return m_buffer[m_index++];
  }

  wchar_t StringScanner::LookAhead(int stepSize)
  {
    if (m_index + stepSize >= m_buffer.size())
    {
      return std::char_traits<wchar_t>::eof();
    }
    return m_buffer[m_index + stepSize];
  }

  bool StringScanner::Finished()
  {
    return m_index >= m_buffer.size();
  }
} // namespace Vypr