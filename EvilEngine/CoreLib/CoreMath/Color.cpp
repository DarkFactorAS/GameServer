
#include "Precompile.h"
#include "Color.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringId.h"

Color Color::Black    = Color( 0, 0, 0 );               // 0x000000
Color Color::White    = Color( 1, 1, 1 );               // 0xFFFFFF
Color Color::Yellow   = Color( 0.6f, 0.52f, 0.17f );
Color Color::Red      = Color( 0.5f, 0, 0 );            // 0xFF0000
Color Color::Green    = Color( 0, 0.5f, 0 );            // 0x00FF00
Color Color::Blue     = Color( 0, 0, 0.5f );            // 0x0000FF
Color Color::Orange   = Color( 0.4f, 0.32f, 0.07f );    // 0x998524
Color Color::Grey     = Color( 0.5f, 0.5f, 0.5f );      // 0x808080
Color Color::DarkGrey = Color( 0.1f, 0.1f, 0.1f );      // 0x191919
Color Color::LightGrey = Color( 0.8f, 0.8f, 0.8f );     // 0xCCCCCC  
Color Color::DarkBlue = Color( 0.03f, 0.12f, 0.2f );      
Color Color::MediumBlue = Color(0.04f, 0.16f, 0.4f);
Color Color::LightBlue = Color(0.04f, 0.56f, 0.79f);

std::map< uint32, Color > Color::s_NamedColorMap;

Color::Color() : 
  Vector4()
{
}

Color::Color( float xx, float yy, float zz ) : 
  Vector4( xx, yy, zz, 1.0f )
{
}

Color::Color( float xx, float yy, float zz, float alpha ) : 
  Vector4( xx, yy, zz, alpha )
{
}

Color::Color( uint32 color )
{
  int a = 0xFF - ( ( color & 0xFF000000 ) >> 24 );   // Alpha
  int r = ( color & 0x00FF0000 ) >> 16;   // Red
  int g = ( color & 0x0000FF00 ) >> 8;   // Green
  int b = ( color & 0x000000FF ) >> 0;   // Blue

  SetX( r / 255.0f );
  SetY( g / 255.0f );
  SetZ( b / 255.0f );
  SetW( a / 255.0f );
}

Color Color::FromString(const String& color)
{
  if (color.IsNumeric())
  {
    uint32 numeric = (uint32)strtol(color.c_str(), NULL, 16);
    return Color( numeric );
  }
  else
  {
    return Color::GetNamedColor(color);
  }
}


uint32 Color::ToUInt32() const
{
  uint32 r = (uint32)( x * 255 );
  uint32 g = (uint32)( y * 255 );
  uint32 b = (uint32)( z * 255 );
  uint32 a = (uint32)( w * 255 );
  return ( a << 24 | r << 16 | g << 8 | b );
}

Color Color::Darken( const Color& color )
{
  return Color( color.x * 0.5f, color.y * 0.5f, color.z * 0.5f );
}

void Color::AddNamedColor(const String& namedColor, const Color& color )
{
  StringId id = StringId( namedColor.GetLower().c_str() );
  s_NamedColorMap[ id ] = color;
}

Color Color::GetNamedColor( const String& namedColor )
{
  String lowerColor = namedColor.GetLower();

  // Check the named colormap
  StringId idColor  = StringId(lowerColor.c_str());
  std::map<uint32, Color>::iterator itColor = s_NamedColorMap.find( idColor );
  if ( itColor != s_NamedColorMap.end() )
  {
    return itColor->second;
  }

  if ( lowerColor.EqualsNoCase("green") )
  {
    return Color::Green;
  }
  else if ( lowerColor.EqualsNoCase("red") )
  {
    return Color::Red;
  }
  else if ( lowerColor.EqualsNoCase("blue") )
  {
    return Color::Blue;
  }
  else if ( lowerColor.EqualsNoCase("orange") )
  {
    return Color::Orange;
  }
  else if ( lowerColor.EqualsNoCase("yellow") )
  {
    return Color::Yellow;
  }
  else if ( lowerColor.EqualsNoCase("black") )
  {
    return Color::Black;
  }
  else if ( lowerColor.EqualsNoCase("white") )
  {
    return Color::White;
  }
  else if ( lowerColor.EqualsNoCase("grey") )
  {
    return Color::Grey;
  }
  else if ( lowerColor.EqualsNoCase("darkgrey") )
  {
    return Color::DarkGrey;
  }
  else if ( lowerColor.EqualsNoCase("lightblue") )
  {
    return Color::LightBlue;
  }
  else if ( lowerColor.EqualsNoCase("darkblue") )
  {
    return Color::DarkBlue;
  }
  else if (lowerColor.EqualsNoCase("lightgrey"))
  {
    return Color::LightGrey;
  }
  return Color::White;
}

String Color::ToString() const
{
  uint32 colorId = ToUInt32();
  switch( colorId )
  {
    case 0x000000: return "black";
    case 0xFFFFFF: return "white";
  }
  return String::FormatString("0x%06x", colorId & 0xFFFFFF );
}


void Color::SetAlpha( float alpha )
{
  SetW( alpha );
}

float Color::GetAlpha() const
{
  return w;
}
