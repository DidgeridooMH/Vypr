#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Vypr
{
  template <typename T> class SymbolTable
  {
  public:
    SymbolTable()
    {
      m_tables.push_back({});
    }

    void PushScope()
    {
      m_tables.push_back({});
    }

    void PopScope()
    {
      m_tables.pop_back();
    }

    bool IsDuplicate(const std::wstring &symbol) const
    {
      return m_tables.back().contains(symbol);
    }

    void AddSymbol(const std::wstring &symbol, T type)
    {
      if (!m_tables.back().contains(symbol))
      {
        m_tables.back().insert({symbol, std::move(type)});
      }
    }

    T GetSymbol(const std::wstring &symbol) const
    {
      for (auto table = m_tables.rbegin(); table != m_tables.rend(); table++)
      {
        if (table->contains(symbol))
        {
          return table->at(symbol);
        }
      }
      return {};
    }

  private:
    std::vector<std::unordered_map<std::wstring, T>> m_tables;
  };

  class StorageType;

  template <>
  std::shared_ptr<StorageType> SymbolTable<
      std::shared_ptr<StorageType>>::GetSymbol(const std::wstring &symbol) const; 

  using TypeTable = SymbolTable<std::shared_ptr<StorageType>>;
} // namespace Vypr
