
#ifndef EVILENGINE_CORERENDERING_RESOURCE_FONTATLASTEXTURERESOURCE
#define EVILENGINE_CORERENDERING_RESOURCE_FONTATLASTEXTURERESOURCE 1

#include "DDSTextureResource.h"
#include "../Bitmap.h"
#include <map>

class FontCharacter
{
public:

  FontCharacter( char character, const Bitmap* image ) :
    m_Character( character ),
    m_Bitmap( image )
  {
  }
  virtual ~FontCharacter()
  {
    delete m_Bitmap;
  }

  int           GetWidth()  const { return m_Bitmap->m_Width; }
  int           GetHeight() const { return m_Bitmap->m_Height; }

  int           m_Character;
  const Bitmap* m_Bitmap;
};

class Font
{
public:

  static const int MAX_CHARACTERS_Y;
  static const int MAX_CHARACTERS_X;
  static const int MAX_CHARACTERS;

  Font( uint32 resourceId, uint32 pixelSize )
  {
    m_ResourceId  = resourceId; 
    m_PixelSize   = pixelSize;
  }
  virtual ~Font()
  {
    for ( std::map< char, FontCharacter* >::iterator itChar = m_CharacterMap.begin(); itChar != m_CharacterMap.end(); ++itChar )
    {
      delete itChar->second;
    }
  }

  void                              AddCharacter( char character, const Bitmap* image );
  const FontCharacter*              GetCharacter( char character ) const;
  int                               GetPixelSize() const { return m_PixelSize; }

private:

  std::map< char, FontCharacter* >  m_CharacterMap;
  uint32                            m_ResourceId;
  uint32                            m_PixelSize;
};

class FontAtlasTextureResource : public DDSTextureResource
{
public:
  FontAtlasTextureResource( uint32 resourceInstance, const char * filename );
  virtual ~FontAtlasTextureResource();

  virtual bool                      LoadData();
  const Font*                       GetFont() const{ return m_Font; }

private:

  Font* m_Font;
};

#endif /// EVILENGINE_CORERENDERING_RESOURCE_TEXTURERESOURCE
