//
//  WindowOSX.m
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#include <Cocoa/Cocoa.h>
#include "WindowOSX.h"


namespace aha
{
    struct WindowOSX::impl_
    {
        NSWindow* window_;
    };
    
    WindowOSX::WindowOSX() : pImpl_(std::make_unique <impl_> ())
    {
        NSRect frame = NSMakeRect(0, 0, 1024, 768);
        NSUInteger windowStyleMask = NSTitledWindowMask|NSResizableWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask;
        pImpl_->window_  = [[[NSWindow alloc] initWithContentRect : frame styleMask : windowStyleMask backing : NSBackingStoreBuffered defer : NO] autorelease];
        [pImpl_->window_ setBackgroundColor : [NSColor whiteColor]];
        [pImpl_->window_ setTitle : @"Aha"];
        [pImpl_->window_ makeKeyAndOrderFront : NSApp];
        [pImpl_->window_ center];
    }
    
    WindowOSX::~WindowOSX() = default;
}
