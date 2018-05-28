//
//  LoginViewController.h
//  CoreGameLogin
//
//  Created by Thor Richard Hanssen on 17/02/15.
//
//

#ifndef CoreGameLogin_LoginViewController_h
#define CoreGameLogin_LoginViewController_h

#ifdef GAMECENTER_ENABLED

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include "GameCenterManager.iOS.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"

namespace EvilEngine
{
  extern Signal4< void, String, String, String, uint8 > SignalLoginAccount;
  extern Signal1< void, String >                        SignalLoginFailed;
}

@interface GameCenterViewController : GLKViewController < GameCenterManagerDelegate >
{
  GameCenterManager*        m_GameCenter;
}
- (void) initLogin;
- (void) loginFailed: (NSError*) error;
- (void) authenticatedPlayer: (GKLocalPlayer*) localPlayer error: (NSError*) error;
- (void) processGameCenterAuth: (UIViewController*) viewController error: (NSError*) error;

@end

#endif

#endif
