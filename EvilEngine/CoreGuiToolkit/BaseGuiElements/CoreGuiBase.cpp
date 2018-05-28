
#include "PreCompile.h"
#include "CoreGuiBase.h"
#include "CoreRendering/RenderObjects/RenderObject.h"
#include "CoreGuiFrame.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"
#include "EvilEngine/CoreEngine/CoreEffectSystem/EffectPackage.h"

#include <algorithm>

float CoreGuiBase::LAYER_SIZE_Z = 0.01f;
float CoreGuiBase::BORDER       = 0.03f;

CoreGuiBase::CoreGuiBase() :
  m_Wireframe( false ),
  m_Parent( NULL ),
  m_BorderFrame( NULL ),
  m_Layer( 0 ),
  m_LayerId( LAYER_NORMAL ),
  m_IsInToolkit( false ),
  m_NeedSort( false ),
  m_DebugGui( false ),
  m_IsXMlResource( false ),
  m_Alignment( ALIGN_NONE ),
  m_Hotkey( 0 ),
  m_AddLayer( 0 )
{
  m_BorderColor         = Color::Yellow;
}

CoreGuiBase::CoreGuiBase( const String& objectName ) :
  m_Wireframe(false),
  m_Parent(NULL),
  m_BorderFrame(NULL),
  m_Layer(0),
  m_LayerId(LAYER_NORMAL),
  m_IsInToolkit(false),
  m_NeedSort(false),
  m_DebugGui(false),
  m_IsXMlResource(false),
  m_Alignment(ALIGN_NONE),
  m_Hotkey(0),
  m_AddLayer(0)
{
  m_BorderColor = Color::Yellow;
  m_ObjectName = objectName;
}

CoreGuiBase::CoreGuiBase( const Vector2& position, const Vector2& size ) :
  CoreBaseObject( position, size, 0 ),
  m_Wireframe( false ),
  m_Parent( NULL ),
  m_BorderFrame( NULL ),
  m_Layer( 0 ),
  m_LayerId( LAYER_NORMAL ),
  m_IsInToolkit( false ),
  m_NeedSort( false ),
  m_DebugGui( false ),
  m_IsXMlResource( false ),
  m_Alignment( ALIGN_NONE ),
  m_Hotkey( 0 ),
  m_AddLayer( 0 )
{
  m_BorderColor         = Color::Yellow;
}

CoreGuiBase::CoreGuiBase( const Vector3& position, const Vector3& size ) :
  CoreBaseObject( position, size, 0 ),
  m_Wireframe( false ),
  m_Parent( NULL ),
  m_BorderFrame( NULL ),
  m_Layer( 0 ),
  m_LayerId( LAYER_NORMAL ),
  m_IsInToolkit( false ),
  m_NeedSort( false ),
  m_DebugGui( false ),
  m_IsXMlResource( false ),
  m_Alignment( ALIGN_NONE ),
  m_Hotkey( 0 ),
  m_AddLayer( 0 )
{
  m_BorderColor         = Color::Yellow;
}

CoreGuiBase::CoreGuiBase( const String& objectName, const Vector3& position, const Vector3& size ) :
  CoreBaseObject( position, size, 0 ),
  m_Wireframe( false ),
  m_Parent( NULL ),
  m_BorderFrame( NULL ),
  m_Layer( 0 ),
  m_LayerId( LAYER_NORMAL ),
  m_ObjectName( objectName ),
  m_IsInToolkit( false ),
  m_NeedSort( false ),
  m_DebugGui( false ),
  m_IsXMlResource( false ),
  m_Alignment( ALIGN_NONE ),
  m_Hotkey( 0 ),
  m_AddLayer( 0 )
{
  m_BorderColor         = Color::Yellow;
}

CoreGuiBase::CoreGuiBase( const String& objectName, const Vector3& position, const Vector3& size, uint32 resourceId ) :
  CoreBaseObject( position, size, resourceId ),
  m_Wireframe( false ),
  m_Parent( NULL ),
  m_BorderFrame( NULL ),
  m_Layer( 0 ),
  m_LayerId( LAYER_NORMAL ),
  m_ObjectName( objectName ),
  m_IsInToolkit( false ),
  m_NeedSort( false ),
  m_DebugGui( false ),
  m_IsXMlResource( false ),
  m_Alignment( ALIGN_NONE ),
  m_Hotkey( 0 ),
  m_AddLayer( 0 )
{
  m_BorderColor         = Color::Yellow;
}

CoreGuiBase::CoreGuiBase( const RectF& area ) :
  CoreBaseObject( Vector2( area.CenterX(), area.CenterY() ), Vector2( area.Width(), area.Height() ), 0 ),
  m_Wireframe( false ),
  m_Parent( NULL ),
  m_BorderFrame( NULL ),
  m_Layer( 0 ),
  m_LayerId( LAYER_NORMAL ),
  m_IsInToolkit( false ),
  m_NeedSort( false ),
  m_DebugGui( false ),
  m_IsXMlResource( false ),
  m_Alignment( ALIGN_NONE ),
  m_Hotkey( 0 ),
  m_AddLayer( 0 )
{
  m_BorderColor         = Color::Yellow;
}

CoreGuiBase::~CoreGuiBase()
{
  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* baseObject = m_GuiObjects[ index ];
    delete baseObject;
  }
  m_GuiObjects.clear();

  for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
  {
    RenderObject* renderObject = m_RenderObjects[ index ];
    delete renderObject;
  }
  m_RenderObjects.clear();
}

void CoreGuiBase::SetName( const String& name )
{
  m_ObjectName = name; 
}

void CoreGuiBase::SetChildMember( const String& objectName, const String& key, const String& value )
{
  CoreGuiBase* base = GetChildWithName( objectName );
  if ( base != NULL )
  {
    base->SetMember( key, value );
  }
}

void CoreGuiBase::SetMember( const String& key, const String& value )
{
  //LogInfoFMT("CoreGuiBase","[%s] SetMember: %s = %s", GetName().c_str(), key.c_str(), value.c_str() );
  if ( key.CompareNoCase("name") == 0 )
  {
    m_ObjectName = value;
  }
  else if ( key.CompareWithCase("resourceid" ) == 0 )
  {
    uint32 resourceId = atoi( value.c_str() );
    SetResourceId( resourceId );
  }  
  else if ( key.CompareWithCase("layer" ) == 0 )
  {
    m_AddLayer = atoi( value.c_str() );
    SetLayer( m_Layer );
  }  
  else if ( key.CompareWithCase("alpha" ) == 0 )
  {
    float alpha = (float)atof( value.c_str() );
    SetAlpha( alpha );
  }  
  else if ( key.CompareWithCase("color" ) == 0 )
  {
    if ( value.IsNumeric() )
    {
      uint32 color = (uint32)strtol( value.c_str(), NULL, 16 );
      SetColor( color );
    }
    else
    {
      Color color = Color::GetNamedColor( value );
      SetColor( color );
    }
  }
  else if ( key.CompareWithCase("border" ) == 0 )
  {
    if ( value.CompareNoCase("true") == 0 )
    {
      ShowBorders( true );
    }
    else
    {
      ShowBorders( false );
    }
  }  
  else if ( key.CompareWithCase("debugframe" ) == 0 )
  {
#ifdef DEBUG

    if ( !value.empty() )
    {
      m_DebugFrameSize = m_Size;
      m_DebugFramePos  = m_Position;
      CoreGuiBase* frame = AddDebugFrame();
      frame->SetFlag( FLAGS_NOTSCALECHILD );
      Color color;
      SetValueColor(frame, color, value );
    }

#endif
  }
  else if ( key.CompareWithCase("bordercolor" ) == 0 )
  {
    SetValueColor( m_BorderFrame, m_BorderColor, value );
  }
  else if ( key.CompareWithCase("visible" ) == 0 )
  {
    if ( value.CompareNoCase("true") == 0 )
    {
      ClearFlag( CoreBaseObject::FLAGS_ISINVISIBLE );
    }
    else
    {
      SetFlag( CoreBaseObject::FLAGS_ISINVISIBLE );
    }
  }  
  else if ( key.EqualsWithCase("align") )
  {
    if ( value.EqualsWithCase("left") )
    {
      SetAlignment( ALIGN_LEFT );
    }
    else if ( value.EqualsWithCase("right") )
    {
      SetAlignment( ALIGN_RIGHT );
    }
    else if ( value.EqualsWithCase("top") )
    {
      SetAlignment( ALIGN_TOP );
    }
    else if ( value.EqualsWithCase("bottom") )
    {
      SetAlignment( ALIGN_BOTTOM );
    }
  }
  else if ( key.CompareWithCase("size" ) == 0 )
  {
    StringList strList = StringList::Split( value, "," );
    if ( strList.size() == 2 )
    {
      float xSize = (float)atof( strList.at( 0 ).c_str() );
      float ySize = (float)atof( strList.at( 1 ).c_str() );

      SetLocalSize(xSize, ySize);
    }
  }
  else if (key.CompareWithCase("localsize") == 0)
  {
    StringList strList = StringList::Split(value, ",");
    if (strList.size() == 2)
    {
      float xSize = (float)atof(strList.at(0).c_str());
      float ySize = (float)atof(strList.at(1).c_str());

      CoreGuiBase* parent = GetParent();
      dfAssert(parent != NULL, "Could not set localsize wihout a parent!");

      if (parent != NULL)
      {
        const Vector2& parentSize = parent->GetSize();
        xSize = parentSize.x * xSize;
        ySize = parentSize.y * ySize;
        SetLocalSize( xSize, ySize );
      }
    }
  }
  else if ( key.CompareWithCase("position" ) == 0 )
  {
    StringList strList = StringList::Split( value, "," );
    if ( strList.size() == 2 )
    {
      float xPos = (float)atof( strList.at( 0 ).c_str() );
      float yPos = (float)atof( strList.at( 1 ).c_str() );
      SetPosition( xPos, yPos );
    }
  }
  else if (key.CompareWithCase("localposition") == 0)
  {
    StringList strList = StringList::Split(value, ",");
    if (strList.size() == 2)
    {
      float xPos = (float)atof(strList.at(0).c_str());
      float yPos = (float)atof(strList.at(1).c_str());

      CoreGuiBase* parent = GetParent();
      dfAssert( parent != NULL, "Could not set localposition wihout a parent!" );

      if (parent != NULL)
      {
        const Vector2& position = parent->GetPosition();
        const Vector2& size     = parent->GetSize();
        SetPosition( position.x + ( xPos * size.x ), position.y + ( yPos * size.y ) );
      }
    }
  }
  else if ( key.EqualsWithCase("hotkey") )
  {
    m_Hotkey = CoreInputManager::GetInstance()->GetHotkey( value );
    dfAssertFMT( !value.empty() && m_Hotkey != 0, "[%s] Unknown hotkey : '%s'", m_ObjectName.c_str(), value.c_str() );
  }
  else if ( key.EqualsWithCase("ratio") )
  {
    if ( value.EqualsWithCase("true") )
    {
      float screenRatio = CoreOpenGLRender::GetRatio();
      if ( screenRatio > 0.0f )
      {
        const Vector2& size = GetSize();
        float diff = size.x - ( size.x / screenRatio );
        SetSize( size.x / screenRatio, size.y );
        AdjustAlignment( diff, 0 );
      }
      else
      {
        const Vector2& size = GetSize();
        float diff = size.y - ( size.y / screenRatio );
        SetSize( size.x, size.y / screenRatio );
        AdjustAlignment( 0, diff );
      }
    }
  }
  else if ( key.EqualsWithCase("padding") )
  {
    const Vector2& size = GetSize();
    float screenRatio = CoreOpenGLRender::GetRatio();
    float paddingY = (float)atof( value.c_str() );
    float paddingX = paddingY / screenRatio;
    if ( paddingX < size.x && paddingY < size.y )
    {
      SetSize( size.x - paddingX, size.y - paddingY );
    }
    else
    {
      AddDebugFrame( Color::Red );
      LogInfo("CoreGuiBase", "SetMember::Padding : Could not add padding since it is larger than the object" );
    }
  }
  else if ( key.EqualsWithCase("paddingpercent") )
  {
    const Vector2& size = GetSize();
    float padding = (float)atof( value.c_str() );
    if ( padding > 0.0f && padding < 1.0f )
    {
      SetSize( size.x - ( size.x * padding ), size.y - ( size.y * padding ) );
    }
    else
    {
      LogInfo("CoreGuiBase", "SetMember::Padding : Could not add padding since it is larger than the object" );
    }
  }
  else if ( key.EqualsWithCase("squared") )
  {
    if ( value.EqualsWithCase("true") )
    {
      SetFlag( FLAGS_SQUARED );
      const Vector3& size = GetSize();

      float screenRatio = CoreOpenGLRender::GetRatio();
      if ( screenRatio > 0.0f )
      {
        float diffX = size.x - ( size.y / screenRatio );
        float diffY = 0.0f;

        if ( size.x + diffX < size.y )
        {
          diffY = size.y - ( ( size.x - diffX ) * screenRatio );
        }

        SetSize( size.x - diffX, size.y - diffY );
        AdjustAlignment( diffX, diffY );
      }
      else
      {
        float diff = size.y - ( size.y / screenRatio );
        SetSize( size.x, size.y / screenRatio );
        AdjustAlignment( 0, diff );
      }
    }
  }
  else if ( key.EqualsWithCase("dock") )
  {
    StringList strList = StringList::Split(value, ",");
    if (strList.size() == 2)
    {
      float xDock = (float)atof(strList.at(0).c_str());
      float yDock = (float)atof(strList.at(1).c_str());

      const Vector3& pos  = GetPosition();
      const Vector3& size = GetSize();

      SetPosition( pos.x + ( size.x * xDock * 2.0f), pos.y + ( size.y * yDock * 2.0f) );
    }
  }
  else if ( key.EqualsWithCase("dockto") )
  {
    m_DockToSibling = value;
    DockTo( value );
  }
  else
  {
    LogWarningFMT("CoreGuiBase", "[%s] Unknown member variable '%s' with value '%s'", m_ObjectName.c_str(), key.c_str(), value.c_str() );
  }
}

