/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for EngineModule modules. This interface
 *               has to be implemented to be able to run as a sub component in the 
 *               CoreEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#ifndef EVILENGINE_COREENGINE_ENGINEMODULE
#define EVILENGINE_COREENGINE_ENGINEMODULE 1

#include "CoreEngine.h"
#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"

class EngineModule
{
public:

  enum CORE_MODULE_TYPE
  {
    COREMODULETYPE_NONE,
    COREMODULETYPE_GUITOOLKIT,
    COREMODULETYPE_SCENE,
    COREMODULETYPE_SERVERLOGIN,
    COREMODULETYPE_PATCHER,
    COREMODULETYPE_EFFECTSYSTEM,
    COREMODULETYPE_GRIDPLAYFIELD,
    COREMODULETYPE_CLIENTLOGIN,
  };

  EngineModule( int moduleId ) :
    m_CoreEngine(NULL),
    m_ModuleId( moduleId )
  {
  };

  EngineModule( int moduleId, CoreEngine* engine ) :
    m_CoreEngine( engine ),
    m_ModuleId( moduleId )
  {
  };

  EngineModule& operator  =   ( const EngineModule& other )
  {
    m_CoreEngine  = other.m_CoreEngine;
    m_ModuleId    = other.m_ModuleId;
    return *this;
  }

  virtual                     ~EngineModule(){};

  void                        SetEngine(CoreEngine* engine) { m_CoreEngine = engine; }
  CoreEngine*                 GetEngine(){ return m_CoreEngine; }
  virtual void                FrameProcess( float deltaTime ) = 0;
  virtual String              GetModuleName() = 0;
  int                         GetModuleId() const { return m_ModuleId; }

#ifdef CLIENT
  virtual bool                OnMouseButtonDown(int /*buttonId*/, float /*xPos*/, float /*yPos*/){ return false; }
  virtual bool                OnMouseButtonUp(int /*buttonId*/, float /*xPos*/, float /*yPos*/){ return false; }
  virtual bool                OnMouseButtonWheel(float /*delta*/, const Vector2& /*mousePos*/) { return false; }
  virtual void                OnMouseMove(float /*xPos*/, float /*yPos*/){}
  virtual bool                OnKeyDown(int /*keyCode*/) { return false; };
  virtual bool                OnKeyUp(int /*keyCode*/) { return false; };
#endif

  virtual void                OnConnectionDisconnected(uint32 /*connectionId*/) {};
  virtual void                OnAccountDisconnected(uint32 /*accountId*/) {};

  CoreEngine*                 m_CoreEngine;
  int                         m_ModuleId;
};

#endif /// EVILENGINE_COREENGINE_ENGINEMODULE
