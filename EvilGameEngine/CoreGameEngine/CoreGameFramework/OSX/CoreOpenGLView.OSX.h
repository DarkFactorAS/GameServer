//
//  CustomOpenGLView.h
//  AnimationDemo
//
//  Created by Thor Richard Hanssen on 05/07/16.
//  Copyright (c) 2016 Thor Richard Hanssen. All rights reserved.
//

#ifdef PLATFORM_OSX

#import <Cocoa/Cocoa.h>

#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

@interface CoreOpenGLView : NSOpenGLView
{
    int colorBits, depthBits;
    BOOL runningFullScreen;
    NSDictionary *originalDisplayMode;

    int                       m_ScreenWidth;
    int                       m_ScreenHeight;
    float                     m_DeltaTime;
    CoreGameEngine*           m_GameEngine;
    CoreOpenGLRender*         m_OpenGLScreen;
}

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen gameEngine:(CoreGameEngine*)gameEngine;
- (void) changeResolution:(NSRect)frame;
- (void) reshape;
- (BOOL) updateView:(NSRect)rect deltaTime:(float)deltaTime;
- (BOOL) isFullScreen;
- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame;
- (void) dealloc;
- (CoreOpenGLRender*) getRender;

- (float) GetCoordFromScreenPixel:(int)screenPixel maxPixelSize:(int)maxPixelSize ratio:(float)ratio;
- (NSPoint) GetMousePointFromEvent:(NSEvent*)theEvent;

@end
#endif
