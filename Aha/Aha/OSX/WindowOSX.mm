//
//  WindowOSX.m
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#include <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>
// Needed for _NSGetProgname
#include <crt_externs.h>
#include "WindowOSX.h"
#include "../Renderer.hpp"
#include "../Event.hpp"


static const NSRange kEmptyRange = {NSNotFound, 0};

@interface View : NSView <NSTextInputClient>
{
@private
    NSTrackingArea* trackingArea_;
    NSMutableAttributedString* markedText_;
    CVDisplayLinkRef displayLink_;
    aha::WindowOSX* window_;
    aha::Renderer* renderer_;
}
- (id) initWithWindow : (aha::WindowOSX*) window;
- (aha::Renderer*) getRenderer;
- (void) setRenderer : (aha::Renderer*) renderer;
@end

@implementation View

- (id) initWithWindow : (aha::WindowOSX *) window
{
    if(self = [super init])
    {
        window_ = window;
        renderer_ = nil;
        trackingArea_ = nil;
        markedText_ = [[NSMutableAttributedString alloc] init];
        [self updateTrackingAreas];
        [self registerForDraggedTypes : [NSArray arrayWithObjects : NSFilenamesPboardType, nil]];
    }
    return self;
}

- (void) drawView
{
    renderer_->preRender();
    renderer_->render();
    renderer_->postRender();
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    // There is no autorelease pool when this method is called
    // because it will be called from a background thread
    // It's important to create one or you will leak objects
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    [self drawView];
    
    [pool release];
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(View*) displayLinkContext getFrameForTime : outputTime];
    return result;
}

- (void) setupDisplayLink
{
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink_);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink_, &MyDisplayLinkCallback, self);
    
    // Set the display link for the current renderer
    NSOpenGLContext* context = (NSOpenGLContext*) renderer_->context();
    NSOpenGLPixelFormat* pixelFormat = (NSOpenGLPixelFormat*) renderer_->pixelFormat();
    CGLContextObj cglContext = [context CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [pixelFormat CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink_, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink_);
}

- (void) stopDisplayLink
{
    CVDisplayLinkStop(displayLink_);
}

- (aha::Renderer*) getRenderer
{
    return renderer_;
}

- (void) setRenderer : (aha::Renderer*) renderer
{
    renderer_ = renderer;
    NSOpenGLContext* context = (NSOpenGLContext*) renderer->context();
    [context setView : self];
}

- (void) dealloc
{
    [trackingArea_ release];
    [markedText_ release];
    [super dealloc];
}

- (BOOL) isOpaque
{
    return YES;
}

- (BOOL) canBecomeKeyView
{
    return YES;
}

- (BOOL) acceptsFirstResponder
{
    return YES;
}

- (void) cursorUpdate : (NSEvent*) event
{
    
}

- (void) mouseEntered : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseEntered", point.x, point.y);
}

- (void) mouseExited : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseExited", point.x, point.y);
}

- (void) mouseMoved : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseMoved", point.x, point.y);
}

- (void) mouseDown : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseLeftButtonDown", point.x, point.y);
}

- (void) mouseDragged : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseLeftButtonDragged", point.x, point.y);
}

- (void) mouseUp : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseLeftButtonUp", point.x, point.y);
}

- (void) rightMouseDown : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseRightButtonDown", point.x, point.y);
}

- (void) rightMouseDragged : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseRightButtonDragged", point.x, point.y);
}

- (void) rightMouseUp : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseRightButtonUp", point.x, point.y);
}

- (void) otherMouseDown : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseMiddleButtonDown", point.x, point.y);
}

- (void) otherMouseDragged : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseMiddleButtonDragged", point.x, point.y);
}

- (void) otherMouseUp : (NSEvent*) event
{
    CGPoint point = [event locationInWindow];
    aha::Event.fire <void (float, float)> ("MouseMiddleButtonUp", point.x, point.y);
}

- (void) scrollWheel : (NSEvent*) event
{
    double deltaX, deltaY;
    deltaX = [event scrollingDeltaX];
    deltaY = [event scrollingDeltaY];
    if([event hasPreciseScrollingDeltas])
    {
        deltaX *= 0.1;
        deltaY *= 0.1;
    }
    if(fabs(deltaX) > 0.0 || fabs(deltaY) > 0.0)
    {
        aha::Event.fire <void (float, float)> ("MouseWheelScrolled", deltaX, deltaY);
    }
}

- (void) viewDidChangeBackingProperties
{
    
}

