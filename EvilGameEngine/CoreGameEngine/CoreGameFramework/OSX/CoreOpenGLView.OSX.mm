/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014 - 2016
 * File        : CoreOpenGLView
 *
 * Description : Contains most of the code to create and initialize an OSX window.
 *               
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "EvilGameEngine/CoreGameEngine/Precompile.h"

#ifdef PLATFORM_OSX

#import "CoreOpenGLView.OSX.h"

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreEngine/CoreInput/Keyboard.h"
#include "EvilEngine/CoreEngine/CoreInput/Touch.h"
#include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"

@interface CoreOpenGLView (InternalMethods)
  - (NSOpenGLPixelFormat *) createPixelFormat:(NSRect)frame;
  - (void) switchToOriginalDisplayMode;
@end

@implementation CoreOpenGLView

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen gameEngine:(CoreGameEngine*)gameEngine
{
    NSOpenGLPixelFormat *pixelFormat;
    
    colorBits = numColorBits;
    depthBits = numDepthBits;
    runningFullScreen = runFullScreen;
    originalDisplayMode = (NSDictionary *) CGDisplayCurrentMode(
                                                                kCGDirectMainDisplay );
    pixelFormat = [ self createPixelFormat:frame ];
    if( pixelFormat != nil )
    {
        self = [ super initWithFrame:frame pixelFormat:pixelFormat ];
        //[ pixelFormat release ];
        if( self )
        {
            [ [ self openGLContext ] makeCurrentContext ];
            if( runningFullScreen )
            {
                [ [ self openGLContext ] setFullScreen ];
            }
        }
    }
    else
        self = nil;
    
    // Initialize the game engine. This is where all the objects are created
    m_GameEngine = gameEngine;
    m_GameEngine->Init();
    m_GameEngine->FrameProcess( 0.0f );

    [ [ self openGLContext ] makeCurrentContext ];
    m_OpenGLScreen  = new CoreOpenGLRender();
    //m_OpenGLScreen->s_SignalRequestScreenResize.Connect( &CoreOpenGLView::UpdateResolution );
    m_ScreenWidth   = self.bounds.size.width;
    m_ScreenHeight  = self.bounds.size.height;
  
    m_GameEngine->SetRender( m_OpenGLScreen );

    CoreInputManager::GetInstance()->SupportMultitouch( false );
  
    // Force widescreen
    if ( m_ScreenHeight > m_ScreenWidth )
    {
        m_ScreenWidth   = self.bounds.size.height;
        m_ScreenHeight  = self.bounds.size.width;
    }
    
    if ( !m_OpenGLScreen->InitOpenGL( m_ScreenWidth, m_ScreenHeight, 16 ) )
    {
        LogError("DarkFactor", "Failed to initalize OpenGL!");
        exit( -1 );
    }

    [ self reshape ];

    return self;
}

/*
 * Cleanup
 */
- (void) dealloc
{
    if( runningFullScreen )
        [ self switchToOriginalDisplayMode ];
    //[ originalDisplayMode release ];
    
    m_GameEngine = NULL;
    m_OpenGLScreen = NULL;
}

- (CoreOpenGLRender*) getRender
{
  return m_OpenGLScreen;
}


/*
 * Create a pixel format and possible switch to full screen mode
 */
