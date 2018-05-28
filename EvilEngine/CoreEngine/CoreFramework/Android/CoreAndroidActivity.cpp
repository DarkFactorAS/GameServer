
#include "PreCompile.h"
#include "CoreAndroidActivity.h"
#include "CoreAndroidUtils.h"

#if defined PLATFORM_ANDROID

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"
#include "EvilEngine/CoreEngine/CoreInput/Touch.h"

void CoreAndroidActivity::HandleTouchEvents( uint32 pointerIndex, uint32 actionId, float xPos, float yPos )
{
  switch( actionId )
  {
    // This event is sent on the first touch and that is always LMB
    // 0x00000000 - ACTION_DOWN
    case 0:
      LogInfoFMT("AndroidMain", "Action : %d [ pIndex:%d ] pos[%0.2f x %0.2f]", actionId, pointerIndex, xPos, yPos );
      CoreInputManager::GetInstance()->HandleMouseMove( Touch::FIRST_MOUSE_BUTTON + pointerIndex, xPos, yPos );
      CoreInputManager::GetInstance()->HandleMouseButtonDown( Touch::LEFT_MOUSE_BUTTON );
      break;

    // 0x00000002 - ACTION_MOVE
    case 2: 
      {
        //LogInfoFMT("AndroidMain", "Action : %d [ pIndex:%d ] pos[%0.2f x %0.2f]", actionId, pointerIndex, xPos, yPos );
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::FIRST_MOUSE_BUTTON + pointerIndex, xPos, yPos );
      }
      break;

    // 0x00000005 - ACTION_POINTER_DOWN
    case 5:
      LogInfoFMT("AndroidMain", "Action : %d [ pIndex:%d ] pos[%0.2f x %0.2f]", actionId, pointerIndex, xPos, yPos );
      CoreInputManager::GetInstance()->HandleMouseMove( Touch::FIRST_MOUSE_BUTTON + pointerIndex, xPos, yPos );
      CoreInputManager::GetInstance()->HandleMouseButtonDown( Touch::FIRST_MOUSE_BUTTON + pointerIndex );
      break;

    // Called when a touchpoint is going up but it is not the last one
    // 0x00000006 - ACTION_POINTER_UP
    case 6:
      LogInfoFMT("AndroidMain", "Action : %d [ pIndex:%d ] pos[%0.2f x %0.2f]", actionId, pointerIndex, xPos, yPos );
      CoreInputManager::GetInstance()->HandleMouseButtonUp( Touch::FIRST_MOUSE_BUTTON + pointerIndex );
      break;

    // This is called when the last touch point is going up
    // 0x00000001 - ACTION_UP
    case 1:
      LogInfoFMT("AndroidMain", "Action : %d [ pIndex:%d ] pos[%0.2f x %0.2f]", actionId, pointerIndex, xPos, yPos );
      CoreInputManager::GetInstance()->HandleMouseButtonUp( Touch::FIRST_MOUSE_BUTTON + pointerIndex );
      break;

    default:
      LogInfoFMT("AndroidMain", "Action : %d [ pIndex:%d ] pos[%0.2f x %0.2f]", actionId, pointerIndex, xPos, yPos );
      break;
  }
}

#endif
