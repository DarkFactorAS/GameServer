/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : RenderCamera
*
* Description : The render camera is like the eye of the user. It determines what the player 
*               actually sees. The camera has a position and rotation.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "RenderCamera.h"

#define TWO_PI 6.28318530717958647692f
#define MINROT 0.00001f;

RenderCamera* RenderCamera::s_Instance = NULL;

RenderCamera::RenderCamera() :
  m_Dx( 0.0f ),
  m_Dy( 0.0f ),
  m_Zoom( 1.0f ),
  m_IsDirty( true ),
  m_CanBeControlled( true )
{
}

RenderCamera* RenderCamera::GetInstance()
{
  if ( s_Instance == NULL )
  {
    s_Instance = new RenderCamera();
  }
  return s_Instance;
}

void RenderCamera::Destroy()
{
  delete s_Instance;
  s_Instance = NULL;
}

/************************************************************************************************
* Init:
* Initializes the camera looking at a specific position, with a specific rotation and zoom. This
* will automaticly place the camera in the correct position and is used when the screen is
* initialized.
*
* @param  (const Vector3&)  targetPosition  - The world position where the camera should look
* @param  (float)           yaw             - How much yaw ( x rotation ) the camera has
* @param  (float)           pitch           - How much pitch ( y rotation ) the camera has
* @param  (float)           zoom            - How far away from the target point the camera is
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderCamera::Init( const Vector3& targetPosition, float yaw, float pitch, float zoom )
{
  m_Dx    = yaw;
  m_Dy    = pitch;
  m_Zoom  = zoom;

  if ( m_Dy == 0 )
  {
    m_Dy = MINROT;
  }

  Quaternion rot = QuatRotationAxis( Vector3::s_Vec3Right, m_Dy ) * QuatRotationAxis( Vector3::s_Vec3Forward, m_Dx );
  Vector3 Z = Vector3::s_Vec3Forward * rot;  Z.Normalize();

  m_Target    = targetPosition;
  m_Position  = targetPosition + ( Z * zoom );
  m_IsDirty = true;
}

/************************************************************************************************
* Center:
* Centers the camera on position 0,0
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderCamera::Center()
{
  m_Position.Set( 0, 0, 0.5f );
  m_Target.Set( Vector3::s_Vec3Zero );
  m_Dx  = 0.0f;
  m_Dy  = 0.0f;
  m_IsDirty = true;
}

/************************************************************************************************
* Zoom:
* Zooms the camera close or further away from the target position it is looking at
*
* param   (float)         deltaZoom   - How much to zoom
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderCamera::Zoom( float deltaZoom )
{
  if ( !m_CurrentPosition.IsNull() )
  {
    return;
  }

  m_IsDirty = true;

  //LogInfoFMT("RenderCamera","Zoom [%0.4f] [%0.4f]", deltaZoom, m_Zoom );

  if ( deltaZoom > 0.0f && m_Zoom < 5.0f )
  {
    m_Zoom += deltaZoom;
  }
  if ( deltaZoom < 0.0f && m_Zoom > 0.5f )
  {
    m_Zoom += deltaZoom;
  }

  Quaternion rot = QuatRotationAxis( Vector3::s_Vec3Right, m_Dy ) * QuatRotationAxis( Vector3::s_Vec3Forward, m_Dx );
  Vector3 Z = Vector3::s_Vec3Forward * rot;  Z.Normalize();
  m_Position  = m_Target + ( Z * m_Zoom );
}

void RenderCamera::MoveToPosition( const Vector3& lookatPosition )
{
  StopSlide();

  Quaternion rot = QuatRotationAxis( Vector3::s_Vec3Right, m_Dy ) * QuatRotationAxis( Vector3::s_Vec3Forward, m_Dx );
  Vector3 Z = Vector3::s_Vec3Forward * rot;  Z.Normalize();

  m_Target    = lookatPosition;
  m_Position  = lookatPosition + ( Z * m_Zoom );
  m_IsDirty   = true;
}

void RenderCamera::MoveToPosition( const Vector3& position, const Vector3& lookatPosition )
{
  StopSlide();

  m_Position = position;
  m_Target   = lookatPosition;
  m_IsDirty   = true;
}

void RenderCamera::SlideToPosition( const Vector3& lookatPosition )
{
  StopSlide();

  if ( Vec3Length( m_Target - lookatPosition ) > 0.1f )
  {
    Quaternion rot = QuatRotationAxis( Vector3::s_Vec3Right, m_Dy ) * QuatRotationAxis( Vector3::s_Vec3Forward, m_Dx );
    Vector3 Z = Vector3::s_Vec3Forward * rot;  
    Z.Normalize();

    m_CurrentPosition = m_Position;
    m_CurrentTarget   = m_Target;
    m_Step            = 0;
    m_Target          = lookatPosition;
    m_Position        = lookatPosition + ( Z * m_Zoom );
    m_IsDirty         = true;
  }
  else
  {
    MoveToPosition( lookatPosition );
  }
}

void RenderCamera::SlideToPosition( const Vector3& position, const Vector3& lookatPosition )
{
  StopSlide();

  if ( Vec3Length( m_Target - lookatPosition ) > 0.1f )
  {
    m_CurrentPosition = m_Position;
    m_CurrentTarget   = m_Target;
    m_Step            = 0;
    m_Position        = position;
    m_Target          = lookatPosition;
    m_IsDirty         = true;
  }
  else
  {
    MoveToPosition( position, lookatPosition );
  }
}

void RenderCamera::StopSlide()
{
  if ( !m_CurrentPosition.IsNull() )
  {
    float spring      = sin( m_Step * TWO_PI * 0.25f );
    m_Position        = Vec3Lerp( m_CurrentPosition, m_Position, spring );
    m_Target          = Vec3Lerp( m_CurrentTarget, m_Target, spring );

    m_Step            = 1.0f; 
    m_IsDirty         = false;
    m_CurrentTarget   = Vector3::s_Vec3Zero;
    m_CurrentPosition = Vector3::s_Vec3Zero;
  }
}

/************************************************************************************************
* Move:
* Move the target position ( where the camera is looking ) and the camera itself with x,y. This
* is used when the user moves the mouse or touches the screen to move the camera.
*
* param   (float)         x   - How much to move in x direction ( sideways )
* param   (float)         y   - How much to move in y direction ( up/down )
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderCamera::Move( float x, float y )
{
  if ( !m_CurrentPosition.IsNull() )
  {
    return;
  }

  m_Target.x += x;
  m_Target.y += y;
  m_Position.x += x;
  m_Position.y += y;
  m_IsDirty  = true;
}

/************************************************************************************************
* UpdateYaw:
* Rotates the camera around the X axis ( makes it circle around the object ).
*
* param   (float)     deltaYaw - How much to add to the rotation
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderCamera::UpdateYaw( float deltaYaw )
{
  if ( !m_CurrentPosition.IsNull() )
  {
    return;
  }

  m_Dx      -= ( deltaYaw / 1 );

  while ( m_Dx < -TWO_PI )
  {
    m_Dx += TWO_PI;
  }
  while ( m_Dx > TWO_PI )
  {
    m_Dx -= TWO_PI;
  }

  m_IsDirty  = true;
}

/************************************************************************************************
* UpdatePitch:
* Rotates the camera around the Y axis ( makes it move up and down ).
*
* param   (float)     deltaPitch - How much to add to the rotation
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderCamera::UpdatePitch( float deltaPitch )
{
  if ( !m_CurrentPosition.IsNull() )
  {
    return;
  }

  m_Dy      -= ( deltaPitch / 1 );

  if ( m_Dy < 0.001f )
  {
    m_Dy = 0.01f;
  }

  while ( m_Dy < -TWO_PI )
  {
    m_Dy += TWO_PI;
  }
  while ( m_Dy > TWO_PI )
  {
    m_Dy -= TWO_PI;
  }

  m_IsDirty  = true;
}

/************************************************************************************************
* FrameProcess:
* Main run loop for the camera ( to update position of the camera relative to its rotation 
* from the target lookat point ).
*
* @param  (float)   deltaTime    - Amount of time that passed since last call.
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool RenderCamera::FrameProcess( float deltaTime )
{
  if ( m_IsDirty )
  {
    if ( !m_CurrentTarget.IsNull() )
    {
      m_Step += deltaTime; 
      if ( m_Step > 1.0f )
      {
        m_Step            = 1.0f; 
        m_IsDirty         = false;
        m_CurrentTarget   = Vector3::s_Vec3Zero;
        m_CurrentPosition = Vector3::s_Vec3Zero;

        //Vector3 Z       = Vector3::s_Vec3Forward * m_Rotation;
        //Z.Normalize();
        //m_Position      = m_Target + ( Z * Vec3Length( m_Position - m_Target ) );
        Matrix::LookAt( m_ViewMatrix, m_Position, m_Target, Vector3::s_Vec3Forward );
      }
      else
      {
        float spring = sin( m_Step * TWO_PI * 0.25f );
        Vector3 deltaPosition = Vec3Lerp( m_CurrentPosition, m_Position, spring );
        Vector3 deltaTarget   = Vec3Lerp( m_CurrentTarget, m_Target, spring );
        //Vector3 Z             = Vector3::s_Vec3Forward * m_Rotation;
        //Z.Normalize();
        //deltaPosition         = deltaTarget + ( Z * Vec3Length( deltaPosition - deltaTarget ) );

        Matrix::LookAt( m_ViewMatrix, deltaPosition, deltaTarget, Vector3::s_Vec3Forward );
      }
    }
    else
    {
      m_IsDirty = false;
      m_Rotation = QuatRotationAxis( Vector3::s_Vec3Right, m_Dy ) * QuatRotationAxis( Vector3::s_Vec3Forward, m_Dx );
      Vector3 Z = Vector3::s_Vec3Forward * m_Rotation;
      Z.Normalize();
      m_Position = m_Target + ( Z * Vec3Length( m_Position - m_Target ) );
      Matrix::LookAt( m_ViewMatrix, m_Position, m_Target, Vector3::s_Vec3Forward );
    }
  }

  return true;
}

const Quaternion& RenderCamera::GetRotiation()
{
  return m_Rotation;
}