- (NSOpenGLPixelFormat *) createPixelFormat:(NSRect)frame
{
    NSOpenGLPixelFormatAttribute pixelAttribs[ 16 ];
    int pixNum = 0;
    NSDictionary *fullScreenMode;
    NSOpenGLPixelFormat *pixelFormat;
    
    pixelAttribs[ pixNum++ ] = NSOpenGLPFADoubleBuffer;
    pixelAttribs[ pixNum++ ] = NSOpenGLPFAAccelerated;
    pixelAttribs[ pixNum++ ] = NSOpenGLPFAColorSize;
    pixelAttribs[ pixNum++ ] = colorBits;
    pixelAttribs[ pixNum++ ] = NSOpenGLPFADepthSize;
    pixelAttribs[ pixNum++ ] = depthBits;
    
    if( runningFullScreen )  // Do this before getting the pixel format
    {
        pixelAttribs[ pixNum++ ] = NSOpenGLPFAFullScreen;
        fullScreenMode = (NSDictionary *) CGDisplayBestModeForParameters(
                                                                         kCGDirectMainDisplay,
                                                                         colorBits, frame.size.width,
                                                                         frame.size.height, NULL );
        CGDisplayCapture( kCGDirectMainDisplay );
        CGDisplayHideCursor( kCGDirectMainDisplay );
        CGDisplaySwitchToMode( kCGDirectMainDisplay,
                              (CFDictionaryRef) fullScreenMode );
    }
    pixelAttribs[ pixNum ] = 0;
    pixelFormat = [ [ NSOpenGLPixelFormat alloc ]
                   initWithAttributes:pixelAttribs ];
    
    return pixelFormat;
}


- (void) changeResolution:(NSRect)frame
{ 
  [ super setFrame:frame ];
  [ self reshape ];
}

/*
 * Enable/disable full screen mode
 */
- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame
{
    BOOL success = FALSE;
    NSOpenGLPixelFormat *pixelFormat;
    NSOpenGLContext *newContext;
    
    [ [ self openGLContext ] clearDrawable ];
    if( runningFullScreen )
        [ self switchToOriginalDisplayMode ];
    runningFullScreen = enableFS;
    pixelFormat = [ self createPixelFormat:frame ];
    if( pixelFormat != nil )
    {
        newContext = [ [ NSOpenGLContext alloc ] initWithFormat:pixelFormat
                                                   shareContext:nil ];
        if( newContext != nil )
        {
            [ super setFrame:frame ];
            [ super setOpenGLContext:newContext ];
            [ newContext makeCurrentContext ];
            if( runningFullScreen )
                [ newContext setFullScreen ];
            [ self reshape ];
            //if( [ self initGL ] )
            //    success = TRUE;
        }
        //[ pixelFormat release ];
    }
    if( !success && runningFullScreen )
        [ self switchToOriginalDisplayMode ];
    
    return success;
}


/*
 * Switch to the display mode in which we originally began
 */
- (void) switchToOriginalDisplayMode
{
    CGDisplaySwitchToMode( kCGDirectMainDisplay,
                          (CFDictionaryRef) originalDisplayMode );
    CGDisplayShowCursor( kCGDirectMainDisplay );
    CGDisplayRelease( kCGDirectMainDisplay );
}

/*
 * Resize ourself
 */
- (void) reshape
{
    m_ScreenWidth  = self.bounds.size.width;
    m_ScreenHeight = self.bounds.size.height;
    m_OpenGLScreen->ResizeScreen( m_ScreenWidth, m_ScreenHeight );
}

- (BOOL) needResize:(NSRect)rect
{
  if ( rect.size.width != m_ScreenWidth || rect.size.height != m_ScreenHeight )
  {
    return TRUE;
  }
  return FALSE;
}


/*
 * Called when the system thinks we need to draw.
 */
- (BOOL) updateView:(NSRect)rect deltaTime:(float)deltaTime
{
    // Clear the screen and depth buffer
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glLoadIdentity();   // Reset the current modelview matrix
    //[ [ self openGLContext ] flushBuffer ];
    
    m_DeltaTime = deltaTime;

    if ( m_GameEngine != NULL )
    {
        m_GameEngine->FrameProcess( m_DeltaTime );
      
        int msg = 0;
        while( m_GameEngine->ProcessMessages( &msg ) )
        {
            switch( msg )
            {
              case CoreGameEngine::MESSAGE_CLOSE_APPLICATION:
                return FALSE;
            }
        }
    }
    if ( m_OpenGLScreen != NULL )
    {
        [ [ self openGLContext ] makeCurrentContext ];
        m_OpenGLScreen->DrawScene( m_DeltaTime );
        [ [ self openGLContext ] flushBuffer ];
    }
    return TRUE;
}


/*
 * Are we full screen?
 */
