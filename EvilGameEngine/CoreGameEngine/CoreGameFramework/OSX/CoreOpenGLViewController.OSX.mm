/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014 - 2016
 * File        : CoreOpenGLViewController
 *
 * Description : Contains most of the code to create and initialize an OSX window.
 *               
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "EvilGameEngine/CoreGameEngine/Precompile.h"

#ifdef PLATFORM_OSX

#import "CoreOpenGLViewController.OSX.h"

#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreEngine/CoreInput/Keyboard.h"
#include "EvilEngine/CoreEngine/CoreInput/Touch.h"
#include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"

#include "EvilEngine/CoreEngine/CoreFramework/iOS/InAppPurchaseManager.iOS.h"

#import <QuartzCore/CAAnimation.h>

class CoreOpenGLViewWrapper : public SignalTarget
{
public:
  
  CoreOpenGLViewWrapper()
  {
    m_CallbackView = NULL;
  }
  
  void CreateCallback( CoreOpenGLRender* openGL, CoreOpenGLViewController* view )
  {
    m_CallbackView = view;
    openGL->s_SignalRequestScreenResize.Connect( this, &CoreOpenGLViewWrapper::UpdateView );
  }
  
  void UpdateView( uint32 width, uint32 height, uint8 mode )
  {
    if ( m_CallbackView != NULL )
    {
      [ m_CallbackView changeResolution:width height:height ];
    }
  }
  
  CoreOpenGLViewController* m_CallbackView;
};

@implementation CoreOpenGLViewController
{
  CoreOpenGLViewWrapper* m_Mapper;
}


-(void) initWindow:(CoreGameEngine*) gameEngine
{
    Log::Initialize("Darkfactor");
    
    m_RenderTimer = nil;

    //get the documents directory:
    NSArray *paths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory   = [paths objectAtIndex:0];
    const char *cDocumentDirectory = [documentsDirectory UTF8String];
    
    // Create the resource manager first since sub modules might use it.
    // And we wnat to make sure we are creating the IOS resource manager
    CoreResourceManager* resourceManager = CoreResourceManager::GetInstance();
    resourceManager->AddSearchPath("Contents/Resources");
    resourceManager->SetUserFolder( cDocumentDirectory );

    // Set the save dir for the config
    UserConfig::SetConfigPath( cDocumentDirectory );

    //[ NSApp setDelegate:self ];   // We want delegate notifications
  
    [ m_OpenGLWindow makeFirstResponder:self ];
    [ m_OpenGLWindow setAcceptsMouseMovedEvents:YES];
    m_OpenGLView = [ [ CoreOpenGLView alloc ] initWithFrame:[ m_OpenGLWindow frame ] colorBits:16 depthBits:16 fullscreen:FALSE gameEngine:gameEngine ];
    if( m_OpenGLView != nil )
    {
        [ m_OpenGLWindow setContentView:m_OpenGLView ];
        [ m_OpenGLWindow makeKeyAndOrderFront:self ];

        CoreOpenGLRender* renderEngine = [ m_OpenGLView getRender ];
        m_Mapper = new CoreOpenGLViewWrapper();
        m_Mapper->CreateCallback( renderEngine, self );
      
        [ self setupRenderTimer ];
    }
    else
        [ self createFailed ];
    
    // Initialize the purchase manager
    Client::InitializeItemShop();
}

/*
 * Setup timer to update the OpenGL view.
 */
- (void) setupRenderTimer
{
    NSTimeInterval timeInterval = 0.005;
    m_LastTime = CACurrentMediaTime();
    
    m_RenderTimer = [ NSTimer scheduledTimerWithTimeInterval:timeInterval
                                                      target:self
                                                    selector:@selector( updateGLView: )
                                                    userInfo:nil repeats:YES ];
    [ [ NSRunLoop currentRunLoop ] addTimer:m_RenderTimer forMode:NSEventTrackingRunLoopMode ];
    [ [ NSRunLoop currentRunLoop ] addTimer:m_RenderTimer forMode:NSModalPanelRunLoopMode ];
}

- (void) changeResolution:(int)width height:(int)height
{
  //[ m_OpenGLView changeResolution:NSMakeRect( 0, 0, width, height ) ];
  //[ m_OpenGLWindow setContentSize:[ m_OpenGLView frame ].size ];
  [ m_OpenGLWindow setContentSize:NSMakeSize( width, height ) ];
  [ m_OpenGLWindow center ];
}

/*
 * Called by the rendering timer.
 */
- (void) updateGLView:(NSTimer *)timer
{
    double currentTime = CACurrentMediaTime();
    float deltaTime = ( currentTime - m_LastTime );
    m_LastTime = CACurrentMediaTime();
  
    if( m_OpenGLView != nil )
    {
      BOOL updatedView = [ m_OpenGLView updateView:[ m_OpenGLView frame ] deltaTime:deltaTime ];
      if ( updatedView == FALSE )
      {
        [ NSApp terminate:self ];
      }
    }
}


/*
 * Set full screen.
 */
- (IBAction)setFullScreen:(id)sender
{
    [ m_OpenGLWindow setContentView:nil ];
    if( [ m_OpenGLView isFullScreen ] )
    {
        if( ![ m_OpenGLView setFullScreen:FALSE inFrame:[ m_OpenGLWindow frame ] ] )
            [ self createFailed ];
        else
            [ m_OpenGLWindow setContentView:m_OpenGLView ];
    }
    else
    {
        if( ![ m_OpenGLView setFullScreen:TRUE inFrame:NSMakeRect( 0, 0, 800, 600 ) ] )
        {
            [ self createFailed ];
        }
    }
}


/*
 * Called if we fail to create a valid OpenGL view
 */
- (void) createFailed
{
    NSWindow *infoWindow;
    
    infoWindow = NSGetCriticalAlertPanel( @"Initialization failed",
                                         @"Failed to initialize OpenGL",
                                         @"OK", nil, nil );
    [ NSApp runModalForWindow:infoWindow ];
    [ infoWindow close ];
    [ NSApp terminate:self ];
}


/*
 * Cleanup
 */
- (void) dealloc
{
    //[ m_OpenGLWindow release ];
    //[ m_OpenGLView release ];
    if( m_RenderTimer != nil && [ m_RenderTimer isValid ] )
    {
        [ m_RenderTimer invalidate ];
    }
}

@end

#endif
