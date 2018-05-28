#include "PreCompile.h"
#include "CoreGuiFrame.h"
#include "../../CoreRendering/RenderObjects/QuadRenderObject.h"

float CoreGuiFrame::BORDER = 0.003f;

CoreGuiFrame::CoreGuiFrame( const Vector2& position, const Vector2& size, const Color& color ) :
  CoreGuiBase( "CoreGuiFrame", position, size )
{
  Vector2 sizeH( BORDER * 0.5f, size.y );
  Vector2 sizeV( size.x, BORDER * 1.0f );

  m_Left    = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeH, GuiResource_White ) );
  m_Top     = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeV, GuiResource_White ) );
  m_Right   = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeH, GuiResource_White ) );
  m_Bottom  = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeV, GuiResource_White ) );

  m_Left->SetColor( color );
  m_Top->SetColor( color );
  m_Right->SetColor( color );
  m_Bottom->SetColor( color );

  

  InitPosition();
}

CoreGuiFrame::CoreGuiFrame( const String& objectName, const Vector2& position, const Vector2& size, const Color& color ) :
  CoreGuiBase( objectName, position, size )
{
  Vector2 sizeH( BORDER * 0.5f, size.y );
  Vector2 sizeV( size.x, BORDER * 1.0f );

  m_Left    = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeH, GuiResource_White ) );
  m_Top     = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeV, GuiResource_White ) );
  m_Right   = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeH, GuiResource_White ) );
  m_Bottom  = (QuadRenderObject*) AddRenderObject( new QuadRenderObject( Vector2::s_Vec2One, sizeV, GuiResource_White ) );

  m_Left->SetColor( color );
  m_Top->SetColor( color );
  m_Right->SetColor( color );
  m_Bottom->SetColor( color );

  InitPosition();
}

void CoreGuiFrame::SetPosition( float x, float y )
{
  CoreGuiBase::SetPosition( x, y );
  InitPosition();
}

void CoreGuiFrame::SetSize( float x, float y )
{
  CoreGuiBase::SetSize( x, y );

  Vector2 sizeH( BORDER * 0.5f, y );
  Vector2 sizeV( x, BORDER * 1.0f );

  m_Left->SetSize( sizeH );
  m_Top->SetSize( sizeV );
  m_Right->SetSize( sizeH );
  m_Bottom->SetSize( sizeV );

  InitPosition();
}

void CoreGuiFrame::InitPosition()
{
  Vector2 sizeH( BORDER * 0.5f, m_Size.y );
  Vector2 sizeV( m_Size.x, BORDER * 1.0f );

  float z = m_Position.z;

  m_Left->SetPosition(    m_Position.x - m_Size.x + sizeH.x,  m_Position.y, z );
  m_Top->SetPosition(     m_Position.x,                       m_Position.y + m_Size.y - sizeV.y, z );
  m_Right->SetPosition(   m_Position.x + m_Size.x - sizeH.x,  m_Position.y, z );
  m_Bottom->SetPosition(  m_Position.x,                       m_Position.y - m_Size.y + sizeV.y, z );
}

void CoreGuiFrame::SetColor( float red, float green, float blue )
{
  CoreGuiBase::SetColor( red, green, blue );
  m_Left->SetColor( red, green, blue );
  m_Top->SetColor( red, green, blue );
  m_Right->SetColor( red, green, blue );
  m_Bottom->SetColor( red, green, blue );
}

void CoreGuiFrame::SetAlpha( float alpha )
{
  CoreGuiBase::SetAlpha( alpha );
  m_Left->SetAlpha( alpha );
  m_Top->SetAlpha( alpha );
  m_Right->SetAlpha( alpha);
  m_Bottom->SetAlpha( alpha);
}

void CoreGuiFrame::AddFrame( CoreGuiBase* parent )
{
  if ( parent != NULL )
  {
    parent->AddChild( new CoreGuiFrame( parent->GetPosition(), parent->GetSize(), Color::White ) );
  }
}

CoreGuiBase* CoreGuiFrame::GetWireGuiUnderMouse( float xPos, float yPos )
{
  CoreGuiBase* guiBase = CoreGuiBase::GetWireGuiUnderMouse( xPos, yPos );
  if ( guiBase == this )
  {
    guiBase = NULL; 
  }
  return guiBase;
}

CoreGuiBase* CoreGuiFrame::GetSolidGuiUnderMouse( float xPos, float yPos )
{
  CoreGuiBase* guiBase = CoreGuiBase::GetSolidGuiUnderMouse( xPos, yPos );
  if ( guiBase == this )
  {
    guiBase = NULL; 
  }
  return guiBase;
}
