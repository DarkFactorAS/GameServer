
#include "Precompile.h"
#include "CoreBaseObject.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

#if defined DEBUG && defined PLATFORM_WINDOWS
  #include "EvilEngine/CoreLib/Assert/DFCallstack.h"
  #include "Windows.h"

  std::vector< CoreBaseObject * > CoreBaseObject::s_ObjectList;
  bool                            CoreBaseObject::s_TrackCallstack = false;
  bool                            CoreBaseObject::s_CheckLeaks     = false;
#endif

#if defined DEBUG
  uint32 CoreBaseObject::s_ThreadId = 0;
#endif

CoreBaseObject::CoreBaseObject() :
  m_ResourceId( 0 ),
  m_Color( 1.0f, 1.0f, 1.0f, 1.0f ),
  m_Flags( 0 ),
  m_Destroy( false ),
  m_DeleteChild( false ),
  m_UpdateLocalMatrix( 0 ),
  m_Parent( NULL )
{
#if defined DEBUG 
  dfBugreportFMT( CoreUtils::GetThreadId() == s_ThreadId, "CoreBaseObject has to be added in mainthread[%d] not in childthread[%d]", s_ThreadId, CoreUtils::GetThreadId() );
#endif
#if defined DEBUG && defined PLATFORM_WINDOWS
  if ( s_CheckLeaks )
  {
    s_ObjectList.push_back( this );
    if ( s_TrackCallstack )
    {
      DFCallstack::GetInstance()->GetCallStack( m_CallStack );
    }
  }
#endif

  SetInstance();
}

CoreBaseObject::CoreBaseObject( uint32_t resourceId ) :
  m_ResourceId( resourceId ),
  m_Color( 1.0f, 1.0f, 1.0f, 1.0f ),
  m_Flags( 0 ),
  m_Destroy( false ),
  m_DeleteChild( false ),
  m_UpdateLocalMatrix( 0 ),
  m_Parent( NULL )
{
#if defined DEBUG 
  dfBugreportFMT( CoreUtils::GetThreadId() == s_ThreadId, "CoreBaseObject has to be added in mainthread[%d] not in childthread[%d]", s_ThreadId, CoreUtils::GetThreadId() );
#endif

#if defined DEBUG && defined PLATFORM_WINDOWS
  if ( s_CheckLeaks )
  {
    s_ObjectList.push_back( this );
    if ( s_TrackCallstack )
    {
      DFCallstack::GetInstance()->GetCallStack( m_CallStack );
    }
  }
#endif
  SetInstance();
}

CoreBaseObject::CoreBaseObject( const Vector3& position, const Vector3& size, uint32_t resourceId ) :
  m_Color( 1.0f, 1.0f, 1.0f, 1.0f ),
  m_Flags( 0 ),
  m_Destroy( false ),
  m_DeleteChild( false ),
  m_UpdateLocalMatrix( 0 ),
  m_Parent( NULL )
{
  m_Position    = position;
  m_Size        = size;
  m_ResourceId  = resourceId;

  UpdateMatrix( UPDATEFLAG_POSITION | UPDATEFLAG_SIZE );

#if defined DEBUG 
  dfBugreportFMT( CoreUtils::GetThreadId() == s_ThreadId, "CoreBaseObject has to be added in mainthread[%d] not in childthread[%d]", s_ThreadId, CoreUtils::GetThreadId() );
#endif

#if defined DEBUG && defined PLATFORM_WINDOWS
  if ( s_CheckLeaks )
  {
    s_ObjectList.push_back( this );
    if ( s_TrackCallstack )
    {
      DFCallstack::GetInstance()->GetCallStack( m_CallStack );
    }
  }
#endif
  SetInstance();
}

CoreBaseObject::~CoreBaseObject()
{
  m_SignalOnDeath( this );

#if defined DEBUG 
  dfBugreportFMT( CoreUtils::GetThreadId() == s_ThreadId, "CoreBaseObject has to be added in mainthread[%d] not in childthread[%d]", s_ThreadId, CoreUtils::GetThreadId() );
#endif

#if defined DEBUG && defined PLATFORM_WINDOWS
  if ( s_CheckLeaks )
  {
    for ( std::vector< CoreBaseObject * >::iterator itObjList = s_ObjectList.begin(); itObjList != s_ObjectList.end(); ++itObjList )
    {
      if ( *itObjList == this )
      {
        s_ObjectList.erase( itObjList );
        return;
      }
    }
    dfBugreportFMT( false, "CoreBaseObject not found in list!" );
  }
#endif
}

void CoreBaseObject::SetVisible( bool isVisible)
{
  if ( isVisible )
  {
    ClearFlag( FLAGS_ISINVISIBLE );
  }
  else
  {
    SetFlag( FLAGS_ISINVISIBLE );
  }
}


