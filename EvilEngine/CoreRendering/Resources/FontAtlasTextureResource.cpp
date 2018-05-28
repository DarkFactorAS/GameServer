
#include "PreCompile.h"
#include "FontAtlasTextureResource.h"

const int Font::MAX_CHARACTERS_X = 10;
const int Font::MAX_CHARACTERS_Y = 10;
const int Font::MAX_CHARACTERS   = 100;

void Font::AddCharacter( char character, const Bitmap* image )
{
  if ( m_CharacterMap[ character ] == NULL )
  {
    m_CharacterMap[ character ] = new FontCharacter( character, image );
  }
};

const FontCharacter* Font::GetCharacter( char character ) const
{
  std::map< char, FontCharacter* >::const_iterator itCharacter = m_CharacterMap.find( character );
  if ( itCharacter != m_CharacterMap.end() )
  {
    return itCharacter->second;
  }
  return NULL;
}

FontAtlasTextureResource::FontAtlasTextureResource( uint32 resourceInstance, const char * filename ) :
  DDSTextureResource( ResourceType_FontAtlasTexture, resourceInstance, filename, false ),
  m_Font( NULL )
{
}

FontAtlasTextureResource::~FontAtlasTextureResource()
{
  delete m_Font;
}

bool FontAtlasTextureResource::LoadData()
{
  // Load the image
  if ( DDSTextureResource::LoadData() )
  {
    // Parse the image for the font
    Bitmap* srcBitmap = new Bitmap( m_Width, m_Height, m_Format, m_Data );
    m_Data = NULL;

    int     scaleY        = m_Height / Font::MAX_CHARACTERS_Y;
    int     scaleX        = m_Width  / Font::MAX_CHARACTERS_X;
    uint32  color         = 0;
    uint32  lastColor     = 0;
    char    char_counter  = 32; // The character map starts with space ( ascii = 32 )
    int     begin_x       = 0;

    m_Font = new Font( m_ResourceId, scaleX );

    //Bitmap* bbitmap = new Bitmap( 64, 64, m_Format );
    //bbitmap->CopyArea( PointI( 0, 0 ), *srcBitmap, Rect( 0, 0, 64,64 ) );
    //m_Font->AddCharacter( 'B', bbitmap );

    for ( int y = 0; y < Font::MAX_CHARACTERS_Y; y++ )
    {
      begin_x = 0;
      for ( unsigned int cur_x = 0; cur_x < m_Width; cur_x++)
      {
        lastColor = color;
        color = srcBitmap->GetPixel ( cur_x, y * scaleY );

        if ( color == 0xffffff00 )
        {
          begin_x = cur_x + 1;
        }
        else if ( color != lastColor && color == 0xffff00ff )
        {
          //if ( char_counter < Font::MAX_CHARACTERS )
          {
            Bitmap* bitmap = new Bitmap( cur_x - begin_x, scaleY, m_Format );
            bitmap->CopyArea( PointI( 0, 0 ), *srcBitmap, RectInt( begin_x, y * scaleY, cur_x - begin_x, scaleY ), true );
            m_Font->AddCharacter( char_counter, bitmap );
            //LogInfoFMT("FontAtlasTextureResource", " Added character [%c] area[ %d,%d x %d,%d ] ", char_counter, begin_x, y*scaleY, cur_x - begin_x, scaleY );

            char_counter++;

            // Skip the characters between 'Z' and 'a' ( we already deducted 32 since we do not count that )
            if (char_counter == 91 )
            {
              char_counter = 97;
            }
          }
        }
      }
    }
    
    delete srcBitmap;

    SetLoaded(true);
    return true;
  }
  return false;
}

//FontCharacter* FontAtlasTextureResource::GetCharacter( char character )
//{
//  if ( m_Font != NULL )
//  {
//    return m_Font->GetCharacter( character );
//  }
//  return NULL;
//}