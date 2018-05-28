
#include "EvilEngine/CoreEngine/CoreResourceManager/Resource.h"

class ImageLoaderResource : public Resource
{
public:
  ImageLoaderResource( const char * filename, bool isFlipped );
  ImageLoaderResource( int resourceType, uint32 resourceInstance, const char * filename, bool isFlipped );
  ImageLoaderResource( const char * filename, ResourceRef* memory, bool isFlipped );

  unsigned int    GetWidth(){ return m_Width; }
  unsigned int    GetHeight(){ return m_Height; }
  unsigned int    GetFormat(){ return m_Format; }
  uint8_t*        GetData(){ return m_Data; }

  virtual bool    LoadData();
  const char*     GetFilename() const{ return m_Filename; }

  virtual uint32  CreateTexture(){ return 0; }

  void            FlipImage( uint8* img, int width, int height, int bpp );

  int             m_RawReadPointer;

protected:

  unsigned int    m_Width;
  unsigned int    m_Height;
  int             m_Format;
  int             m_PixelSize;
  bool            m_Loaded;
  bool            m_IsFlipped;

private:

  bool            ParseBitmapFromMemory( ResourceRef * memory );
  bool            ParseBmpFromMemory( ResourceRef * memory );
  bool            ParsePngFromMemory( ResourceRef * memory );

  bool            Convert32BitBmpToRGBA();
  bool            Convert24BitBmpToRGBA();
  bool            Convert8BitBmpToRGBA( unsigned char* palette );
};
