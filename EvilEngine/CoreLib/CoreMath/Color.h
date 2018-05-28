#ifndef EVILENGINE_CORELIB_COREMATH_COLOR
#define EVILENGINE_CORELIB_COREMATH_COLOR 1

#include "Vector4.h"

#include <map>

class Color : public Vector4
{
public:

  Color();
  Color( float r, float g, float b);
  Color( float r, float g, float b, float alpha);
  Color( uint32 color );

  static Color    FromString(const String& color);

  static void     AddNamedColor(const String& namedColor, const Color& color);
  static Color    GetNamedColor( const String& namedColor );
  String          ToString() const;

  uint32          ToUInt32() const;

  float           GetAlpha() const;
  void            SetAlpha( float alpha );

  float           GetRed() const{ return x; }
  float           GetGreen() const{ return y; }
  float           GetBlue() const{ return z; }

  static Color    Darken( const Color& color );

  bool  operator  ==( const Color& othervec ){ return ( x == othervec.x && y == othervec.y && z == othervec.z && w == othervec.w ); }
  bool  operator  !=( const Color& othervec ){ return ( x != othervec.x || y != othervec.y || z != othervec.z || w != othervec.w ); }

  static Color Black;
  static Color White;
  static Color Yellow;
  static Color Red;
  static Color Green;
  static Color Blue;
  static Color Orange;
  static Color Grey;
  static Color DarkGrey;
  static Color LightGrey;
  static Color MediumBlue;
  static Color LightBlue;
  static Color DarkBlue;

  static std::map< uint32, Color > s_NamedColorMap;
};

#endif /// EVILENGINE_CORELIB_COREMATH_COLOR