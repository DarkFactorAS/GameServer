#ifndef EVILENGINE_CORELIB_BASICTYPES_STRINGID
#define EVILENGINE_CORELIB_BASICTYPES_STRINGID 1
#pragma once

class StringId
{
public:
  typedef uint32 Key;

  StringId() : m_Hash(0) {}
  StringId(const char* str);

#ifdef DEBUG
  void CheckCollisions();
#endif

  inline bool operator==(const StringId& other) const { return m_Hash == other.m_Hash; }
  inline bool operator<(const StringId& other) const { return m_Hash < other.m_Hash; }

  operator Key() const
  {
    return m_Hash;
  }

  static Key ComputeHash(const char* str);

public:
  Key m_Hash;
#ifdef DEBUG
  std::string m_Name;
#endif
};

#endif //EVILENGINE_CORELIB_BASICTYPES_STRINGID
