
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIBASE
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIBASE

//#ifndef CLIENT
//  #pragma error "Gui toolkit is only valid in client builds"
//#endif

#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"
#include "EvilEngine/CoreEngine/CoreBaseObject/CoreBaseObject.h"
#include "EvilEngine/CoreEngine/CoreEffectSystem/EffectPackage.h"
#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include <map>

class RenderObject;
class CoreGuiFrame;
class EffectPackage;

class CoreGuiBase : public CoreBaseObject
{
public:

  enum LAYER
  {
    LAYER_BACKGROUND  = 10,
    LAYER_NORMAL      = 50,
    LAYER_WINDOW      = 100,
    LAYER_DIALOG      = 200,
    LAYER_DEBUGGUI    = 1000,
    LAYER_FRAME       = 1010,
  };

  enum ALIGNMENT
  {
    ALIGN_NONE,
    ALIGN_LEFT,
    ALIGN_TOP,
    ALIGN_RIGHT,
    ALIGN_BOTTOM,
  };

  CoreGuiBase();
  CoreGuiBase(const String& objectName );
  CoreGuiBase( const Vector2& position, const Vector2& size );
  CoreGuiBase( const Vector3& position, const Vector3& size );
  CoreGuiBase( const String& objectName, const Vector3& position, const Vector3& size );
  CoreGuiBase( const String& objectName, const Vector3& position, const Vector3& size, uint32 resourceId );
  CoreGuiBase( const RectF& area );
  ~CoreGuiBase();

  void                                      SetMemberList( const std::vector< KeyValue >& memberList ) { m_Members = memberList; }
  const std::vector< KeyValue >&            GetMemberList() const{ return m_Members; }
  
  void                      SetChildMember( const String& objectName, const String& key, const String& value );
  virtual void              SetMember( const String& key, const String& value );

  bool operator  <   ( const CoreGuiBase& other ) const;
  bool operator()   ( const CoreGuiBase* i,const CoreGuiBase* j);

  virtual bool              OnDraw( const Matrix& projection );
  virtual void              SetWireframe( bool isWireframe ){ m_Wireframe = isWireframe; }

  virtual void              FrameProcess( float /*deltaTime*/ );

  virtual CoreGuiBase*      GetWireGuiUnderMouse( float xPos, float yPos );
  virtual CoreGuiBase*      GetSolidGuiUnderMouse( float xPos, float yPos );

  virtual CoreGuiBase*      OnMouseButtonDown( int  buttonId , float  xPos , float  yPos  );
  virtual CoreGuiBase*      OnMouseButtonUp( int  buttonId , float  xPos , float  yPos  );
  virtual bool              OnMouseButtonWheel( float delta, const Vector2& mousePos );
  virtual bool              OnMouseDrag( float  xPos , float  yPos  );
  virtual bool              OnKeyDown( int keyCode  );
  virtual bool              OnKeyUp( int keyCode );

  virtual bool              GainFocus(){ return false; };
  virtual void              LooseFocus(){};
  virtual bool              CanTakeFocus(){ return false; }
  virtual CoreGuiBase*      SetNextFocus();
  virtual CoreGuiBase*      FindNextFocus( CoreGuiBase* currentObject );
  virtual bool              RequiresKeyboard(){ return false; }
  
  virtual void              SetPosition( float x, float y, float z )          DF_OVERRIDE { m_Position.Set( x, y, z ); UpdateMatrix( UPDATEFLAG_POSITION ); }
  virtual void              SetPosition( const Vector2& position )            DF_OVERRIDE { SetPosition( position.GetX(), position.GetY() ); }
  virtual void              SetPosition( float x, float y )                   DF_OVERRIDE;
  virtual void              Move( float deltaX, float deltaY, bool isLocal )  DF_OVERRIDE;

  virtual void              SetSize( const Vector2& size )                     { SetSize( size.x, size.y ); }
  virtual void              SetSize( float x, float y );
  void                      ScaleDebugFrame( float x, float y );

  virtual void              SetAlpha( float alpha ) DF_OVERRIDE;

  //virtual void              Scale( float scale ) DF_OVERRIDE { Scale2D( scale, scale ); }
  void                      Scale2D( float scaleX, float scaleY );

  void                      SetAlignment( int alignment ){ m_Alignment = alignment; }
  void                      AdjustAlignment( float x, float y );

  void                      SetDebugGui( bool isDebug ){ m_DebugGui = isDebug; }
  bool                      IsDebugGui();

  virtual bool              IsIntersecting( float xPos, float yPos );
  void                      SetLayer( int layer );  // TODO > Rename this to layer category
  void                      SetLayerId( int layer );  // TODO > Rename this to layer category

  RenderObject*             AddRenderObject( RenderObject* renderObject );
  RenderObject*             RemoveRenderObject( RenderObject* renderObject );
  void                      RemoveAllRenderObjects();


