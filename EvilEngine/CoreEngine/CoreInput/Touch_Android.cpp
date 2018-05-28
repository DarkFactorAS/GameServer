
#include "PreCompile.h"

#if 0
            case AMOTION_EVENT_ACTION_DOWN:
              {
                pointerId = AMotionEvent_getPointerId(event, 0);
                x = AMotionEvent_getX(event, 0);
                y = AMotionEvent_getY(event, 0);

                // Gesture handling
                if ( __gestureEventsProcessed.test(Gesture::GESTURE_TAP) ||
                  __gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) )
                {
                  __pointer0.pressed = true;
                  __pointer0.time = Game::getInstance()->getAbsoluteTime();
                  __pointer0.pointerId = pointerId;
                  __pointer0.x = x;
                  __pointer0.y = y;
                }

                // Primary pointer down.
                gameplay::Platform::touchEventInternal(Touch::TOUCH_PRESS, x, y, pointerId);
                __primaryTouchId = pointerId;
              }
              break;

            case AMOTION_EVENT_ACTION_UP:
              {
                pointerId = AMotionEvent_getPointerId(event, 0);
                x = AMotionEvent_getX(event, 0);
                y = AMotionEvent_getY(event, 0);

                // Gestures
                bool gestureDetected = false;
                if ( __pointer0.pressed &&  __pointer0.pointerId == pointerId)
                {
                  int deltaX = x - __pointer0.x;
                  int deltaY = y - __pointer0.y;

                  // Test for swipe
                  if (__gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) &&
                    gameplay::Game::getInstance()->getAbsoluteTime() - __pointer0.time < GESTURE_SWIPE_DURATION_MAX && 
                    (abs(deltaX) > GESTURE_SWIPE_DISTANCE_MIN || abs(deltaY) > GESTURE_SWIPE_DISTANCE_MIN) )
                  {
                    int direction = 0;
                    if ( abs(deltaX) > abs(deltaY) )
                    {
                      if (deltaX > 0)
                        direction = gameplay::Gesture::SWIPE_DIRECTION_RIGHT;
                      else if (deltaX < 0)
                        direction = gameplay::Gesture::SWIPE_DIRECTION_LEFT;
                    }
                    else
                    {
                      if (deltaY > 0)
                        direction = gameplay::Gesture::SWIPE_DIRECTION_DOWN;
                      else if (deltaY < 0)
                        direction = gameplay::Gesture::SWIPE_DIRECTION_UP;
                    }
                    gameplay::Platform::gestureSwipeEventInternal(x, y, direction);
                    __pointer0.pressed = false;
                    gestureDetected = true;
                  }
                  else if(__gestureEventsProcessed.test(Gesture::GESTURE_TAP) &&
                    gameplay::Game::getInstance()->getAbsoluteTime() - __pointer0.time < GESTURE_TAP_DURATION_MAX)
                  {
                    gameplay::Platform::gestureTapEventInternal(x, y);
                    __pointer0.pressed = false;
                    gestureDetected = true;
                  }
                }

                if (!gestureDetected && (__multiTouch || __primaryTouchId == pointerId) )
                {
                  gameplay::Platform::touchEventInternal(Touch::TOUCH_RELEASE, x, y, pointerId);
                }
                __primaryTouchId = -1;
              }
              break;

            case AMOTION_EVENT_ACTION_POINTER_DOWN:
              {
                pointerId = AMotionEvent_getPointerId(event, 0);
                x = AMotionEvent_getX(event, 0);
                y = AMotionEvent_getY(event, 0);

                // Gesture handling
                if ( __gestureEventsProcessed.test(Gesture::GESTURE_TAP) ||
                  __gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) )
                {
                  __pointer1.pressed = true;
                  __pointer1.time = Game::getInstance()->getAbsoluteTime();
                  __pointer1.pointerId = pointerId;
                  __pointer1.x = x;
                  __pointer1.y = y;
                }

                // Non-primary pointer down.
                if (__multiTouch)
                {
                  pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                  pointerId = AMotionEvent_getPointerId(event, pointerIndex);
                  gameplay::Platform::touchEventInternal(Touch::TOUCH_PRESS, AMotionEvent_getX(event, pointerIndex), AMotionEvent_getY(event, pointerIndex), pointerId);
                }
              }
              break;

            case AMOTION_EVENT_ACTION_POINTER_UP:
              {
                pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                pointerId = AMotionEvent_getPointerId(event, pointerIndex);
                x = AMotionEvent_getX(event, 0);
                y = AMotionEvent_getY(event, 0);

                bool gestureDetected = false;
                if ( __pointer1.pressed &&  __pointer1.pointerId == pointerId)
                {
                  int deltaX = x - __pointer1.x;
                  int deltaY = y - __pointer1.y;

                  // Test for swipe
                  if (__gestureEventsProcessed.test(Gesture::GESTURE_SWIPE) &&
                    gameplay::Game::getInstance()->getAbsoluteTime() - __pointer1.time < GESTURE_SWIPE_DURATION_MAX && 
                    (abs(deltaX) > GESTURE_SWIPE_DISTANCE_MIN || abs(deltaY) > GESTURE_SWIPE_DISTANCE_MIN) )
                  {
                    int direction = 0;
                    if (deltaX > 0)
                      direction |= gameplay::Gesture::SWIPE_DIRECTION_RIGHT;
                    else if (deltaX < 0)
                      direction |= gameplay::Gesture::SWIPE_DIRECTION_LEFT;

                    if (deltaY > 0)
                      direction |= gameplay::Gesture::SWIPE_DIRECTION_DOWN;
                    else if (deltaY < 0)
                      direction |= gameplay::Gesture::SWIPE_DIRECTION_UP;

                    gameplay::Platform::gestureSwipeEventInternal(x, y, direction);
                    __pointer1.pressed = false;
                    gestureDetected = true;
                  }
                  else if(__gestureEventsProcessed.test(Gesture::GESTURE_TAP) &&
                    gameplay::Game::getInstance()->getAbsoluteTime() - __pointer1.time < GESTURE_TAP_DURATION_MAX)
                  {
                    gameplay::Platform::gestureTapEventInternal(x, y);
                    __pointer1.pressed = false;
                    gestureDetected = true;
                  }
                }

                if (!gestureDetected && (__multiTouch || __primaryTouchId == pointerId) )
                {
                  gameplay::Platform::touchEventInternal(Touch::TOUCH_RELEASE, AMotionEvent_getX(event, pointerIndex), AMotionEvent_getY(event, pointerIndex), pointerId);
                }
                if (__primaryTouchId == pointerId)
                  __primaryTouchId = -1;
              }
              break;

            case AMOTION_EVENT_ACTION_MOVE:
              {
                // ACTION_MOVE events are batched, unlike the other events.
                pointerCount = AMotionEvent_getPointerCount(event);
                for (size_t i = 0; i < pointerCount; ++i)
                {
                  pointerId = AMotionEvent_getPointerId(event, i);
                  if (__multiTouch || __primaryTouchId == pointerId)
                  {
                    gameplay::Platform::touchEventInternal(Touch::TOUCH_MOVE, AMotionEvent_getX(event, i), AMotionEvent_getY(event, i), pointerId);
                  }
                }
              }
              break;
        }
        return 1;

#endif
