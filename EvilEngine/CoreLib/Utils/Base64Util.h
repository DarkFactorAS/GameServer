#ifndef EVILENGINE_CORELIB_COREUTILS_BASE64UTIL
#define EVILENGINE_CORELIB_COREUTILS_BASE64UTIL 1

class Base64Util
{
public:

  static std::string Encode(unsigned char const* bytes_to_encode, unsigned int in_len);
  static std::string Decode(std::string const& encoded_string);

};

#endif /// EVILENGINE_CORELIB_COREUTILS_BASE64UTIL