  CoreGuiBase*              AddChildWithHack( CoreGuiBase* baseObject, int layer );
  CoreGuiBase*              AddChild( CoreGuiBase* guiObject );
  CoreGuiBase*              AddChildWithLayer( CoreGuiBase* baseObject, int layerId );

  CoreGuiBase*              RemoveAnyChild( const String& guiName );
  CoreGuiBase*              RemoveAnyChild( CoreGuiBase* baseObject );
  CoreGuiBase*              RemoveChild( CoreGuiBase* baseObject );
  CoreGuiBase*              GetChildWithName( const String& guiName );
  void                      RemoveAllChildren();


  CoreGuiBase*              AddDebugFrame();
  CoreGuiBase*              AddDebugFrame( const Color& color );

  void                      RemoveDebugFrame();
  virtual void              ShowBorders( bool showBorders );
  virtual void              SetBorderColor( const Color& color );

  virtual float             GetLeft()     { return ( m_Position.x - m_Size.x ); }
  virtual float             GetRight()    { return ( m_Position.x + m_Size.x ); }
  virtual float             GetTop()      { return ( m_Position.y - m_Size.y ); }
  virtual float             GetBottom()   { return ( m_Position.y + m_Size.y ); }
  float                     GetWidth()    { return GetRight() - GetLeft(); }
  float                     GetHeight()   { return GetBottom() - GetTop(); }

  virtual RectF             GetBoundingBox();
  virtual void              CropToBoundingBox();

  void                      SetParent( CoreGuiBase* parent );
  CoreGuiBase*              GetParent(){ return m_Parent; }
  CoreGuiBase*              GetTopParent();
  CoreGuiBase*              GetXMLParent();

  static float              LAYER_SIZE_Z;
  static float              BORDER;

  virtual void              Destroy() DF_OVERRIDE;
  virtual bool              RunDelete();

  int                       GetLayer() const{ return m_Layer; }

  const String&             GetName() const{ return m_ObjectName; }
  void                      SetName( const String& name );
#if defined DEBUG
  virtual String            GetStaticClassName() DF_OVERRIDE { return "CoreGuiBase"; }
#endif
    
  void                                      SetInToolkit( bool isInToolkit ){ m_IsInToolkit = isInToolkit; }
  bool                                      IsAddedToToolkit(){ return m_IsInToolkit; }
  void                                      SetResourceName( const String& resourceName ){ m_ResourceName = resourceName; }
  const String&                             GetResourceName() const{ return m_ResourceName; }
  void                                      SetXMLResource(){ m_IsXMlResource = true; }
  bool                                      IsXMLResource(){ return m_IsXMlResource; }

  bool                                      IsOrphan() const{ return m_GuiObjects.empty(); }
  const std::vector<CoreGuiBase*>&          GetChildren(){ return m_GuiObjects; }

  EffectPackage*                            CreateEffectPackage( uint32 effectFlags = 0 );
  void                                      RemoveEffectPackage(EffectPackage* removeEffectPackage);

  Signal1<void, CoreGuiBase* >              m_LooseFocus;
  Signal2<void, int, CoreGuiBase* >         m_ActionEvent;

protected:

  void                                      DockTo( const String& dockSibling );

  virtual void                              SetDeleteChild() DF_OVERRIDE;
  void                                      SetValueColor( CoreGuiBase* guiObject, Color& colorMember, const String& value );
  void                                      SetLocalSize(float xSize, float ySize);

  bool                                      m_Wireframe;
  std::vector< CoreGuiBase* >               m_GuiObjects;
  std::vector< RenderObject* >              m_RenderObjects;

  CoreGuiBase*                              m_Parent;
  CoreGuiFrame*                             m_BorderFrame;

  std::map< int, int >                      m_ChildLayer;

  String                                    m_ObjectName;
  bool                                      m_IsInToolkit;
  String                                    m_ResourceName;
  bool                                      m_NeedSort;
  bool                                      m_DebugGui;
  bool                                      m_IsXMlResource;
  int                                       m_Alignment;
  int                                       m_Hotkey;
  Color                                     m_BorderColor;
  Vector2                                   m_DebugFrameSize;
  Vector2                                   m_DebugFramePos;
  String                                    m_DockToSibling;

private:

  CoreGuiBase*                              AddChildInternal( CoreGuiBase* baseObject, int layer );
  int                                       GetLayerId(){ return m_LayerId; }
  int                                       GetLayerType( int layer );

  int                                       m_Layer;
  int                                       m_LayerId;
  int                                       m_AddLayer;

  std::vector< EffectPackage* >             m_EffectPackageList;
  std::vector< KeyValue >                   m_Members;
};

class CoreGuiBaseSorter
{
public:

  CoreGuiBaseSorter()
  {
  }

  bool operator()( const CoreGuiBase* i,const CoreGuiBase* j)
  {
    return i->GetLayer() < j->GetLayer();
  }
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIBASE
