

#include "PreCompile.h"
#include "CoreGuiTileView.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "../BaseGuiElements/CoreGuiImageView.h"
#include "../BaseGuiElements/CoreGuiButton.h"
#include "EvilEngine/CoreRendering/CoreTextureManager/CoreTextureManager.h"
#include "EvilEngine/CoreRendering/RenderObjects/QuadRenderObject.h"

#include <stdlib.h>

CoreGuiTileView::CoreGuiTileView( const Vector2& position, const Vector2& size, const Vector2& tileSize, int layout ) :
  CoreGuiBase( "CoreGuiTileView", position, size ),
  m_TileSize( tileSize ),
  m_Layout( layout ),
  m_NeedsUpdate( false ),
  m_Offset( 0.0f ),
  m_ScrollSpeed( 0.0f ),
  m_SelectedTile( NULL ),
  m_Framebuffer( NULL ),
  m_FramebufferView( NULL ),
  m_OffsetIndex( 0 ),
  m_VisibleTiles( 100 )
{
  Init();
}

CoreGuiTileView::CoreGuiTileView( const String& objectName, const Vector2& position, const Vector2& size, const Vector2& tileSize, int layout ) :
  CoreGuiBase( objectName, position, size ),
  m_TileSize( tileSize ),
  m_Layout( layout ),
  m_NeedsUpdate( false ),
  m_Offset( 0.0f ),
  m_ScrollSpeed( 0.0f ),
  m_SelectedTile( NULL ),
  m_Framebuffer( NULL ),
  m_FramebufferView( NULL ),
  m_OffsetIndex( 0 ),
  m_VisibleTiles( 100 )
{
  Init();
}

CoreGuiTileView::~CoreGuiTileView()
{
  delete m_Arrow1;
  delete m_Arrow2;

  for ( size_t index = 0; index < m_Tiles.size(); ++index )
  {
    delete m_Tiles[ index ];
    m_Tiles[ index ] = NULL;
  }
}

void CoreGuiTileView::SetMember( const String& key, const String& value )
{
  if ( key.CompareWithCase("tilesize" ) == 0 )
  {
    StringList strList = StringList::Split( value, "," );
    if ( strList.size() == 2 )
    {
      m_TileSize.x  = (float)atof( strList.at( 0 ).c_str() );
      m_TileSize.y  = (float)atof( strList.at( 1 ).c_str() );
      m_NeedsUpdate = false;
    }
  }
  else if ( key.EqualsWithCase("visibletiles") )
  {
    m_VisibleTiles  = (int)atoi( value.c_str() );
  }
  else if ( key.EqualsWithCase("layout") )
  {
    if ( value.EqualsWithCase("vertical") )
    {
      m_Layout      = LAYOUT_VERTICAL;
      m_NeedsUpdate = false;
    }
    else
    {
      m_Layout      = LAYOUT_HORIZONTAL;
      m_NeedsUpdate = false;
    }
  }
  else
  {
    CoreGuiBase::SetMember( key, value );
  }
}

void CoreGuiTileView::CreateFrameBuffer()
{
#if !defined PLATFORM_IOS && !defined PLATFORM_OSX

  CHECK_OPENGL

  // Tiny hax for now. Should get the proper screensize somewhere
  GLint m_viewport[4];
  glGetIntegerv( GL_VIEWPORT, m_viewport );

  const Vector3& size = GetSize();
  uint32 pixSizeX = (uint32)(m_viewport[2] * size.x);
  uint32 pixSizeY = (uint32)(m_viewport[3] * size.y);

  if ( m_Framebuffer == NULL || ( m_Framebuffer->m_Width != pixSizeX && m_Framebuffer->m_Height != pixSizeY ) )
  {
    if ( m_FramebufferView != NULL )
    {
      SetPosition( m_FramebufferView->GetPosition().x, m_FramebufferView->GetPosition().y );
      RemoveRenderObject( m_FramebufferView );
      m_FramebufferView = NULL;
    }

    Vector3 pos = GetPosition();
    if ( m_FramebufferView != NULL )
    {
      pos = m_FramebufferView->GetPosition();
    }
    CoreTextureManager::DestroyFrameBufferTexture( m_Framebuffer );
    //m_Framebuffer = CoreTextureManager::CreateFrameBufferTexture( pixSizeX, pixSizeY );
    if ( m_Framebuffer != NULL )
    {
      m_FramebufferView = new QuadRenderObject( pos, GetSize(), 0 );
      m_FramebufferView->AssignShader( false );
      m_FramebufferView->AddTexture( new Texture( 0, m_Framebuffer->m_TextureId ) );
      AddRenderObject( m_FramebufferView );

      // Adjust the size for all tiles
      float scaleX = 1.0f / m_TileSize.x;
      float scaleY = scaleX * CoreOpenGLRender::GetRatio();
      for ( size_t index = 0; index < m_Tiles.size(); index++ )
      {
        CoreGuiBase* tile    = m_Tiles[ index ];
        tile->SetSize( m_TileSize.x * scaleX , m_TileSize.y * scaleY );
      }
      return;
    }
  }

  for ( size_t index = 0; index < m_Tiles.size(); index++ )
  {
    CoreGuiBase* tile    = m_Tiles[ index ];
    tile->SetSize( m_TileSize.x * m_Size.x, m_TileSize.y * m_Size.y );
  }

  CHECK_OPENGL
#endif
}

void CoreGuiTileView::Init()
{
  m_NormalColor   = Color::DarkGrey;
  m_SelectedColor = Color::Grey;

  switch ( m_Layout )
  {
  case LAYOUT_HORIZONTAL:
    m_Arrow1 = new CoreGuiButton( String::zero, Vector2( m_Position.x - m_Size.x + ( m_TileSize.x * 0.5f ), m_Position.y ), Vector2( m_TileSize.x * 0.25f, m_TileSize.y -  CoreGuiBase::BORDER ), GuiResource_TileViewArrowLeft );
    m_Arrow2 = new CoreGuiButton( String::zero, Vector2( m_Position.x + m_Size.x - ( m_TileSize.x * 0.5f ), m_Position.y ), Vector2( m_TileSize.x * 0.25f, m_TileSize.y -  CoreGuiBase::BORDER ), GuiResource_TileViewArrowRight );
    //AddChild( m_Arrow1 );
    //AddChild( m_Arrow2 );
    m_Arrow1->m_SignalButtonClicked.Connect( this, &CoreGuiTileView::ScrollRight );
    m_Arrow1->m_SignalButtonReleased.Connect( this, &CoreGuiTileView::ScrollStop );
    m_Arrow2->m_SignalButtonClicked.Connect( this, &CoreGuiTileView::ScrollLeft );
    m_Arrow2->m_SignalButtonReleased.Connect( this, &CoreGuiTileView::ScrollStop );
    break;
  case LAYOUT_VERTICAL:
    m_Arrow1 = new CoreGuiButton( String::zero, Vector2( m_Position.x - m_Size.x + ( m_TileSize.x * 0.5f ), m_Position.y ), Vector2( m_TileSize.x * 0.25f, m_TileSize.y -  CoreGuiBase::BORDER ), GuiResource_TileViewArrowLeft );
    m_Arrow2 = new CoreGuiButton( String::zero, Vector2( m_Position.x + m_Size.x - ( m_TileSize.x * 0.5f ), m_Position.y ), Vector2( m_TileSize.x * 0.25f, m_TileSize.y -  CoreGuiBase::BORDER ), GuiResource_TileViewArrowRight );
    //AddChild( m_Arrow1 );
    //AddChild( m_Arrow2 );
    m_Arrow1->m_SignalButtonClicked.Connect( this, &CoreGuiTileView::ScrollRight );
    m_Arrow1->m_SignalButtonReleased.Connect( this, &CoreGuiTileView::ScrollStop );
    m_Arrow2->m_SignalButtonClicked.Connect( this, &CoreGuiTileView::ScrollLeft );
    m_Arrow2->m_SignalButtonReleased.Connect( this, &CoreGuiTileView::ScrollStop );
    break;
  }

  CreateFrameBuffer();
}

void CoreGuiTileView::ReInit()
{
  m_Arrow1->SetSize( m_TileSize.x * 0.25f, m_TileSize.y -  CoreGuiBase::BORDER );
  m_Arrow2->SetSize( m_TileSize.x * 0.25f, m_TileSize.y -  CoreGuiBase::BORDER );

  switch ( m_Layout )
  {
  case LAYOUT_HORIZONTAL:
    m_Arrow1->SetPosition( m_Position.x - m_Size.x + ( m_TileSize.x * 0.5f ), m_Position.y );
    m_Arrow2->SetPosition( m_Position.x + m_Size.x - ( m_TileSize.x * 0.5f ), m_Position.y );
    break;
  case LAYOUT_VERTICAL:
    m_Arrow1->SetPosition( m_Position.x, m_Position.y - m_Size.x + ( m_TileSize.x * 0.5f ) );
    m_Arrow2->SetPosition( m_Position.x, m_Position.y + m_Size.x - ( m_TileSize.x * 0.5f ) );
    break;
  }

  //CreateFrameBuffer();
}

void CoreGuiTileView::SetSelectedColor( const Color& color )
{
  m_SelectedColor = color;
}

void CoreGuiTileView::SetNormalColor( const Color& color )
{
  m_NormalColor = color;
}

void CoreGuiTileView::ScrollLeft( int /*btnId*/ )
{
  m_ScrollSpeed -= 0.01f;
  m_NeedsUpdate = true;
}

void CoreGuiTileView::ScrollRight( int /*btnId*/ )
{
  m_ScrollSpeed += 0.01f;
  m_NeedsUpdate = true;
}

void CoreGuiTileView::ScrollStop()
{
  m_ScrollSpeed = 0.0f;
}

void CoreGuiTileView::Clear()
{
  for ( size_t index = 0; index < m_Tiles.size(); index++ )
  {
    delete m_Tiles[index];
  }
  m_Tiles.clear();
}

bool CoreGuiTileView::RunDelete()
{
  if ( HasDeleteChild() )
  {
    for ( std::vector<CoreGuiBase*>::iterator itObj = m_Tiles.begin(); itObj != m_Tiles.end(); )
    {
      CoreGuiBase* guiObject = *itObj;
      if ( guiObject->HasDestroy() )
      {
        itObj = m_Tiles.erase( itObj );
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
  }
  return HasDestroy();
}

CoreGuiBase* CoreGuiTileView::AddTile( CoreGuiBase* tile )
{
  Vector2 oldSize = tile->GetSize();

  /// CoreOpenGLRender::GetRatio();
  if ( m_Framebuffer != NULL )
  {
    tile->SetSize( m_TileSize.x, m_TileSize.y );
  }
  else
  {
    tile->SetSize( m_TileSize.x * m_Size.x, m_TileSize.y * m_Size.y );
    tile->ScaleDebugFrame( m_TileSize.x * m_Size.x, m_TileSize.y * m_Size.y );
  }
  //tile->Scale( diffX, diffY );
  m_Tiles.push_back( tile );
  m_NeedsUpdate = true;

  //tile->SetLayer( GetLayer() + 1 );
  //tile->AddDebugFrame( m_NormalColor );

  if ( tile->HasDeleteChild() )
  {
    SetDeleteChild();
  }

  //AddChild( tile );
  return tile;
}

CoreGuiBase* CoreGuiTileView::GetTileWithName( const String& tileName )
{
  for ( size_t index = 0; index < m_Tiles.size(); index++ )
  {
    CoreGuiBase* tile = m_Tiles[ index ];
    if ( tile->GetName().EqualsWithCase( tileName ) )
    {
      return tile;
    }
  }
  return NULL;
}

bool CoreGuiTileView::RemoveTile( CoreGuiBase* tile )
{
  for ( std::vector< CoreGuiBase* >::iterator itTile = m_Tiles.begin(); itTile != m_Tiles.end(); ++itTile )
  {
    if ( *itTile == tile )
    {
      m_Tiles.erase( itTile );
      delete tile;
      m_NeedsUpdate = true;
      return true;
    }
  }
  return false;
}

void CoreGuiTileView::SetSelected( CoreGuiBase* tile )
{
  // Make sure I have this tile
  for ( size_t index = 0; index < m_Tiles.size(); index++ )
  {
    CoreGuiBase* guiObject    = m_Tiles[ index ];
    if ( guiObject == tile )
    {
      if ( m_SelectedTile != NULL )
      {
        m_SelectedTile->RemoveDebugFrame();
        m_SelectedTile->AddDebugFrame( m_NormalColor );
      }
      m_SelectedTile = guiObject;
      m_SelectedTile->RemoveDebugFrame();
      m_SelectedTile->AddDebugFrame( m_SelectedColor );
      return;
    }
  }
}


CoreGuiBase* CoreGuiTileView::OnMouseButtonDown( int buttonId, float xPos, float yPos )
{
  for ( size_t index = 0; index < m_Tiles.size(); index++ )
  {
    CoreGuiBase* guiObject    = m_Tiles[ index ];
    if ( !guiObject->HasFlag( FLAGS_ISINVISIBLE ) && guiObject->IsIntersecting( xPos,yPos ) )
    {
      SignalItemClicked( guiObject );
      guiObject->OnMouseButtonDown( buttonId, xPos, yPos );
      return guiObject;
    }
  }

  return CoreGuiBase::OnMouseButtonDown( buttonId, xPos, yPos );
}

bool CoreGuiTileView::OnMouseButtonWheel( float delta, const Vector2& mousePos )
{
  if ( IsIntersecting( mousePos.x, mousePos.y ) && IsVisible() )
  {
    if ( delta > 0 )
    {
      if ( (int)m_Tiles.size() - m_OffsetIndex > m_VisibleTiles )
      {
        m_OffsetIndex++;
      }
    }
    else
    {
      m_OffsetIndex--;
    }

    if ( m_OffsetIndex < 0 )
    {
      m_OffsetIndex = 0;
    }

    m_NeedsUpdate = true;

    //// Make sure that the offset is inside the view
    //switch( m_Layout )
    //{
    //  case LAYOUT_HORIZONTAL:
    //    break;
    //  case LAYOUT_VERTICAL:
    //    {
    //      float visibleHeight  = GetHeight();
    //      float fullHeight     = ( m_Tiles.size() * m_TileSize.y ) * 2.1f;
    //      float scrollDiff     = fullHeight - visibleHeight;

    //      if ( scrollDiff > 0.0f )
    //      {
    //        if ( scrollDiff < -m_Offset )
    //        {
    //          m_Offset = -scrollDiff;
    //        }
    //        else if ( m_Offset > 0.0f )
    //        {
    //          m_Offset = 0.0f;
    //        }
    //      }
    //      else
    //      {
    //        m_Offset = 0.0f;
    //      }
    //      //else if ( m_Offset > fullHeight )
    //      //{
    //      //  m_Offset = fullHeight;
    //      //}
    //    }
    //    break;
    //}
    return true;
  }
  return false;
}

void CoreGuiTileView::SetPosition( float x, float y )
{
  CoreGuiBase::SetPosition( x, y );
  if ( m_FramebufferView != NULL )
  {
    m_FramebufferView->SetPosition( x, y );
  }
  else
  {
    m_TilePosition = m_Position;
  }
  m_NeedsUpdate = true;
}

void CoreGuiTileView::SetSize( float x, float y )
{
  CoreGuiBase::SetSize( x, y );
  if ( m_FramebufferView != NULL )
  {
    m_FramebufferView->SetSize( x, y, m_Size.z );
  }
  else
  {
    for ( size_t index = 0; index < m_Tiles.size(); index++ )
    {
      CoreGuiBase* tile    = m_Tiles[ index ];
      tile->SetSize( m_TileSize.x * m_Size.x, m_TileSize.y * m_Size.y );
    }
  }
  m_NeedsUpdate = true;
}

float CoreGuiTileView::GetVisibleTop()
{
  if ( m_FramebufferView != NULL )
  {
    return -1.0f;
    //return m_FramebufferView->GetPosition().y - m_FramebufferView->GetSize().y;
  }
  return GetTop();
}

void CoreGuiTileView::Reposition()
{
  m_NeedsUpdate = false; 

  switch( m_Layout )
  {
    case LAYOUT_HORIZONTAL:
      {
        float leftX  = m_Arrow1->GetPosition().x + m_Arrow1->GetSize().x + ( m_TileSize.x * 0.5f );
        float rightX = m_Arrow2->GetPosition().x - m_Arrow2->GetSize().x - ( m_TileSize.x * 0.5f );
        float tileAreaWidth = GetWidth() - m_Arrow1->GetWidth() - m_Arrow2->GetWidth() - m_TileSize.x;

        float tileWidth = m_TileSize.x;
        if ( m_Tiles.size() > 0 )
        {
          CoreGuiBase* guiObject    = m_Tiles[ 0 ];
          tileWidth = guiObject->GetWidth();
        }

        if ( m_ScrollSpeed > 0 && m_Offset > 0 )
        {
          m_Offset = 0;
          ScrollStop();
        }
        if ( m_ScrollSpeed < 0 && m_Offset < 0 - ( ( m_Tiles.size() * tileWidth * 1.05f ) - tileAreaWidth  ) )
        {
          m_Offset = 0 - ( ( m_Tiles.size() * tileWidth * 1.05f ) - tileAreaWidth );
          ScrollStop();
        }

        float xx = m_Arrow1->GetPosition().x + m_TileSize.x + m_Offset;
        float yy = m_Position.y;
        for ( size_t index = 0; index < m_Tiles.size(); index++ )
        {
          CoreGuiBase* guiObject    = m_Tiles[ index ];

          if ( xx > leftX && xx < rightX )
          {
            guiObject->SetPosition( xx, yy );
            guiObject->ClearFlag( FLAGS_ISINVISIBLE );
          }
          else
          {
            guiObject->SetPosition( xx, yy );
            guiObject->SetFlag( FLAGS_ISINVISIBLE );
          }

          xx += guiObject->GetSize().x * 2.1f;
        }
      }
      break;

    case LAYOUT_VERTICAL:
      {
        float top     = GetTop();
        float bottom  = GetBottom();

        float xx      = m_TilePosition.x;
        float yy      = GetVisibleTop() + 0.01f + m_Offset;

        for ( size_t index = m_OffsetIndex; index < m_Tiles.size(); index++ )
        {
          CoreGuiBase* guiObject    = m_Tiles[ index ];
          yy += guiObject->GetSize().y * 1.05f;
          guiObject->SetPosition( xx, yy );

          // If framebuffer is not enabled, do the poor man's clipping
          if ( m_Framebuffer == NULL )
          {
            if ( top < yy + m_TileSize.y && yy - m_TileSize.y < bottom )
            {
              guiObject->ClearFlag( FLAGS_ISINVISIBLE );
            }
            else
            {
              guiObject->SetFlag( FLAGS_ISINVISIBLE );
            }
          }

          yy += guiObject->GetSize().y * 1.05f;
        }
      }
      break;
  }
}

bool CoreGuiTileView::OnDraw( const Matrix& projection )
{
  //CoreGuiBase::OnDraw( projection );

  //m_Arrow1->OnDraw( projection );
  //m_Arrow2->OnDraw( projection );

  if ( m_ScrollSpeed != 0.0f )
  {
    m_Offset += m_ScrollSpeed;
    m_NeedsUpdate = true;
  }

  if ( m_NeedsUpdate )
  {
    ReInit();
    Reposition();
  }

  //const Vector3& pos = GetPosition();
  if ( m_Framebuffer != NULL && m_FramebufferView != NULL )
  {
    glBindFramebuffer( GL_FRAMEBUFFER, m_Framebuffer->m_FrameBufferId );
    //glViewport(0,0, m_Framebuffer->m_Width, m_Framebuffer->m_Height );
    glViewport(0,0, CoreOpenGLRender::m_Width, CoreOpenGLRender::m_Height );

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Framebuffer->m_TextureId, 0);
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Framebuffer->m_DepthRenderBufferId );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_OPENGL
  }

  for ( size_t index = m_OffsetIndex; index < m_Tiles.size(); index++ )
  {
    CoreGuiBase* guiObject    = m_Tiles[ index ];
    if ( !guiObject->HasFlag( FLAGS_ISINVISIBLE ) )
    {
      guiObject->OnDraw( projection );
    }
  }

  if ( m_Framebuffer != NULL && m_FramebufferView != NULL )
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, CoreOpenGLRender::m_Width, CoreOpenGLRender::m_Height );
    m_FramebufferView->OnDraw( projection );
    CHECK_OPENGL
  }

  return true;
}

CoreGuiBase* CoreGuiTileView::GetWireGuiUnderMouse( float xPos, float yPos )
{
  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject    = m_GuiObjects[ index - 1 ];
    CoreGuiBase* focusObject  = guiObject->GetWireGuiUnderMouse( xPos, yPos );
    if ( focusObject != NULL )
    {
      return focusObject;
    }
  }

  for ( size_t index = m_Tiles.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject    = m_Tiles[ index - 1 ];
    CoreGuiBase* focusObject  = guiObject->GetWireGuiUnderMouse( xPos, yPos );
    if ( focusObject != NULL )
    {
      return focusObject;
    }
  }

  if ( IsIntersecting( xPos, yPos ) )
  {
    return this;
  }
  return NULL;
}

CoreGuiBase* CoreGuiTileView::GetSolidGuiUnderMouse( float xPos, float yPos )
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

  for ( size_t index = m_Tiles.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject    = m_Tiles[ index - 1 ];
    CoreGuiBase* focusObject  = guiObject->GetSolidGuiUnderMouse( xPos, yPos );
    if ( focusObject != NULL )
    {
      return focusObject;
    }
  }

  if ( IsIntersecting( xPos, yPos ) )
  {
    return this;
  }
  return NULL;
}