void CoreGuiBase::DockTo( const String& dockSibling )
{
  CoreGuiBase* parent = GetParent();
  if ( parent != NULL )
  {
    CoreGuiBase* sibling = parent->GetChildWithName( dockSibling );
    if ( sibling != NULL )
    {
      const Vector3& siblingSize      = sibling->GetSize();
      const Vector3& siblingPosition  = sibling->GetPosition();
      const Vector3& mySize           = GetSize();
      Vector3 myPosition              = GetPosition();
      if ( siblingPosition.x + siblingSize.x < myPosition.x - mySize.x )
      {
        myPosition.x = siblingPosition.x + siblingSize.x + mySize.x;
      }
      else if ( siblingPosition.x - siblingSize.x > myPosition.x + mySize.x )
      {
        myPosition.x = siblingPosition.x - siblingSize.x - mySize.x;
      }
      SetPosition( myPosition.x, myPosition.y );
    }
  }
}

void CoreGuiBase::SetValueColor( CoreGuiBase* guiObject, Color& colorMember, const String& value )
{
  if ( value.IsNumeric() )
  {
    colorMember = (uint32)strtol( value.c_str(), NULL, 16 );
  }
  else
  {
    colorMember = Color::GetNamedColor( value );
  }
  if ( guiObject != NULL )
  {
    guiObject->SetColor( colorMember );
  }
}

void CoreGuiBase::SetLocalSize( float xSize, float ySize)
{
  if (xSize < 0.0f && ySize > 0.0f)
  {
    xSize = ySize / CoreOpenGLRender::GetRatio();
    SetSize(xSize, ySize);

    AdjustAlignment(0, xSize - ySize);
  }
  else if (ySize < 0.0f && xSize > 0.0f)
  {
    ySize = xSize * CoreOpenGLRender::GetRatio();
    SetSize(xSize, ySize);

    AdjustAlignment(0, (xSize - ySize) * CoreOpenGLRender::GetRatio());
  }
  else if (xSize >= 0 && ySize >= 0)
  {
    SetSize(xSize, ySize);
  }
}

void CoreGuiBase::AdjustAlignment( float x, float y )
{
  //const Vector3& position = GetPosition();

  switch ( m_Alignment )
  {
    case ALIGN_TOP:
      Move( 0, -y, true );
      break;
    case ALIGN_BOTTOM:
      Move( 0, y, true );
      break;
    case ALIGN_LEFT:
      Move( -x, 0, true );
      break;
    case ALIGN_RIGHT:
      Move( x, 0, true );
    default:
      break;
  }
}

bool CoreGuiBase::operator  <   ( const CoreGuiBase& other ) const
{
  return m_LayerId < other.m_LayerId || m_Layer < other.m_Layer; 
}

bool CoreGuiBase::operator()   ( const CoreGuiBase* i,const CoreGuiBase* j)
{
  return i->m_LayerId < j->m_LayerId || i->m_Layer < j->m_Layer;
}

CoreGuiBase* CoreGuiBase::GetWireGuiUnderMouse( float xPos, float yPos )
{
  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* guiObject    = m_GuiObjects[ index ];
    CoreGuiBase* focusObject  = guiObject->GetWireGuiUnderMouse( xPos, yPos );
    if ( focusObject != NULL )
    {
      return focusObject;
    }
  }

  if ( m_Wireframe && IsIntersecting( xPos, yPos ) )
  {
    return this;
  }
  return NULL;
}

CoreGuiBase* CoreGuiBase::GetSolidGuiUnderMouse( float xPos, float yPos )
{
  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject    = m_GuiObjects[ index - 1 ];
    CoreGuiBase* focusObject  = guiObject->GetSolidGuiUnderMouse( xPos, yPos );
    if ( focusObject != NULL )
    {
      return focusObject;
    }
  }

  if ( !m_Wireframe  && IsIntersecting( xPos, yPos ) )
  {
    return this;
  }
  return NULL;
}

CoreGuiBase* CoreGuiBase::OnMouseButtonDown( int  buttonId , float  xPos , float  yPos  )
{
  for ( size_t index = m_GuiObjects.size(); index > 0 ; index-- )
  {
    CoreGuiBase* guiObject   = m_GuiObjects[ index - 1 ];
    if ( guiObject->IsVisible() )
    {
      CoreGuiBase* focusObject = guiObject->OnMouseButtonDown( buttonId, xPos, yPos );
      if ( focusObject != NULL )
      {
        return focusObject;
      }
      if ( guiObject->IsModal() )
      {
        return guiObject;
      }
    }
  }
  return NULL;
}

CoreGuiBase* CoreGuiBase::OnMouseButtonUp( int  buttonId , float  xPos , float  yPos  )
{
  for ( size_t index = m_GuiObjects.size(); index > 0 ; index-- )
  {
    CoreGuiBase* guiObject   = m_GuiObjects[ index - 1 ];
    CoreGuiBase* focusObject = guiObject->OnMouseButtonUp( buttonId, xPos, yPos );
    if ( focusObject != NULL )
    {
      return focusObject;
    }
  }

  return NULL;
}

bool CoreGuiBase::OnMouseDrag( float  xPos , float  yPos  )
{
  for ( size_t index = m_GuiObjects.size(); index > 0 ; index-- )
  {
    CoreGuiBase* guiObject   = m_GuiObjects[ index - 1 ];
    if ( guiObject->OnMouseDrag( xPos, yPos ) )
    {
      return true;
    }
  }
  return false;
}

bool CoreGuiBase::OnKeyDown( int keyCode  )
{
  for ( size_t index = m_GuiObjects.size(); index > 0 ; index-- )
  {
    CoreGuiBase* guiObject   = m_GuiObjects[ index - 1 ];
    if ( guiObject->OnKeyDown( keyCode ) )
    {
      return true;
    }
  }
  return false;
}

bool CoreGuiBase::OnKeyUp( int keyCode )
{
  for ( size_t index = m_GuiObjects.size(); index > 0 ; index-- )
  {
    CoreGuiBase* guiObject   = m_GuiObjects[ index - 1 ];
    if ( guiObject->OnKeyUp( keyCode ) )
    {
      return true;
    }
  }
  return false;
}


bool CoreGuiBase::OnMouseButtonWheel( float delta, const Vector2& mousePos )
{
  for ( size_t index = m_GuiObjects.size(); index > 0 ; index-- )
  {
    CoreGuiBase* guiObject   = m_GuiObjects[ index - 1 ];
    if ( guiObject->OnMouseButtonWheel( delta, mousePos ) )
    {
      return true;
    }
  }
  return false;
}

bool CoreGuiBase::IsIntersecting( float xPos, float yPos )
{
  if ( xPos >= m_Position.x - m_Size.x && xPos <= m_Position.x + m_Size.x && yPos >= m_Position.y - m_Size.y && yPos <= m_Position.y + m_Size.y )
  {
    return true;
  }

  //for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  //{
  //  CoreGuiBase* guiObject = m_GuiObjects[ index ];
  //  guiObject->SetLayer( layer + 1 );
  //}

  return false;
}

