
#ifndef TEXTURES_CREATION_H_
#define TEXTURES_CREATION_H_

#include "TextureResource.h"

#define BYTE uint8
#define WORD uint16
#define DWORD uint32
#define LONG uint32
#define LPVOID uint32 // void*

class DDSTextureResource : public TextureResource
{
public:
  DDSTextureResource( uint32 resourceInstance, const char * filename, bool flipped );
  DDSTextureResource( int resourceType, uint32 resourceInstance, const char * filename, bool flipped );

  bool            LoadData();
  bool            ParseData( ResourceRef* rawData );
  virtual uint32  CreateTexture();

private:

  void            BGR2RGB( uint8_t* data, int numPixels, int pixelSize );
  int             getMaskByteIndex(unsigned int mask);

  uint            m_TextureId;
  int             m_NumMinMaps;
};


typedef struct _DDSCAPS2
{
    DWORD       dwCaps;         // capabilities of surface wanted
    DWORD       dwCaps2;
    DWORD       dwCaps3;
    union
    {
        DWORD       dwCaps4;
        DWORD       dwVolumeDepth;
    };
} DDSCAPS2;


typedef struct _DDCOLORKEY
{
    DWORD        dwColorSpaceLowValue;   
    DWORD        dwColorSpaceHighValue;  
} DDCOLORKEY;

typedef struct _DDPIXELFORMAT
{
    DWORD       dwSize;                 // size of structure
    DWORD       dwFlags;                // pixel format flags
    DWORD       dwFourCC;               // (FOURCC code)
    union
    {
        DWORD   dwRGBBitCount;          // how many bits per pixel
        DWORD   dwYUVBitCount;          // how many bits per pixel
        DWORD   dwZBufferBitDepth;      // how many total bits/pixel in z buffer (including any stencil bits)
        DWORD   dwAlphaBitDepth;        // how many bits for alpha channels
        DWORD   dwLuminanceBitCount;    // how many bits per pixel
        DWORD   dwBumpBitCount;         // how many bits per "buxel", total
        DWORD   dwPrivateFormatBitCount;// Bits per pixel of private driver formats. Only valid in texture
                                        // format list and if DDPF_D3DFORMAT is set
    };
    union
    {
        DWORD   dwRBitMask;             // mask for red bit
        DWORD   dwYBitMask;             // mask for Y bits
        DWORD   dwStencilBitDepth;      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
        DWORD   dwLuminanceBitMask;     // mask for luminance bits
        DWORD   dwBumpDuBitMask;        // mask for bump map U delta bits
        DWORD   dwOperations;           // DDPF_D3DFORMAT Operations
    };
    union
    {
        DWORD   dwGBitMask;             // mask for green bits
        DWORD   dwUBitMask;             // mask for U bits
        DWORD   dwZBitMask;             // mask for Z bits
        DWORD   dwBumpDvBitMask;        // mask for bump map V delta bits
        struct
        {
            WORD    wFlipMSTypes;       // Multisample methods supported via flip for this D3DFORMAT
            WORD    wBltMSTypes;        // Multisample methods supported via blt for this D3DFORMAT
        } MultiSampleCaps;

    };
    union
    {
        DWORD   dwBBitMask;             // mask for blue bits
        DWORD   dwVBitMask;             // mask for V bits
        DWORD   dwStencilBitMask;       // mask for stencil bits
        DWORD   dwBumpLuminanceBitMask; // mask for luminance in bump map
    };
    union
    {
        DWORD   dwRGBAlphaBitMask;      // mask for alpha channel
        DWORD   dwYUVAlphaBitMask;      // mask for alpha channel
        DWORD   dwLuminanceAlphaBitMask;// mask for alpha channel
        DWORD   dwRGBZBitMask;          // mask for Z channel
        DWORD   dwYUVZBitMask;          // mask for Z channel
    };
} DDPIXELFORMAT;

typedef struct _DDSURFACEDESC2
{
    DWORD                dwSize;                 // size of the DDSURFACEDESC structure
    DWORD                dwFlags;                // determines what fields are valid
    DWORD                dwHeight;               // height of surface to be created
    DWORD                dwWidth;                // width of input surface
    union
    {
        LONG             lPitch;                 // distance to start of next line (return value only)
        DWORD            dwLinearSize;           // Formless late-allocated optimized surface size
    };
    union
    {
        DWORD            dwBackBufferCount;      // number of back buffers requested
        DWORD            dwDepth;                // the depth if this is a volume texture
    };
    union
    {
        DWORD            dwMipMapCount;          // number of mip-map levels requestde
                                                 // dwZBufferBitDepth removed, use ddpfPixelFormat one instead
        DWORD            dwRefreshRate;          // refresh rate (used when display mode is described)
        DWORD            dwSrcVBHandle;          // The source used in VB::Optimize
    };
    DWORD                dwAlphaBitDepth;        // depth of alpha buffer requested
    DWORD                dwReserved;             // reserved
    LPVOID               lpSurface;              // pointer to the associated surface memory
    union
    {
        DDCOLORKEY      ddckCKDestOverlay;      // color key for destination overlay use
        DWORD            dwEmptyFaceColor;       // Physical color for empty cubemap faces
    };
    DDCOLORKEY          ddckCKDestBlt;          // color key for destination blt use
    DDCOLORKEY          ddckCKSrcOverlay;       // color key for source overlay use
    DDCOLORKEY          ddckCKSrcBlt;           // color key for source blt use
    union
    {
        DDPIXELFORMAT   ddpfPixelFormat;        // pixel format description of the surface
        DWORD            dwFVF;                  // vertex format description of vertex buffers
    };
    DDSCAPS2            ddsCaps;                // direct draw surface capabilities
    DWORD                dwTextureStage;         // stage in multitexture cascade
} DDSURFACEDESC2;

#define DDSD_CAPS               0x00000001l     // default
#define DDSD_HEIGHT             0x00000002l
#define DDSD_WIDTH              0x00000004l
#define DDSD_PITCH              0x00000008l
#define DDSD_BACKBUFFERCOUNT    0x00000020l
#define DDSD_ZBUFFERBITDEPTH    0x00000040l
#define DDSD_ALPHABITDEPTH      0x00000080l
#define DDSD_LPSURFACE          0x00000800l
#define DDSD_PIXELFORMAT        0x00001000l
#define DDSD_CKDESTOVERLAY      0x00002000l
#define DDSD_CKDESTBLT          0x00004000l
#define DDSD_CKSRCOVERLAY       0x00008000l
#define DDSD_CKSRCBLT           0x00010000l
#define DDSD_MIPMAPCOUNT        0x00020000l
#define DDSD_REFRESHRATE        0x00040000l
#define DDSD_LINEARSIZE         0x00080000l
#define DDSD_TEXTURESTAGE       0x00100000l
#define DDSD_FVF                0x00200000l
#define DDSD_SRCVBHANDLE        0x00400000l
#define DDSD_DEPTH              0x00800000l
#define DDSD_ALL                0x00fff9eel
#define DDPF_FOURCC                             0x00000004l
#define DDPF_RGB                                0x00000040l
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT2  (MAKEFOURCC('D','X','T','2'))
#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT4  (MAKEFOURCC('D','X','T','4'))
#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))
//
//typedef struct {
//  GLsizei  width;
//  GLsizei  height;
//  GLint    components;
//  GLenum   format;
//  GLint	   numMipmaps;
//  GLubyte *pixels;
//} gliGenericImage;

#endif /* TEXTURES_CREATION_H_ */