- (BOOL) isFullScreen
{
    return runningFullScreen;
}

- (BOOL)acceptsFirstResponder {
    
    return YES;
}

// accept first mouse events
// Tbis means if the window does not have focus, it will still treat the click
- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    
    NSLog(@"Got first mouse event");
    return YES;
}

- (float)GetCoordFromScreenPixel :(int)screenPixel maxPixelSize:(int)maxPixelSize ratio:(float)ratio
{
    if ( maxPixelSize > 0 )
    {
        float ret = ( screenPixel * ratio ) / maxPixelSize;
        return ret;
    }
    return 0.0f;
}

- (NSPoint) GetMousePointFromEvent :(NSEvent*)theEvent
{
    NSPoint mousePoint = theEvent.locationInWindow;
    mousePoint.x = [self GetCoordFromScreenPixel:(mousePoint.x * 2 - m_ScreenWidth) maxPixelSize:m_ScreenWidth ratio:1.0f];
    mousePoint.y = [self GetCoordFromScreenPixel:(m_ScreenHeight - mousePoint.y * 2) maxPixelSize:m_ScreenHeight ratio:1.0f];
    return mousePoint;
}

- (void)mouseDown:(NSEvent *)theEvent
{
    if ( m_GameEngine != NULL )
    {
        NSPoint mousePoint = [self GetMousePointFromEvent:theEvent];
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::LEFT_MOUSE_BUTTON, mousePoint.x, mousePoint.y );
        CoreInputManager::GetInstance()->HandleMouseButtonDown( Touch::LEFT_MOUSE_BUTTON );
    }
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
    if ( m_GameEngine != NULL )
    {
        NSPoint mousePoint = [self GetMousePointFromEvent:theEvent];
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::RIGHT_MOUSE_BUTTON, mousePoint.x, mousePoint.y );
        CoreInputManager::GetInstance()->HandleMouseButtonDown( Touch::RIGHT_MOUSE_BUTTON );
    }
}

- (void)mouseUp:(NSEvent *)theEvent
{
    if ( m_GameEngine != NULL )
    {
        NSPoint mousePoint = [self GetMousePointFromEvent:theEvent];
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::LEFT_MOUSE_BUTTON, mousePoint.x, mousePoint.y );
        CoreInputManager::GetInstance()->HandleMouseButtonUp( Touch::LEFT_MOUSE_BUTTON );
    }
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
    if ( m_GameEngine != NULL )
    {
        NSPoint mousePoint = [self GetMousePointFromEvent:theEvent];
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::RIGHT_MOUSE_BUTTON, mousePoint.x, mousePoint.y );
        CoreInputManager::GetInstance()->HandleMouseButtonUp( Touch::RIGHT_MOUSE_BUTTON );
    }
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    if ( m_GameEngine != NULL )
    {
        NSPoint mousePoint = [self GetMousePointFromEvent:theEvent];
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::LEFT_MOUSE_BUTTON, mousePoint.x, mousePoint.y );
    }
    [super mouseMoved: theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    if ( m_GameEngine != NULL )
    {
        NSPoint mousePoint = [self GetMousePointFromEvent:theEvent];
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::LEFT_MOUSE_BUTTON, mousePoint.x, mousePoint.y );
    }
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
    if ( m_GameEngine != NULL )
    {
        NSPoint mousePoint = [self GetMousePointFromEvent:theEvent];
        CoreInputManager::GetInstance()->HandleMouseMove( Touch::RIGHT_MOUSE_BUTTON, mousePoint.x, mousePoint.y );
    }
}

- (void) keyDown:(NSEvent *)theEvent
{
    unichar unicodeKey = [ [ theEvent characters ] characterAtIndex:0 ];
    if ( unicodeKey == 0x7F /* Backspace key, but OSX handles it as delete key */ )
    {
        m_GameEngine->OnKeyDown( Keyboard::KEY_BACKSPACE );
    }
    else
    {
        m_GameEngine->OnKeyDown( unicodeKey );
    }
}

@end

#endif