void CoreGuiBase::SetLayer( int layer )
{
  m_Layer   = layer + m_AddLayer;
  m_LayerId = GetLayerType( layer );
  //CoreBaseObject::SetPosition( m_Position.x, m_Position.y, m_Layer * LAYER_SIZE_Z );


  //SetPosition( m_Position.x, m_Position.y );

  //for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  //{
  //  CoreGuiBase* guiObject = m_GuiObjects[ index ];
  //  if ( guiObject->GetLayerId() <= m_LayerId )
  //  {
  //    guiObject->SetLayer( layer + 1 + (int)index );
  //  }
  //}

  //for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
  //{
  //  RenderObject* renderObject = m_RenderObjects[ index ];
  //  const Vector3& pos = renderObject->GetPosition();
  //  //renderObject->SetPosition( pos.x, pos.y, m_Layer * LAYER_SIZE_Z );
  //}
}

void CoreGuiBase::FrameProcess( float deltaTime )
{
  // Run frame process on all children
  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* guiObject = m_GuiObjects[ index ];
    guiObject->FrameProcess( deltaTime );
  }

  // Run effects
  for (std::vector< EffectPackage* >::iterator itEffectPackage = m_EffectPackageList.begin(); itEffectPackage != m_EffectPackageList.end(); )
  {
    EffectPackage* effectPackage = *itEffectPackage;
    if (!effectPackage->ShouldDestroy() && effectPackage->Run(deltaTime))
    {
      itEffectPackage++;
    }
    else
    {
      delete effectPackage;
      itEffectPackage = m_EffectPackageList.erase(itEffectPackage);
    }
  }
}


/************************************************************************************************
* AddChild:
* Add a gui object as a child of this one. The effect is that it will follow this object ( also 
* when deleted from the scene ) and always be rendered on top of its parent.
*
* @param  (CoreGuiBase*)   baseObject    - The gui object to add as a child
* @return (CoreGuiBase*)                 - The gui object that was just added as a child
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiBase* CoreGuiBase::AddDebugFrame()
{
  return AddDebugFrame( Color::Yellow );
}
CoreGuiBase* CoreGuiBase::AddDebugFrame( const Color& color )
{
  RemoveDebugFrame();
  return AddChild( new CoreGuiFrame( "DebugFrame", GetPosition(), GetSize(), color ) );
}

void CoreGuiBase::RemoveDebugFrame()
{
  RemoveChild( GetChildWithName("DebugFrame") );
}

void CoreGuiBase::ShowBorders( bool showBorders )
{
  if ( showBorders && m_BorderFrame == NULL )
  {
    m_BorderFrame = new CoreGuiFrame( "Border", GetPosition(), GetSize(), m_BorderColor );
    AddChildWithHack( m_BorderFrame, LAYER_FRAME );
  }
  else if ( !showBorders && m_BorderFrame != NULL )
  {
    RemoveChild( m_BorderFrame );
    m_BorderFrame = NULL;
  }
}

void CoreGuiBase::SetBorderColor( const Color& color )
{
  CoreGuiBase* frame = GetChildWithName("Border");
  if ( frame != NULL )
  {
    frame->SetColor( color );
  }
}

RectF CoreGuiBase::GetBoundingBox()
{
  RectF rectf = RectF( GetLeft(), GetTop(), GetRight(), GetBottom() );

  bool ignoreThis = rectf.m_X == rectf.m_Width || rectf.m_Y == rectf.m_Height;

  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    //if ( !guiObject->HasFlag( CoreBaseObject::FLAGS_ISINVISIBLE ) )
    {
      RectF childRect = guiObject->GetBoundingBox();
      if ( childRect.m_X != childRect.m_Width && childRect.m_Y != childRect.m_Height )
      {
        if ( ignoreThis || childRect.m_X < rectf.m_X ){ rectf.m_X = childRect.m_X; }
        if ( ignoreThis || childRect.m_Width > rectf.m_Width ){ rectf.m_Width = childRect.m_Width; }
        if ( ignoreThis || childRect.m_Y < rectf.m_Y ){ rectf.m_Y = childRect.m_Y; }
        if ( ignoreThis || childRect.m_Height > rectf.m_Height ){ rectf.m_Height = childRect.m_Height; }
      }
    }
  }

  return rectf;
}

void CoreGuiBase::CropToBoundingBox()
{
  m_Size.x      = 0;
  m_Size.y      = 0;
  m_Position.x  = 0;
  m_Position.y  = 0;

  RectF bbox = GetBoundingBox();

  m_Size.x      = ( bbox.m_Width - bbox.m_X ) * 0.5f;
  m_Size.y      = ( bbox.m_Height - bbox.m_Y ) * 0.5f;
  m_Position.x  = bbox.m_X + m_Size.x;
  m_Position.y  = bbox.m_Y + m_Size.y;
}

int CoreGuiBase::GetLayerType( int layer )
{
  if ( layer >= LAYER_DEBUGGUI )
  {
    return LAYER_DEBUGGUI;
  }
  else if ( layer >= LAYER_DIALOG )
  {
    return LAYER_DIALOG;
  }
  else if ( layer >= LAYER_WINDOW )
  {
    return LAYER_WINDOW;
  }
  else if ( layer >= LAYER_NORMAL )
  {
    return LAYER_NORMAL;
  }
  else if ( layer >= LAYER_BACKGROUND )
  {
    return LAYER_BACKGROUND;
  }
  else if ( layer > 0 )
  {
    return 0;
  }
  return LAYER_NORMAL;
}

CoreGuiBase* CoreGuiBase::AddChildWithHack( CoreGuiBase* baseObject, int layerId )
{
  return AddChildInternal( baseObject, layerId );
}

/************************************************************************************************
* AddChild:
* Add a gui object as a child of this one. The effect is that it will follow this object ( also 
* when deleted from the scene ) and always be rendered on top of its parent.
*
* @param  (CoreGuiBase*)   baseObject    - The gui object to add as a child
* @return (CoreGuiBase*)                 - The gui object that was just added as a child
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiBase* CoreGuiBase::AddChild( CoreGuiBase* baseObject )
{
  return AddChildInternal( baseObject, 50 );
}

CoreGuiBase* CoreGuiBase::AddChildWithLayer( CoreGuiBase* baseObject, int layerId )
{
  return AddChildInternal( baseObject, layerId );
}

CoreGuiBase* CoreGuiBase::AddChildInternal( CoreGuiBase* baseObject, int layer )
{
  if ( baseObject != NULL )
  {
    dfAssert( baseObject->GetParent() == NULL, "Cannot add non top level gui objects as a child!");
    dfAssert( !baseObject->IsAddedToToolkit(), "This gui object is already added to the gui toolkit!");

    /*
    // Find the correct layer
    int layerType = GetLayerType( layer );

#ifdef DEBUG
    if ( layerType > 0 && m_ChildLayer[ 0 ] > 0 )
    {
      LogInfoFMT("CoreGuiBase","LayerId[%d] childLayer[%d]", layerType, m_ChildLayer[0]);
    }
#endif

    int childLayer = m_ChildLayer[ layerType ];
    if ( childLayer == 0 )
    {
      childLayer = layerType;
    }

    // If the layer is too low, increase it to match this guis layer
    if ( m_Layer > childLayer )
    {
      layerType = GetLayerType( m_Layer );
      childLayer = m_ChildLayer[ layerType ];
      if ( childLayer == 0 )
      {
        childLayer = layerType + 1;
      }
      if ( m_Layer > childLayer )
      {
        childLayer = m_Layer + 1;
      }
    }
    else
    {
      m_Layer = childLayer;
    }

    if ( layer > childLayer )
    {
      childLayer = layer;
    }
    else if ( baseObject->GetLayer() <= childLayer || ( layer < childLayer && layer > 0 ) )
    {
      childLayer = childLayer;
      //baseObject->SetLayer( ++childLayer );
    }
    else
    {
      childLayer = baseObject->GetLayer();
    }
    baseObject->SetLayer( ++childLayer );
    baseObject->SetParent( this );
    baseObject->SetFlag( FLAGS_UPDATELAYER );
    //baseObject->UpdateParentAndSiblingLayer();
    m_GuiObjects.push_back( baseObject );

#ifdef DEBUG
    if ( layerType > 0 && m_ChildLayer[ 0 ] > 0 )
    {
      LogInfoFMT("CoreGuiBase","LayerId[%d] childLayer[%d]", layerType, m_ChildLayer[0]);
    }
#endif

    //if ( baseObject->GetLayer() > childLayer )
    {
      m_ChildLayer[ layerType ] = childLayer;
      //m_Layer = baseObject->GetLayer();
    }

    */

    baseObject->m_Layer = layer;

    //baseObject->SetLayer( layer );
    baseObject->SetParent( this );
    m_GuiObjects.push_back( baseObject );

    if ( baseObject->HasDeleteChild() )
    {
      SetDeleteChild();
    }
    m_NeedSort = true;
  }
  return baseObject;
}

