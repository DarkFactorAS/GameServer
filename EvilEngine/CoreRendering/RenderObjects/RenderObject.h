#ifndef EVILENGINE_CORERENDERING_RENDEROBJECT_RENDEROBJECT
#define EVILENGINE_CORERENDERING_RENDEROBJECT_RENDEROBJECT 1

#include "EvilEngine/CoreLib/BasicTypes/Definitions/BasicTypes.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"
#include "EvilEngine/CoreRendering/CoreTextureManager/Texture.h"
#include "EvilEngine/CoreEngine/CoreBaseObject/CoreBaseObject.h"

#define RENDER_BUFFER_SIZE 8

class CoreOpenGLRender;

class RenderObject : public CoreBaseObject
{
public:

  enum Attributes_e
  {
    SHADER_ATTRIBUTE_POSITION,
    SHADER_ATTRIBUTE_TEXTURE_COORD,
    SHADER_ATTRIBUTE_NORMALS,
    SHADER_ATTRIBUTE_COLOR
  };

  virtual         ~RenderObject();

  virtual void                              SetResourceId( uint32 resourceId )                  DF_OVERRIDE;
  virtual void                              SetResourceId( int instanceId, uint32 resourceId );

  virtual bool                              OnDraw( const Matrix& projection );

  void            SetWireframe( bool isWireframe ){ m_Wireframe = isWireframe; }

  virtual void    SetColor( float red, float green, float blue ) DF_OVERRIDE { m_Color.Set( red, green, blue ); }
  virtual void    SetColor( const Color& color ) DF_OVERRIDE { SetColor( color.x, color.y, color.z ); }
  virtual void    SetAlpha( float alpha ) DF_OVERRIDE { m_Color.SetW( alpha ); }

  void            AddPoint( float vertexX, float vertexY, float vertexZ );
  void            AddPoint( const Vector3& position, const Vector3& normal );
  void            AddPoint( const Vector3& vertex, const Vector3& normal, const Vector2& texture );
  void            AddPoint( float vertexX, float vertexY, float vertexZ, const Vector3& normal, float textureScaleX, float textureScaleY );
  void            AddPoint( float vertexX, float vertexY, float vertexZ, float normalX, float normalY, float normalZ, float textureX, float textureY );

  bool            HasLoadedTexture() const{ return (m_Texture!=NULL); }

  static bool     s_DrawWireframe;
  static bool     s_DrawInvisibleAlpha;

  void            AddTexture( Texture* texture ){ m_Texture = texture; }
  void            SetShaderId( int shaderId ){ m_Shader = shaderId; }

  void            AssignShader();
  void            AssignShader( bool useLineShader );

protected:

  RenderObject();
  RenderObject( const Vector3& position, const Vector3& size, uint32_t resourceId );

  virtual void    Init(){};

  void            SetVertices( int numVectices );

  bool            m_Wireframe;

  /// Vertex buffer
  float*          m_Buffer;
  int             m_BufferIdx;
  int             m_BufferSize;

  //bool            m_LoadedTextures;
  bool            m_AssignedShader;
  const Texture*  m_Texture;

  /// Shader members
  int             m_Shader;
  int             m_UniformTexture;
  int             m_Uniform_MVP_Matrix;
  int             m_Uniform_Normals;
  int             m_Uniform_Tint;

  /// Render object properties
  Color           m_Color;

  /// For auto generated resources
  static uint32_t s_ResourceId;
};

#endif /// EVILENGINE_CORERENDERING_RENDEROBJECT_RENDEROBJECT
