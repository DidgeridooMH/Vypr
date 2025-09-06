#include "Vypr/Scanner/FileScanner.hpp"

#include <filesystem>

namespace Vypr
{
  FileScanner::FileScanner(const std::wstring &filename)
      : m_file(std::filesystem::path(filename)), m_bufferPosition(0),
        m_bufferSize(0)
  {
    if (!m_file.is_open())
    {
      throw FileNotFoundException();
    }

    FillBuffer();
  }

  wchar_t FileScanner::LookAhead(int offset)
  {
    if (m_bufferPosition + offset < m_bufferSize)
    {
      return m_buffer[m_bufferPosition + offset];
    }

    if (FillBuffer())
    {
      return LookAhead(offset);
    }

    return std::char_traits<wchar_t>::eof();
  }

  bool FileScanner::Finished()
  {
    return m_buffer.empty() && m_file.eof();
  }

  bool FileScanner::FillBuffer()
  {
    if (m_file.eof())
    {
      return false;
    }

    if (m_bufferPosition < m_bufferSize)
    {
      std::copy(m_buffer.begin() + m_bufferPosition,
                m_buffer.begin() + m_bufferSize, m_buffer.begin());
      m_bufferSize -= m_bufferPosition;
      m_bufferPosition = 0;
    }

    m_file.read(m_buffer.data() + m_bufferSize,
                InternalBufferSize - m_bufferSize);
    m_bufferSize += m_file.gcount();
    return m_bufferSize > 0;
  }

  wchar_t FileScanner::NextInternal()
  {
    if (m_bufferPosition < m_bufferSize)
    {
      return m_buffer[m_bufferPosition++];
    }

    if (FillBuffer())
    {
      return NextInternal();
    }

    return std::char_traits<wchar_t>::eof();
  }
} // namespace Vypr