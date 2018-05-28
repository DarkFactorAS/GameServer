//
//  GameViewController.h
//  AnimationDemo
//
//  Created by Thor Richard Hanssen on 05/07/16.
//  Copyright (c) 2016 Thor Richard Hanssen. All rights reserved.
//

#ifdef PLATFORM_OSX

#include "CoreOpenGLView.OSX.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"

#import <Cocoa/Cocoa.h>

@interface CoreOpenGLViewController : NSViewController
{
    IBOutlet NSWindow*        m_OpenGLWindow;
    CoreOpenGLView*           m_OpenGLView;
    NSTimer*                  m_RenderTimer;
    double                    m_LastTime;
}

- (void) initWindow:(CoreGameEngine*) gameEngine;
- (void) setupRenderTimer;
- (void) updateGLView:(NSTimer *)timer;
- (void) createFailed;
- (void) changeResolution:(int)width height:(int)height;

@end

#endif
