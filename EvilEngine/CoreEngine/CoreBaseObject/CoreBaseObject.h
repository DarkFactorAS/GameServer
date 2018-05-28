#ifndef EVILENGINE_COREENGINE_COREBASEOBJECT
#define EVILENGINE_COREENGINE_COREBASEOBJECT 1
#pragma once

#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include <vector>

class CoreBaseObject
{
public:

  enum FLAGS
  {
    FLAGS_ISINVISIBLE       = 1,
    FLAGS_ISMODAL           = 2,
    FLAGS_NOTSCALECHILD     = 4,
    FLAGS_SQUARED           = 8,
  };

  enum UPDATE_FLAG
  {
    UPDATEFLAG_POSITION     = 1,
    UPDATEFLAG_ROTATION     = 2,
    UPDATEFLAG_SIZE         = 4,
  };

  CoreBaseObject();
  CoreBaseObject( uint32_t resourceId );
  CoreBaseObject( const Vector3& position, const Vector3& size, uint32_t resourceId );
  virtual ~CoreBaseObject();

  uint32                                    GetInstance() const{ return m_Instance; }
  void                                      SetInstance( uint32 instanceId ){ m_Instance = instanceId; }

  const Vector3&                            GetPosition() const{ return m_Position; }
  const Vector3&                            GetRotation() const{ return m_Rotation; }
  const Vector3&                            GetSize() const{ return m_Size; }

  virtual void                              SetResourceId( uint32 resourceId ){ m_ResourceId = resourceId; }
  virtual uint32                            GetResourceId() const { return m_ResourceId; }

  virtual void                              SetPosition( float x, float y, float z ){ m_Position.Set( x, y, z ); UpdateMatrix( UPDATEFLAG_POSITION ); }
  virtual void                              SetPosition( const Vector2& position ){ SetPosition( position.x, position.y, m_Position.z ); }
  virtual void                              SetPosition( const Vector3& position ){ SetPosition( position.x, position.y, position.z ); }
  virtual void                              SetPosition( float x, float y ){ SetPosition( x, y, m_Position.z ); }
  virtual void                              Move( float deltaX, float deltaY, bool isLocal );

  virtual void                              SetRotation( float x, float y, float z ); //{ m_Rotation.Set( x, y, z ); UpdateMatrix( UPDATEFLAG_ROTATION ); }
  virtual void                              SetRotation( const Vector3& rot ){ SetRotation( rot.x, rot.y, rot.z ); }

  virtual void                              SetSize( float x, float y, float z ){ m_Size.Set( x, y, z ); UpdateMatrix( UPDATEFLAG_SIZE ); }
  virtual void                              SetSize( const Vector2& size ){ SetSize( size.x, size.y, m_Size.z ); }
  virtual void                              SetSize( const Vector3& size ){ SetSize( size.x, size.y, size.z ); }
  //virtual void                              Scale( float scale );

  virtual bool                              HasFlag( int flag ){ return ( m_Flags & flag ) != 0; }
  virtual void                              SetFlag( int flag ){ m_Flags |= flag; }
  virtual void                              ClearFlag( int flag ){ m_Flags &= ~flag; }
  bool                                      IsVisible(){ return !HasFlag( FLAGS_ISINVISIBLE ); }
  bool                                      IsModal(){ return HasFlag( FLAGS_ISMODAL ); }
  void                                      SetVisible( bool isVisible);

  virtual void                              SetColor( float red, float green, float blue ){ m_Color.Set( red, green, blue ); }
  virtual void                              SetColor( const Color& color ){ SetColor( color.x, color.y, color.z ); }
  const Color&                              GetColor() const{ return m_Color; }

  virtual void                              SetAlpha( float alpha ){ m_Color.SetAlpha( alpha ); }
  virtual float                             GetAlpha() const{ return m_Color.GetAlpha(); }

  Signal1< void, const CoreBaseObject* >    m_SignalOnDeath;

  void                                      SetParent( CoreBaseObject* parent ){ m_Parent = parent; }
  CoreBaseObject*                           GetParent(){ return m_Parent; }

  virtual bool                              HasDestroy(){ return m_Destroy; }
  virtual void                              FadeIn();
  virtual void                              FadeOutAndDestroy();
  virtual void                              Destroy();
  virtual bool                              HasDeleteChild() const{ return m_DeleteChild; }

#if defined DEBUG
  virtual String                            GetStaticClassName(){ return "CoreBaseObject"; }
  static uint32                             s_ThreadId;
#endif

#if defined DEBUG && defined PLATFORM_WINDOWS
  static void                               PrintLeakedList();
  String                                    m_CallStack;
  static bool                               s_TrackCallstack;
  static bool                               s_CheckLeaks;
#endif

protected:

  void                                      UpdateMatrix( uint32 updateFlag );
  virtual void                              SetDeleteChild();

  uint32                                    m_Instance;
  uint32_t                                  m_ResourceId;

  Vector3                                   m_Position;
  Vector3                                   m_Size;
  Vector3                                   m_Rotation;
  Vector3                                   m_RadRotation;
  Color                                     m_Color;
  int                                       m_Flags;

  bool                                      m_Destroy;
  bool                                      m_DeleteChild;

  Matrix                                    m_LocalMatrix;
  uint32                                    m_UpdateLocalMatrix;

private:
  
  void                                      SetInstance();

  CoreBaseObject*                           m_Parent;
  
#if defined DEBUG && defined PLATFORM_WINDOWS
  static std::vector< CoreBaseObject * >    s_ObjectList;
#endif
};

#endif /// EVILENGINE_COREENGINE_COREBASEOBJECT
