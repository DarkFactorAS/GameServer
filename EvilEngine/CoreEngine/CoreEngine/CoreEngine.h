#ifndef EVILENGINE_COREENGINE_COREENGINE
#define EVILENGINE_COREENGINE_COREENGINE 1
#pragma once

#if defined CLIENT
  #include "EvilEngine/CoreEngine/CoreInput/Keyboard.h"
  #include "EvilEngine/CoreLib/CoreMath/Vector2.h"
#endif

#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"

#include <map>
#include <vector>
#include <list>

class EngineModule;
class CoreScene;

class CoreEngine : public SignalTarget
{
public:

  enum MESSAGE_TYPE
  {
    MESSAGE_CLOSE_KEYBOARD = 1,
    MESSAGE_OPEN_KEYBOARD = 2,
    MESSAGE_CLOSE_APPLICATION = 3,
  };

  CoreEngine();
  ~CoreEngine();

  void                              AddEngineModule(EngineModule* engineModule);
  void                              RemoveEngineModule(EngineModule* engineModule);
  EngineModule*                     GetEngineModule(int moduleId);

  virtual bool                      FrameProcess( float deltaTime );
  virtual bool                      Init(){ return false; };
  virtual void                      Shutdown(){};

  void                              AddMessage(int messageId);
  virtual bool                      ProcessMessages(int* messageId);

  void                              ShutdownEngine(){ m_IsShutdown = true; }

#ifdef CLIENT
  // Input 
  void                              OnTouch(int eventId, const Vector2& position);
  virtual void                      OnMouseButtonDown(int buttonId);
  virtual void                      OnMouseButtonUp(int buttonId);
  virtual void                      OnMouseMove(const Vector2& position);
  virtual void                      OnMouseButtonWheel(float delta, const Vector2& mousePos);
  virtual void                      OnKeyDown(int keyCode);
  virtual void                      OnKeyUp(int keyCode);
  const Vector2&                    GetMousePosition() const { return m_MousePosition; }
  bool                              IsMouseDown(int buttonId);
  Signal1< void, int >              SignalGlobalHotkey;
#endif

  //static CoreEngine*                GetInstance();
  static void                       SetInstance( CoreEngine* engine );

protected:

  bool                              m_IsShutdown;
  std::list< int >                  m_MessageQueue;
  std::vector< EngineModule* >      m_EngineModules;

#ifdef CLIENT
  Vector2                           m_MousePosition;
  std::map<int, bool>               m_MouseButtons;
#endif

private:

  static CoreEngine*                s_CoreEngine;
};

#endif /// EVILENGINE_COREENGINE_COREENGINE