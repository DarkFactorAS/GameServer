
#include "Precompile.h"
#include "CoreEngine.h"
#include "EngineModule.h"
#include "EvilEngine/CoreEngine/CoreEventTimer/EventTimerManager.h"
#include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

CoreEngine* CoreEngine::s_CoreEngine = NULL;

CoreEngine::CoreEngine() : 
  m_IsShutdown(false)
{
#ifdef CLIENT
  CoreInputManager::GetInstance()->SignalTouchEvent.Connect(this, &CoreEngine::OnTouch);
  CoreInputManager::GetInstance()->SignalMouseWheel.Connect(this, &CoreEngine::OnMouseButtonWheel);
#endif
}

CoreEngine::~CoreEngine()
{
}

//CoreEngine* CoreEngine::GetInstance()
//{
//  dfAssert( s_CoreEngine != NULL, "GetEngine is NULL!" );
//  return s_CoreEngine;
//}

void CoreEngine::SetInstance(CoreEngine* engine)
{
  dfBugreport(s_CoreEngine == NULL, "GetEngine already set!");
  s_CoreEngine = engine;
}

/************************************************************************************************
* AddEngineModule:
* Adds a gamemodule so it can be runned in the engine loop
*
* @param  (EngineModule) engineModule      - The engine module we want to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEngine::AddEngineModule( EngineModule* engineModule )
{
  LogInfoFMT("CoreEngine","Add engine module : %d/%s", engineModule->GetModuleId(), engineModule->GetModuleName().c_str() );
#ifdef DEBUG
  dfAssertFMT(engineModule->GetModuleId() != 0, "Module '%s' cannot have ID 0!", engineModule->GetModuleName().c_str() );
  for ( std::vector< EngineModule* >::const_iterator itModule = m_EngineModules.begin(); itModule != m_EngineModules.end(); ++itModule )
  {
      EngineModule* module = *itModule;
      dfAssertFMT( module->GetModuleId() != engineModule->GetModuleId() , "Module with type %d is already registered!", module->GetModuleId() );
  }
#endif
  engineModule->SetEngine( this );
  m_EngineModules.push_back( engineModule );
}

/************************************************************************************************
* RemoveEngineModule:
* Removes a gamemodule so it is not runned in the game engine loop anymore
*
* @param  (EngineModule)         - The engine module we want to remove
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEngine::RemoveEngineModule( EngineModule* removeEngineModule )
{
  for ( std::vector<EngineModule*>::iterator itModule = m_EngineModules.begin(); itModule != m_EngineModules.end(); itModule++ )
  {
    EngineModule* engineModule = *itModule;
    if ( engineModule == removeEngineModule )
    {
      LogInfoFMT("CoreEngine","Removed engine module : %d/%s", engineModule->GetModuleId(), engineModule->GetModuleName().c_str() );
      m_EngineModules.erase( itModule );
      return;
    }
  }
}

/************************************************************************************************
* GetEngineModule:
* Return a engine module with a specific ID
*
* @param  (moduleId)           - ID of the module to search for
* @return (EngineModule*)      - The engineModule module with the matching ID
*
* @author Thor Richard Hansen
*************************************************************************************************/
EngineModule* CoreEngine::GetEngineModule( int moduleId )
{
  for ( std::vector<EngineModule*>::iterator itModule = m_EngineModules.begin(); itModule != m_EngineModules.end(); itModule++ )
  {
    EngineModule* engineModule = *itModule;
    if ( engineModule->GetModuleId() == moduleId )
    {
      return engineModule;
    }
  }
  return NULL;
}

bool CoreEngine::FrameProcess( float deltaTime )
{
  if ( m_IsShutdown )
  {
    return false;
  }

#ifdef CLIENT
  // Process mouse/keyboard/input
  CoreInputManager::GetInstance()->ProcessEvents();
#endif

  for (size_t index = 0; index < m_EngineModules.size(); index++)
  {
    EngineModule* engineModule = m_EngineModules[index];
    engineModule->FrameProcess(deltaTime);
  }

  EventTimerManager::GetInstance()->FrameProcess( deltaTime );
  return true;
}

