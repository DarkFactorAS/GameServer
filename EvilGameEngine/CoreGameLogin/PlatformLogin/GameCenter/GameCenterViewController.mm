/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : GameCenterViewController.mm
 *
 * Description : Game Center Login view for iOS
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"

#ifdef GAMECENTER_ENABLED

#import <GameKit/GameKit.h>
#include <CommonCrypto/CommonDigest.h>

#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#import "GameCenterViewController.h"


@interface GameCenterViewController ()

- (void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

@implementation GameCenterViewController

- (void) initLogin
{
  if ( [ GameCenterManager isGameCenterAvailable ] )
  {
    if ( m_GameCenter == nullptr )
    {
      m_GameCenter = [[ GameCenterManager alloc ] init ];
      [ m_GameCenter setDelegate: self ];
    }
    [ m_GameCenter authenticateLocalUser ];
  }
  else
  {
    EvilEngine::SignalLoginFailed( "Game Center is not available" );
  }
}

/************************************************************************************************
 * alertView:
 * Called when the user closes the alert view/dialog that displayed the error message.
 * We use this to close the game center login and report login failure to C++
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
  NSLog(@"GameCenter user closed errorDialog" );
  EvilEngine::SignalLoginFailed("Game Center failed");
}

/************************************************************************************************
 * processGameCenterAuth:
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void) processGameCenterAuth: (UIViewController*) viewController error: (NSError*) error
{
  if(error != NULL || viewController == nullptr )
  {
    [ self loginFailed: error ];
  }
  else
  {
    // Show the iOS login view to Game Center since the player is not logged in
    [ self presentViewController:viewController animated:YES completion:nil ];
  }
}

/************************************************************************************************
 * authenticatedPlayer:
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void) authenticatedPlayer: (GKLocalPlayer*) localPlayer error: (NSError*) error
{
  // Delegate the callback for generateIdentityVerificationSignatureWithCompletionHandler
  [localPlayer generateIdentityVerificationSignatureWithCompletionHandler:^(NSURL *publicKeyUrl, NSData *signature, NSData *salt, uint64_t timestamp, NSError *error)
   {
     if(error != nil)
     {
       return; //some sort of error, can't authenticate right now
     }
     
     // Username = LocalPlayerID | app_bundle_id | timestamp | salt
     // Password = signed request
     //NSString* nsUrl        = [ [ NSData dataWithContentsOfURL:publicKeyUrl ] base64EncodedStringWithOptions:0 ];
     NSString* bundleId     = [[NSBundle mainBundle] bundleIdentifier];
     NSString* saltData     = [ salt base64EncodedStringWithOptions:0 ];
     NSString* nsUsername   = [ NSString stringWithFormat:@"%@|%@|%llu|%@|%@", bundleId, localPlayer.playerID, timestamp, saltData, publicKeyUrl ];
     NSString* nsPassword   = [ signature base64EncodedStringWithOptions:0 ];
     
     NSLog(@"GameCenter : authenticateLocalUser::BundleId           : %@", bundleId );
     NSLog(@"GameCenter : authenticateLocalUser::Auth URL           : %@", publicKeyUrl );
     //NSLog(@"GameCenter : authenticateLocalUser::Auth URL b64       : %@", nsUrl );
     NSLog(@"GameCenter : authenticateLocalUser::Auth Signature     : %@", signature );
     NSLog(@"GameCenter : authenticateLocalUser::Auth Signature b64 : %@", nsPassword );
     NSLog(@"GameCenter : authenticateLocalUser::Auth Salt          : %@", salt );
     NSLog(@"GameCenter : authenticateLocalUser::Auth Salt b64      : %@", saltData );
     NSLog(@"GameCenter : authenticateLocalUser::Auth Timestamp     : %llu", timestamp );
     
     const char* username   = [ nsUsername cStringUsingEncoding:NSUTF8StringEncoding ];
     const char* password   = [ nsPassword cStringUsingEncoding:NSUTF8StringEncoding ];
     const char* playername = [ localPlayer.alias cStringUsingEncoding:[NSString defaultCStringEncoding] ];
     
     EvilEngine::SignalLoginAccount( username, password, playername, Account::LOGIN_METHOD_IOSGAMECENTERCREATE );
   }];
}

/************************************************************************************************
 * loginFailed:
 * Called when the Game Center fails for some reason. This will trigger a login fail from
 * Game Center and notifies the c++ code to act on the failure.
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void) loginFailed: (NSError*) error
{
  if ( error != nullptr )
  {
    NSLog(@"GameCenter loginFailed : %@", [error localizedDescription] );
    UIAlertView* alert= [[UIAlertView alloc] initWithTitle: @"Game Center Login Failed"
                                                 message: [NSString stringWithFormat: @"Reason: %@", [error localizedDescription]]
                                                delegate: self cancelButtonTitle: @"Close" otherButtonTitles: NULL];
    [alert show];
  }
  else
  {
    UIAlertView* alert= [[UIAlertView alloc] initWithTitle: @"Game Center Login Failed"
                                                   message: @"Unknown error"
                                                  delegate: self cancelButtonTitle: @"Close" otherButtonTitles: NULL];
    [alert show];
  }
}

@end

#endif