RenderObject* CoreGuiBase::AddRenderObject( RenderObject* renderObject )
{
  dfAssert( renderObject->GetParent() == NULL, "Renderobject already added to the gui" );
  renderObject->SetParent( this );

  //const Vector3& position = renderObject->GetPosition();
  //renderObject->SetPosition( position.x, position.y, ( m_Layer + 1 ) * LAYER_SIZE_Z );

  m_RenderObjects.push_back( renderObject );
  return renderObject;
}

RenderObject* CoreGuiBase::RemoveRenderObject( RenderObject* renderObject )
{
  if ( renderObject == NULL )
  {
    return NULL;
  }
  for ( std::vector< RenderObject* >::iterator itObj = m_RenderObjects.begin(); itObj != m_RenderObjects.end(); ++itObj )
  {
    RenderObject* localRenderObject = *itObj;
    if ( localRenderObject == renderObject )
    {
      m_RenderObjects.erase( itObj );
      delete renderObject;
      return NULL;
    }
  }
  dfAssert( false, "CoreGuiBase::RemoveRenderObject : Did not find renderObject!" );
  return NULL;
}

void CoreGuiBase::RemoveAllRenderObjects()
{
  for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
  {
    RenderObject* renderObject = m_RenderObjects[ index ];
    delete renderObject;
  }
  m_RenderObjects.clear();
}

CoreGuiBase* CoreGuiBase::GetChildWithName( const String& guiName )
{
  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    if ( guiObject->HasDestroy() )
    {
      continue;
    }
    if ( guiObject->GetName().CompareNoCase( guiName ) == 0 )
    {
      return guiObject;
    }
    CoreGuiBase* child = guiObject->GetChildWithName( guiName );
    if ( child != NULL )
    {
      return child;
    }
  }
  return NULL;
}

