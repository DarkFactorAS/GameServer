#ifndef EVILENGINE_CORERENDERING_SHADERMANAGER_SHADERMANAGER
#define EVILENGINE_CORERENDERING_SHADERMANAGER_SHADERMANAGER 1

class RenderObject;
class ShaderResource;

class CoreShaderManager
{
public:

  enum SHADERTYPE
  {
    SHADERTYPE_TEXTURE,
    SHADERTYPE_DEBUGLINE,
  };

  CoreShaderManager();

  static CoreShaderManager* GetInstance();
  bool                      LoadShader( int shaderType, const char* shadername );
  int                       GetTextureShader(){ return m_TextureShader; }
  int                       GetDebugLineShader(){ return m_DebugLineShader; }
  
private:
  
  int                      ShaderBuild( const char* vertex_shader_name, const char* vertex_shader_source, const int vertex_shader_source_length, const char* fragment_shader_name, const char* fragment_shader_source, const int   fragment_shader_source_length );
  int                      ShaderCompile( const int type,  const char* filename, const char* source, const int length);
  int                      ShaderLink(const int vertex_shader, const int fragment_shader);

  static CoreShaderManager* s_Instance;
  int                       m_TextureShader;
  int                       m_DebugLineShader;
};

#endif /// EVILENGINE_CORERENDERING_SHADERMANAGER_SHADERMANAGER
