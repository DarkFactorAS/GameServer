#ifndef EVILENGINE_CORERENDERING_CAMERA_RENDERCAMERA
#define EVILENGINE_CORERENDERING_CAMERA_RENDERCAMERA 1

#include "../../CoreLib/CoreMath/Vector3.h"
#include "../../CoreLib/CoreMath/Matrix.h"
#include "../../CoreLib/CoreMath/Quaternion.h"

class RenderCamera
{
public:

  static RenderCamera*  GetInstance();
  static void           Destroy();
  static RenderCamera*  s_Instance;

  virtual bool          FrameProcess( float deltaTime );

  const Vector3&        GetPosition(){ return m_Position; }
  const Vector3&        GetTargetPosition(){ return m_Target; }
  Vector3               GetLookatVector(){ return m_Target - m_Position; }
  const Quaternion&     GetRotiation();

  void                  Zoom( float deltaZoom );
  void                  Move( float x, float y );
  void                  Center();
  void                  Init( const Vector3& targetPosition, float yaw, float pitch, float zoom );
  void                  UpdateYaw( float deltaYaw );
  void                  UpdatePitch( float deltaPitch );

  const Matrix&         GetViewMatrix() const{ return m_ViewMatrix; }
  bool                  IsDirty() const{ return m_IsDirty; }

  float                 GetYaw(){ return m_Dx; }
  float                 GetPitch(){ return m_Dy; }

  void                  MoveToPosition( const Vector3& position, const Vector3& lookatPosition );
  void                  MoveToPosition( const Vector3& lookatPosition );
  void                  SlideToPosition( const Vector3& position, const Vector3& lookatPosition );
  void                  SlideToPosition( const Vector3& lookatPosition );
  void                  StopSlide();

  bool                  CanBeControlled() const{ return m_CanBeControlled; }
  void                  SetControllable( bool isControllable ){ m_CanBeControlled = isControllable; }

  float                 GetZoom() const{ return m_Zoom; }

private:

  RenderCamera();

  Matrix                m_ViewMatrix;
  bool                  m_IsDirty;  

  float                 m_Zoom;
  float                 m_Dx;
  float                 m_Dy;
  Vector3               m_Position;
  Vector3               m_CurrentPosition;
  Vector3               m_Target;
  Vector3               m_CurrentTarget;
  Quaternion            m_Rotation;
  bool                  m_CanBeControlled;

  float                 m_Step;
};

#endif /// EVILENGINE_CORERENDERING_CAMERA_RENDERCAMERA