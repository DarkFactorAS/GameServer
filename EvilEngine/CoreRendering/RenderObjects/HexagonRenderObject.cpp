
#include "Precompile.h"
#include "HexagonRenderObject.h"
#include "MultiFaceRenderObject.h"
#include "RenderListObject.h"

HexagonRenderObject::HexagonRenderObject( uint32 resourceId, uint32 wallResourceId ) :
  RenderListObject()
{
  InitRender( Vector3::s_Vec3Zero, Vector3::s_Vec3Zero, resourceId, wallResourceId );
  RenderListObject::SetResourceId( resourceId );
}

HexagonRenderObject::HexagonRenderObject( const Vector3& position, const Vector3& size, uint32 resourceId, uint32 wallResourceId ) :
  RenderListObject()
{
  InitRender( position, size, resourceId, wallResourceId );
  RenderListObject::SetResourceId( resourceId );
}

void HexagonRenderObject::Init()
{
}

void HexagonRenderObject::InitRender( const Vector3& position, const Vector3& size, uint32 resourceId, uint32 wallResourceId )
{
  RenderObject::Init();

  MultiFaceRenderObject* groundRenderObject = new MultiFaceRenderObject( 30, resourceId );
  AddRenderObject( GroundRender, groundRenderObject );

  //      b___c
  //    a/    \d
  //     \____ /
  //      f   e

  //      3___5
  //    1/    \6
  //     \____ /
  //      2   4

  float posUp   = 0.5f;
  float posDown = 0.0f;

  float radius  = 1.0f;
  float angle   = 0.0f;

  float* pointX = new float[6];
  float* pointY = new float[6];
  for ( int n = 0; n < 6; n++)
  {
    angle = ( n / 6.0f * 2 * (float)M_PI );
    pointX[n] = radius * cos(angle);
    pointY[n] = radius * sin(angle);
  }

  float ax = pointX[0];   float ay = pointY[0];
  float bx = pointX[1];   float by = pointY[1];
  float cx = pointX[2];   float cy = pointY[2];
  float dx = pointX[3];   float dy = pointY[3];
  float ex = pointX[4];   float ey = pointY[4];
  float fx = pointX[5];   float fy = pointY[5];

  Vector3 aUp( ax, ay, posUp );    Vector3 aDown( ax, ay, posDown );
  Vector3 bUp( bx, by, posUp );    Vector3 bDown( bx, by, posDown );
  Vector3 cUp( cx, cy, posUp );    Vector3 cDown( cx, cy, posDown );
  Vector3 dUp( dx, dy, posUp );    Vector3 dDown( dx, dy, posDown );
  Vector3 eUp( ex, ey, posUp );    Vector3 eDown( ex, ey, posDown );
  Vector3 fUp( fx, fy, posUp );    Vector3 fDown( fx, fy, posDown );

  groundRenderObject->AddPoint(  0  ,  0   ,  0.5f );  // 0
  groundRenderObject->AddPoint(  ax , ay   ,  0.5f );  // a
  groundRenderObject->AddPoint(  bx , by   ,  0.5f, Vector3::s_Vec3Up, 0.5f, 0.6f );  // b
  groundRenderObject->AddPoint(  cx , cy   ,  0.5f, Vector3::s_Vec3Up, 0.5f, 0.6f );           // c     
  groundRenderObject->AddPoint(  0  ,  0   ,  0.5f );  // 0
  groundRenderObject->AddPoint(  dx , dy   ,  0.5f );       // d
  groundRenderObject->AddPoint(  ex , ey   ,  0.5f, Vector3::s_Vec3Up, 0.5f, 0.6f );      // e
  groundRenderObject->AddPoint(  fx , fy   ,  0.5f, Vector3::s_Vec3Up, 0.5f, 0.6f );         // f
  groundRenderObject->AddPoint(  0  ,  0   ,  0.5f );  // 0
  groundRenderObject->AddPoint(  ax , ay   ,  0.5f );  // a

  // Side #1
  MultiFaceRenderObject* wallRenderObject   = NULL;
  Vector3 normalA = Vector3::Normal( aUp, aDown, bUp );
  wallRenderObject   = new MultiFaceRenderObject( 4, wallResourceId );
  wallRenderObject->SetColor( Color::Grey );
  wallRenderObject->AddPoint( aUp,   normalA ); 
  wallRenderObject->AddPoint( aDown, normalA ); 
  wallRenderObject->AddPoint( bUp,   normalA ); 
  wallRenderObject->AddPoint( bDown, normalA ); 
  AddRenderObject( WallRender, wallRenderObject );

  // Side #2
  Vector3 normalB = Vector3::Normal( bUp, bDown, cUp );
  wallRenderObject = new MultiFaceRenderObject( 4, wallResourceId );
  wallRenderObject->SetColor( Color::Grey );
  wallRenderObject->AddPoint(  bUp,   normalB ); 
  wallRenderObject->AddPoint(  bDown, normalB ); 
  wallRenderObject->AddPoint(  cUp,   normalB ); 
  wallRenderObject->AddPoint(  cDown, normalB ); 
  AddRenderObject( WallRender, wallRenderObject );

  // Side #3
  Vector3 normalC = Vector3::Normal( cUp, cDown, dUp );
  wallRenderObject = new MultiFaceRenderObject( 4, wallResourceId );
  wallRenderObject->SetColor( Color::Grey );
  wallRenderObject->AddPoint(  cUp,   normalC ); 
  wallRenderObject->AddPoint(  cDown, normalC ); 
  wallRenderObject->AddPoint(  dUp,   normalC ); 
  wallRenderObject->AddPoint(  dDown, normalC ); 
  AddRenderObject( WallRender, wallRenderObject );

  // Side #4
  Vector3 normalD = Vector3::Normal( dUp, dDown, eUp );
  wallRenderObject = new MultiFaceRenderObject( 4, wallResourceId );
  wallRenderObject->SetColor( Color::Grey );
  wallRenderObject->AddPoint(  dUp,   normalD ); 
  wallRenderObject->AddPoint(  dDown, normalD ); 
  wallRenderObject->AddPoint(  eUp,   normalD ); 
  wallRenderObject->AddPoint(  eDown, normalD ); 
  AddRenderObject( WallRender, wallRenderObject );


  // Side #5
  Vector3 normalE = Vector3::Normal( eUp, eDown, fUp );
  wallRenderObject = new MultiFaceRenderObject( 4, wallResourceId );
  wallRenderObject->SetColor( Color::Grey );
  wallRenderObject->AddPoint(  eUp,   normalE ); 
  wallRenderObject->AddPoint(  eDown, normalE ); 
  wallRenderObject->AddPoint(  fUp,   normalE ); 
  wallRenderObject->AddPoint(  fDown, normalE ); 
  AddRenderObject( WallRender, wallRenderObject );

  // Side #6
  Vector3 normalF = Vector3::Normal( fUp, fDown, aUp );
  wallRenderObject = new MultiFaceRenderObject( 4, wallResourceId );
  wallRenderObject->SetColor( Color::Grey );
  wallRenderObject->AddPoint(  fUp,   normalF ); 
  wallRenderObject->AddPoint(  fDown, normalF ); 
  wallRenderObject->AddPoint(  aUp,   normalF ); 
  wallRenderObject->AddPoint(  aDown, normalF ); 
  AddRenderObject( WallRender, wallRenderObject );

  // Update these two
  SetPosition( position.x, position.y, position.z );
  SetSize( size.x, size.y, size.z );
}

void HexagonRenderObject::SetWallColor( const Color& color )
{
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    if ( renderObject->GetInstance() == WallRender )
    {
      renderObject->SetColor( color );
    }
  }
}

void HexagonRenderObject::SetResourceId( uint32 resourceId )
{
  RenderListObject::SetResourceId( GroundRender, resourceId );
}