- (void) drawRect : (NSRect) rect
{
    [self drawView];
}

- (void) updateTrackingAreas
{
    if(trackingArea_ != nil)
    {
        [self removeTrackingArea : trackingArea_];
        [trackingArea_ release];
    }
    
    const NSTrackingAreaOptions options =
    NSTrackingActiveAlways              |
    NSTrackingInVisibleRect             |
    NSTrackingMouseEnteredAndExited     |
    NSTrackingMouseMoved                |
    NSTrackingActiveInKeyWindow         |
    NSTrackingEnabledDuringMouseDrag    |
    NSTrackingCursorUpdate              |
    NSTrackingAssumeInside;
    
    trackingArea_ = [[NSTrackingArea alloc] initWithRect : [self bounds] options : options owner : self userInfo : nil];
    [self addTrackingArea : trackingArea_];
    [super updateTrackingAreas];
}

- (void) keyDown : (NSEvent*) event
{
    [self interpretKeyEvents:[NSArray arrayWithObject:event]];
    printf("Key Down...\n");
}

- (void) flagsChanged : (NSEvent*) event
{
    
}

- (void) keyUp : (NSEvent*) event
{
    printf("Key Up...\n");
}

- (NSDragOperation) draggingEntered : (id <NSDraggingInfo>) sender
{
    if((NSDragOperationGeneric & [sender draggingSourceOperationMask]) == NSDragOperationGeneric)
    {
        [self setNeedsDisplay:YES];
        return NSDragOperationGeneric;
    }
    return NSDragOperationNone;
}

- (BOOL) prepareForDragOperation : (id <NSDraggingInfo>) sender
{
    [self setNeedsDisplay : YES];
    return YES;
}

- (BOOL) performDragOperation : (id <NSDraggingInfo>) sender
{
    return YES;
}

- (void) concludeDragOperation : (id <NSDraggingInfo>) sender
{
    [self setNeedsDisplay:YES];
}

- (BOOL) hasMarkedText
{
    return [markedText_ length] > 0;
}

- (NSRange) markedRange
{
    if ([markedText_ length] > 0)
        return NSMakeRange(0, [markedText_ length] - 1);
    else
        return kEmptyRange;
}

- (NSRange) selectedRange
{
    return kEmptyRange;
}

- (void) setMarkedText : (id) string selectedRange : (NSRange) selectedRange replacementRange : (NSRange) replacementRange
{
    if ([string isKindOfClass : [NSAttributedString class]])
        [markedText_ initWithAttributedString : string];
    else
        [markedText_ initWithString : string];
}

- (void) unmarkText
{
    [[markedText_ mutableString] setString : @""];
}

- (NSArray*) validAttributesForMarkedText
{
    return [NSArray array];
}

- (NSAttributedString*) attributedSubstringForProposedRange : (NSRange) range actualRange : (NSRangePointer) actualRange
{
    return nil;
}

- (NSUInteger) characterIndexForPoint : (NSPoint) point
{
    return 0;
}

- (NSRect) firstRectForCharacterRange : (NSRange)range actualRange : (NSRangePointer) actualRange
{
    return NSMakeRect(0.0, 0.0, 0.0, 0.0);
}

- (void) insertText : (id) string replacementRange : (NSRange) replacementRange
{
    
}

- (void)doCommandBySelector:(SEL)selector
{
}
@end


namespace aha
{
    namespace internal_
    {
        // Try to figure out what the calling application is called
        //
        static NSString* findAppName(void)
        {
            size_t i;
            NSDictionary* infoDictionary = [[NSBundle mainBundle] infoDictionary];
            
            // Keys to search for as potential application names
            NSString* GLFWNameKeys[] =
            {
                @"CFBundleDisplayName",
                @"CFBundleName",
                @"CFBundleExecutable",
            };
            
            for (i = 0;  i < sizeof(GLFWNameKeys) / sizeof(GLFWNameKeys[0]);  i++)
            {
                id name = [infoDictionary objectForKey:GLFWNameKeys[i]];
                if (name &&
                    [name isKindOfClass:[NSString class]] &&
                    ![name isEqualToString:@""])
                {
                    return name;
                }
            }
            
            char** progname = _NSGetProgname();
            if (progname && *progname)
                return [NSString stringWithUTF8String:*progname];
            
            // Really shouldn't get here
            return @"Aha Application";
        }
        
