#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_COREEFFECTSYSTEM
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_COREEFFECTSYSTEM 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreEngine/CoreEngine/CoreEngine.h"

class CoreGameEngine;
class EffectPackage;
class CoreBaseObject;

class CoreEffectSystem : public EngineModule
{
public:
    
                  CoreEffectSystem();
  virtual         ~CoreEffectSystem();

  // Implemented from EngineModule interface
  virtual void    FrameProcess( float deltaTime ) DF_OVERRIDE;
  virtual String  GetModuleName() DF_OVERRIDE { return StaticStr("CoreEffectSystem"); }

#ifdef CLIENT
  virtual bool    OnMouseButtonDown( int /*buttonId*/, float /*xPos*/, float /*yPos*/ ) DF_OVERRIDE { return false; };
  virtual bool    OnMouseButtonUp( int /*buttonId*/, float /*xPos*/, float /*yPos*/ ) DF_OVERRIDE { return false; };
  virtual void    OnMouseMove( float /*xPos*/, float /*yPos*/ ) DF_OVERRIDE {};
  //virtual void    OnMouseDrag( float /*xPos*/, float /*yPos*/ ) DF_OVERRIDE {};
#endif

  void            AddEffectPackage( EffectPackage* effectPackage );
  EffectPackage*  CreateEffectPackage( CoreBaseObject* runObject );
  void            RemoveEffectPackage( EffectPackage* effectPackage );
  void            RemoveEffectpackagesOnObject( CoreBaseObject* runObject );

private:

  std::vector< EffectPackage* > m_EffectPackageList;
};

#endif /// EVILENGINE_COREGAMEENGINE_CORESCREENMODULE