/************************************************************************************************
* OnMouseButtonDown:
* Forward the mouse button down event to all running modules
*
* @param  (int)     buttonId  - What button this is ( Left, Right, Middle, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT

void CoreEngine::OnTouch(int eventId, const Vector2& position)
{
  switch (eventId)
  {
  case CoreInputManager::INPUT_TOUCH_DOWN:
    OnMouseButtonDown(CoreInputManager::INPUT_TOUCH_1);
    break;
  case CoreInputManager::INPUT_TOUCH2_DOWN:
    OnMouseButtonDown(CoreInputManager::INPUT_TOUCH_2);
    break;
  case CoreInputManager::INPUT_TOUCH_UP:
    OnMouseButtonUp(CoreInputManager::INPUT_TOUCH_1);
    break;
  case CoreInputManager::INPUT_TOUCH2_UP:
    OnMouseButtonUp(CoreInputManager::INPUT_TOUCH_2);
    break;
  case CoreInputManager::INPUT_MOUSEMOVE:
    OnMouseMove( position);
    break;
  case CoreInputManager::INPUT_MOUSEMOVE2:
    {
      if ( !CoreInputManager::GetInstance()->DoesSupportMultitouch() )
      {
        OnMouseMove( position);
      }
      break;
    }
  }
}

void CoreEngine::OnMouseButtonDown(int buttonId)
{
  m_MouseButtons[buttonId] = true;

  // Run all engine modules
  for (size_t index = 0; index < m_EngineModules.size(); index++)
  {
    EngineModule* engineModule = m_EngineModules[index];
    if (engineModule->OnMouseButtonDown(buttonId, m_MousePosition.x, m_MousePosition.y))
    {
      return;
    }
  }
}

/************************************************************************************************
* OnMouseButtonUp:
* Forward the mouse button up event to all running modules
*
* @param  (int)     buttonId  - What button this is ( Left, Right, Middle, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEngine::OnMouseButtonUp(int buttonId)
{
  m_MouseButtons[buttonId] = false;

  for (size_t index = 0; index < m_EngineModules.size(); index++)
  {
    EngineModule* engineModule = m_EngineModules[index];
    engineModule->OnMouseButtonUp(buttonId, m_MousePosition.x, m_MousePosition.y);
  }
}

bool CoreEngine::IsMouseDown(int buttonId)
{
  std::map<int, bool>::const_iterator itButton = m_MouseButtons.find(buttonId);
  if (itButton != m_MouseButtons.end())
  {
    return itButton->second;
  }
  return false;
}

/************************************************************************************************
* OnMouseButtonWheel:
* The user is using the scrollwheel on the mouse or a scroll guesture
*
* @param  (short)     delta  - How much the scrollwheel changed
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEngine::OnMouseButtonWheel(float delta, const Vector2& mousePos)
{
  for (size_t index = 0; index < m_EngineModules.size(); index++)
  {
    EngineModule* engineModule = m_EngineModules[index];
    engineModule->OnMouseButtonWheel(delta, mousePos);
  }
}

/************************************************************************************************
* OnMouseMove:
* Forward the mouse move event to all running modules
*
* @param  (float)     xPos  - The new x position for the mouse/touchpoint
* @param  (float)     yPos  - The new y position for the mouse/touchpoint
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEngine::OnMouseMove(const Vector2& position)
{
  float deltaX = position.x - m_MousePosition.x;
  float deltaY = position.y - m_MousePosition.y;
  m_MousePosition.Set(position.x, position.y);
  for (size_t index = 0; index < m_EngineModules.size(); index++)
  {
    EngineModule* engineModule = m_EngineModules[index];
    engineModule->OnMouseMove(deltaX, deltaY);
  }
}

void CoreEngine::OnKeyDown(int keyCode)
{
  for (size_t index = 0; index < m_EngineModules.size(); index++)
  {
    EngineModule* engineModule = m_EngineModules[index];
    if (engineModule->OnKeyDown(keyCode))
    {
      return;
    }
  }
  SignalGlobalHotkey(keyCode);
}

void CoreEngine::OnKeyUp(int keyCode)
{
  for (size_t index = 0; index < m_EngineModules.size(); index++)
  {
    EngineModule* engineModule = m_EngineModules[index];
    engineModule->OnKeyUp(keyCode);
  }
}

#endif

void CoreEngine::AddMessage(int messageId)
{
  m_MessageQueue.push_back(messageId);
}

bool CoreEngine::ProcessMessages(int* messageId)
{
  if (!m_MessageQueue.empty())
  {
    *messageId = *m_MessageQueue.begin();
    m_MessageQueue.pop_front();
    return true;
  }
  return false;
}