        // Set up the menu bar (manually)
        // This is nasty, nasty stuff -- calls to undocumented semi-private APIs that
        // could go away at any moment, lots of stuff that really should be
        // localize(d|able), etc.  Loading a nib would save us this horror, but that
        // doesn't seem like a good thing to require of Aha users.
        //
        static void createMenuBar(void)
        {
            NSString* appName = findAppName();
            
            NSMenu* bar = [[NSMenu alloc] init];
            [NSApp setMainMenu:bar];
            
            NSMenuItem* appMenuItem =
            [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
            NSMenu* appMenu = [[NSMenu alloc] init];
            [appMenuItem setSubmenu:appMenu];
            
            [appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName]
                               action:@selector(orderFrontStandardAboutPanel:)
                        keyEquivalent:@""];
            [appMenu addItem:[NSMenuItem separatorItem]];
            NSMenu* servicesMenu = [[NSMenu alloc] init];
            [NSApp setServicesMenu:servicesMenu];
            [[appMenu addItemWithTitle:@"Services"
                                action:NULL
                         keyEquivalent:@""] setSubmenu:servicesMenu];
            [servicesMenu release];
            [appMenu addItem:[NSMenuItem separatorItem]];
            [appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", appName]
                               action:@selector(hide:)
                        keyEquivalent:@"h"];
            [[appMenu addItemWithTitle:@"Hide Others"
                                action:@selector(hideOtherApplications:)
                         keyEquivalent:@"h"]
             setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
            [appMenu addItemWithTitle:@"Show All"
                               action:@selector(unhideAllApplications:)
                        keyEquivalent:@""];
            [appMenu addItem:[NSMenuItem separatorItem]];
            [appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName]
                               action:@selector(terminate:)
                        keyEquivalent:@"q"];
            
            // Prior to Snow Leopard, we need to use this oddly-named semi-private API
            // to get the application menu working properly.
            SEL setAppleMenuSelector = NSSelectorFromString(@"setAppleMenu:");
            [NSApp performSelector:setAppleMenuSelector withObject:appMenu];
        }
    }
    
    struct WindowOSX::impl_
    {
        NSWindow* window_;
        View* view_;
        int width_;
        int height_;
    };
    
    WindowOSX::WindowOSX(const std::string& title, float width, float height, bool isFullScreen) : pimpl_(std::make_unique <impl_> ())
    {
        CGSize size([[NSScreen mainScreen] visibleFrame].size);
        pimpl_->width_ = (width = width == -1? size.width : width);
        pimpl_->height_ = (height = height == -1? size.height : height);
        NSRect frame = NSMakeRect(0, 0, width, height);
        NSUInteger windowStyleMask = NSWindowStyleMaskTitled|NSWindowStyleMaskResizable|NSWindowStyleMaskClosable|NSWindowStyleMaskMiniaturizable;
        pimpl_->window_  = [[[NSWindow alloc] initWithContentRect : frame styleMask : windowStyleMask backing : NSBackingStoreBuffered defer : NO] retain];
        [pimpl_->window_ setBackgroundColor : [NSColor whiteColor]];
        [pimpl_->window_ setTitle : [NSString stringWithCString : title.c_str() encoding : NSUTF8StringEncoding]];
        [pimpl_->window_ makeKeyAndOrderFront : pimpl_->window_];
        [pimpl_->window_ center];
        internal_::createMenuBar();
        pimpl_->view_ = [[View alloc] initWithWindow : this];
        if(pimpl_->view_)
        {
            [pimpl_->window_ setContentView : pimpl_->view_];
        }
    }
    
    WindowOSX::~WindowOSX()
    {
        stopRenderLoop();
        [pimpl_->window_ release];
        pimpl_->window_ = nil;
    }
    
    size_t WindowOSX::width() const
    {
        return pimpl_->width_;
    }
    
    void WindowOSX::width(size_t w)
    {
        
    }
    
    size_t WindowOSX::height() const
    {
        return pimpl_->height_;
    }
    
    void WindowOSX::height(size_t h)
    {
        
    }
    
    void WindowOSX::size(size_t w, size_t h)
    {
        
    }
    
    aha::Renderer* WindowOSX::renderer() const
    {
        return [pimpl_->view_ getRenderer];
    }
    
    void WindowOSX::renderer(aha::Renderer* renderer)
    {
        [pimpl_->view_ setRenderer : renderer];
    }
    
    void WindowOSX::startRenderLoop()
    {
        [pimpl_->view_ setupDisplayLink];
    }
    
    void WindowOSX::stopRenderLoop()
    {
        [pimpl_->view_ stopDisplayLink];
    }
}