//void CoreBaseObject::Scale( float scale )
//{
//  m_Size.x *= scale;
//  m_Size.y *= scale;
//  m_Size.z *= scale;
//}
void CoreBaseObject::Move( float deltaX, float deltaY, bool /*isLocal*/ )
{
  SetPosition( m_Position.x + deltaX, m_Position.y + deltaY ); 
}

void CoreBaseObject::SetInstance()
{
  static uint32 s_Instance = 0;
  m_Instance = ++s_Instance;
}

void CoreBaseObject::FadeIn()
{
}

void CoreBaseObject::FadeOutAndDestroy()
{
  // TODO: Change this to actually fade out
  Destroy();
}

void CoreBaseObject::Destroy()
{
  m_Destroy = true;
  if ( m_Parent != NULL )
  {
    SetDeleteChild();
  }
}

void CoreBaseObject::SetDeleteChild()
{
  m_DeleteChild = true;
  if ( m_Parent != NULL )
  {
    m_Parent->SetDeleteChild();
  }
}

#if defined DEBUG && defined PLATFORM_WINDOWS
void CoreBaseObject::PrintLeakedList()
{
  if ( !s_CheckLeaks )
  {
    return;
  }

  int pCallstack = 10;
  for ( std::vector< CoreBaseObject * >::iterator itObjList = s_ObjectList.begin(); itObjList != s_ObjectList.end(); ++itObjList )
  {
    CoreBaseObject* obj     = *itObjList;
    CoreBaseObject* parent  = obj->m_Parent;
    if ( parent != nullptr )
    {
      LogWarningFMT("CoreBaseObject", "Leaked object %d/%s [Parent:%d/%s]", obj->m_Instance, obj->GetStaticClassName().c_str(), parent->m_Instance, parent->GetStaticClassName().c_str() );
    }
    else
    {
      LogWarningFMT("CoreBaseObject", "Leaked object %d/%s", obj->m_Instance, obj->GetStaticClassName().c_str() );
    }
    if ( pCallstack > 0 && !obj->m_CallStack.empty() )
    {
      LogWarningFMT("CoreBaseObject", "-- Callstack \n%s", obj->m_CallStack.c_str() );
      pCallstack--;
    }
  }
  if (!s_ObjectList.empty())
  {
    LogFatalFMT("CoreBaseObject", "CoreBaseObject is leaking %d objects!", s_ObjectList.size());
  }
}
#endif

void CoreBaseObject::SetRotation( float x, float y, float z )
{
  static float fRad = (float)M_PI / 180.0f;

  if ( x != m_Rotation.x )
  {
    m_RadRotation.x = x * fRad;
  }
  if ( y != m_Rotation.y )
  {
    m_RadRotation.y = y * fRad;
  }
  if ( z != m_Rotation.z )
  {
    m_RadRotation.z = z * fRad;
  }

  m_Rotation.Set( x, y, z ); 
  UpdateMatrix( UPDATEFLAG_ROTATION ); 
}


void CoreBaseObject::UpdateMatrix( uint32 /*updateFlag*/ )
{
  m_UpdateLocalMatrix++;

  //if ( ( updateFlag & UPDATEFLAG_POSITION ) == UPDATEFLAG_POSITION )
  {
    Matrix::CreateTranslation( m_Position, &m_LocalMatrix );
    //m_LocalMatrix.SetTranslation( m_Position.x, m_Position.y, m_Position.z );
  }

  //if ( ( updateFlag & UPDATEFLAG_ROTATION ) == UPDATEFLAG_ROTATION )
  {
    //static float fRad = (float)M_PI / 180.0f;
    //float radX = m_Rotation.x * fRad;
    //float radY = m_Rotation.y * fRad;
    //float radZ = m_Rotation.z * fRad;

    //m_LocalMatrix.Rotate( Vector3::s_Vec3One, m_RadRotation );

    if ( m_RadRotation.x != 0.0f )
    {
      m_LocalMatrix.Rotate( Vector3::s_Vec3Forward, m_RadRotation.x );
    }
    if ( m_RadRotation.y != 0.0f )
    {
      m_LocalMatrix.Rotate( Vector3::s_Vec3Up, m_RadRotation.y );
    }
    if ( m_RadRotation.z != 0.0f )
    {
      m_LocalMatrix.Rotate( Vector3::s_Vec3Right, m_RadRotation.z );
    }
  }

  //if ( ( updateFlag & UPDATEFLAG_SIZE ) == UPDATEFLAG_SIZE )
  {
    m_LocalMatrix.Scale( m_Size );
  }
}