/************************************************************************************************
* RemoveAllChildren:
* Remove all gui elements that is a child of this one. The effect is that it will stop following this object ( also 
* when deleted from the scene ) and always be rendered on top of its parent.
*
* @param  (CoreGuiBase*)   baseObject    - The gui object to add as a child
* @return (CoreGuiBase*)                 - The gui object that if it was not removed
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGuiBase::RemoveAllChildren()
{
  bool deleteChild = false;
  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    guiObject->Destroy();
    guiObject->SetParent( NULL );
  }

  if ( deleteChild )
  {
    SetDeleteChild();
  }
}

/************************************************************************************************
* RemoveChild:
* Remove a gui object as a child of this one. The effect is that it will stop following this object ( also 
* when deleted from the scene ) and always be rendered on top of its parent.
*
* @param  (CoreGuiBase*)   baseObject    - The gui object to add as a child
* @return (CoreGuiBase*)                 - The gui object that if it was not removed
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiBase* CoreGuiBase::RemoveChild( CoreGuiBase* baseObject )
{
  if ( baseObject == NULL )
  {
    return NULL;
  }
  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    if ( guiObject == baseObject )
    {
      SetDeleteChild();
      guiObject->Destroy();
      guiObject->SetParent( NULL );
      return NULL;
    }
  }
  return baseObject;
}

CoreGuiBase* CoreGuiBase::RemoveAnyChild( const String& guiName )
{
  return RemoveAnyChild( GetChildWithName( guiName ) );
}

/************************************************************************************************
* RemoveAnyChild:
* Remove a gui object as a child of this one or a child of my child. The effect is that it will 
* stop following this object ( also when deleted from the scene ) and always be rendered on top 
* of its parent.
*
* @param  (CoreGuiBase*)   baseObject    - The gui object to add as a child
* @return (CoreGuiBase*)                 - The gui object that was just removed as a child
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiBase* CoreGuiBase::RemoveAnyChild( CoreGuiBase* baseObject )
{
  if ( baseObject == NULL )
  {
    return NULL;
  }
  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    if ( guiObject == baseObject )
    {
      SetDeleteChild();
      guiObject->Destroy();
      guiObject->SetParent( NULL );
      return baseObject;
    }

    CoreGuiBase* child = guiObject->RemoveAnyChild( baseObject );
    if ( child != NULL )
    {
      return baseObject;
    }
  }
  return NULL;
}

CoreGuiBase* CoreGuiBase::SetNextFocus()
{
  // If this object, move on to the parent
  if ( m_Parent != NULL )
  {
    return m_Parent->FindNextFocus( this );
  }
  return FindNextFocus( this );
}

CoreGuiBase* CoreGuiBase::FindNextFocus( CoreGuiBase* currentObject )
{
  if ( CanTakeFocus() )
  {
    return this;
  }

  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    if ( guiObject == currentObject )
    {
      currentObject = NULL;
    }
    else if ( currentObject == NULL )
    {
      currentObject = guiObject->FindNextFocus( NULL );
      if ( currentObject != NULL )
      {
        return currentObject;
      }
    }
  }
  return NULL;
}


void CoreGuiBase::SetPosition( float x, float y )
{
  if ( m_ObjectName.Equals("playerbg") )
  {
    LogInfoFMT("CoreGuiBase", "Move from %2.0f x %0.2f => %0.2f x %0.2f", m_Position.x, m_Position.y, x, y );
  }
  if ( m_ObjectName.Equals("action") )
  {
    LogInfoFMT("CoreGuiBase", "Move from %2.0f x %0.2f => %0.2f x %0.2f", m_Position.x, m_Position.y, x, y );
  }

  if ( m_Position.x != x || m_Position.y != y )
  {
    for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
    {
      CoreGuiBase* guiObject = m_GuiObjects[ index ];
      guiObject->Move( x - m_Position.x, y - m_Position.y, false );
      if ( !guiObject->m_DockToSibling.empty() )
      {
        guiObject->DockTo( guiObject->m_DockToSibling );
      }
    }
    for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
    {
      RenderObject* renderObject = m_RenderObjects[ index ];
      renderObject->Move( x - m_Position.x, y - m_Position.y, false );
    }
  }
  CoreBaseObject::SetPosition( x, y, m_Position.z );
}

void CoreGuiBase::Move( float deltaX, float deltaY, bool isLocal )
{
  if ( ( deltaX != 0.0f || deltaY != 0.0f ) && ( !isLocal || !HasFlag( FLAGS_NOTSCALECHILD ) ) )
  {
    for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
    {
      CoreGuiBase* guiObject = m_GuiObjects[ index ];
      guiObject->Move( deltaX, deltaY, isLocal );
      if ( !guiObject->m_DockToSibling.empty() )
      {
        guiObject->DockTo( guiObject->m_DockToSibling );
      }
    }

    for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
    {
      RenderObject* renderObject = m_RenderObjects[ index ];
      renderObject->Move( deltaX, deltaY, isLocal );
    }

    CoreBaseObject::SetPosition( m_Position.x + deltaX, m_Position.y + deltaY, m_Position.z );
    //m_Position.Set( m_Position.x + deltaX, m_Position.y + deltaY, m_Position.z );
  }
}

void CoreGuiBase::SetSize( float x, float y )
{
  if ( m_Size.x != 0 && m_Size.y != 0 )
  {
    float divX = x / m_Size.x;
    float divY = y / m_Size.y;

    for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
    {
      CoreGuiBase* guiObject = m_GuiObjects[ index ];
      if ( !guiObject->HasFlag( FLAGS_NOTSCALECHILD ) )
      {
        const Vector3& pos     = guiObject->GetPosition();
        guiObject->SetPosition( m_Position.x + ( pos.x - m_Position.x ) * divX, m_Position.y - ( m_Position.y - pos.y ) * divY );
        guiObject->Scale2D( divX, divY );
        if ( guiObject->HasFlag( FLAGS_SQUARED ) )
        {
          guiObject->SetMember( "squared","true" );
        }
      }
    }
  }
  else
  {
    for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
    {
      CoreGuiBase* guiObject = m_GuiObjects[ index ];
      guiObject->Scale2D( x, y );
    }
  }

  m_Size.Set( x, y, 0 );
}

void CoreGuiBase::ScaleDebugFrame( float x, float y )
{
  if ( !m_DebugFrameSize.IsNull() )
  {
    CoreGuiBase* frame = GetChildWithName("DebugFrame");
    if ( frame != NULL )
    {
      frame->SetSize( m_DebugFrameSize.x * x, m_DebugFrameSize.y * y );
    }
  }
  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* guiObject = m_GuiObjects[ index ];
    guiObject->ScaleDebugFrame( x, y );
  }
}

void CoreGuiBase::SetAlpha( float alpha )
{
  CoreBaseObject::SetAlpha( alpha );

  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* guiObject = m_GuiObjects[ index ];
    guiObject->SetAlpha( alpha );
  }
}

void CoreGuiBase::Scale2D( float scaleX, float scaleY )
{
  if ( m_Size.x != 0 || m_Size.y != 0 )
  {
    SetSize( m_Size.x * scaleX, m_Size.y * scaleY );
  }
  else
  {
    SetSize( scaleX, scaleY );
  }

  //for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  //{
  //  CoreGuiBase* guiObject = m_GuiObjects[ index ];
  //  guiObject->Scale( scaleX, scaleY );
  //}
}

bool CoreGuiBase::OnDraw( const Matrix& projection )
{
  if ( m_NeedSort )
  {
    m_NeedSort = false;
    CoreGuiBaseSorter sorter;
    std::sort( m_GuiObjects.begin(), m_GuiObjects.end(), sorter );
  }

  for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
  {
    RenderObject* renderObject = m_RenderObjects[ index ];
    renderObject->OnDraw( projection );
  }

  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* guiObject = m_GuiObjects[ index ];
    if ( !guiObject->HasFlag( FLAGS_ISINVISIBLE ) )
    {
      guiObject->OnDraw( projection );
    }
  }

  return true;
}

bool CoreGuiBase::RunDelete()
{
  if ( HasDeleteChild() )
  {
    for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); )
    {
      CoreGuiBase* guiObject = *itObj;
      if ( guiObject->HasDestroy() )
      {
        itObj = m_GuiObjects.erase( itObj );
        delete guiObject;
        continue;
      }
      else if ( guiObject->HasDeleteChild() )
      {
        guiObject->RunDelete();
      }
      itObj++;
    }
    m_DeleteChild = false;
    //SetDeleteChild();
  }
  return HasDestroy();
}

void CoreGuiBase::Destroy()
{
  CoreBaseObject::Destroy();
  if ( m_Parent != NULL )
  {
    m_Parent->SetDeleteChild();
  }
}

void CoreGuiBase::SetDeleteChild()
{
  CoreBaseObject::SetDeleteChild();
  if ( m_Parent != NULL )
  {
    m_Parent->SetDeleteChild();
  }
}

void CoreGuiBase::SetParent( CoreGuiBase* parent )
{
#if defined DEBUG && defined PLATFORM_WINDOWS
  CoreBaseObject::SetParent( parent );
#endif
  m_Parent = parent; 
}

CoreGuiBase* CoreGuiBase::GetTopParent()
{
  if ( m_Parent != NULL )
  {
    return m_Parent->GetTopParent();
  }
  return this;
}

CoreGuiBase* CoreGuiBase::GetXMLParent()
{
  if ( !m_ResourceName.empty() )
  {
    return this;
  }
  if ( m_Parent != NULL )
  {
    return m_Parent->GetXMLParent();
  }
  return NULL;
}

bool CoreGuiBase::IsDebugGui()
{
  if ( m_DebugGui )
  {
    return true;
  }

  if ( m_Parent != NULL )
  {
    return m_Parent->IsDebugGui();
  }

  return false;
}

/************************************************************************************************
* Create an effect package and add it to the CoreBaseObject:
*
* @param  (CoreBaseObject*)   runObject - The object that we want to create an effect package on
*
* @author Thor Richard Hansen
*************************************************************************************************/
EffectPackage* CoreGuiBase::CreateEffectPackage( uint32 effectFlags )
{
  EffectPackage* effectPackage = new EffectPackage(this, effectFlags);
  m_EffectPackageList.push_back(effectPackage);
  return effectPackage;
}

/************************************************************************************************
* RemoveEffectPackage:
* Removes an effectpackage from the run list. The effectpackage will live until all effects in it
* is done executing
*
* @param  (const EffectPackage*)   effectPackage - The effectpackage to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGuiBase::RemoveEffectPackage(EffectPackage* removeEffectPackage)
{
  for (std::vector< EffectPackage* >::iterator itEffectPackage = m_EffectPackageList.begin(); itEffectPackage != m_EffectPackageList.end(); ++itEffectPackage)
  {
    EffectPackage* effectPackage = *itEffectPackage;
    if (effectPackage->GetId() == removeEffectPackage->GetId())
    {
      m_EffectPackageList.erase(itEffectPackage);
      delete effectPackage;
      return;
    }
  }
}
