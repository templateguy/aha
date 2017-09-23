//
//  WindowOSX.m
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "WindowOSX.h"
#import "ViewController.h"


namespace aha
{
    struct WindowOSX::impl_
    {
        NSWindow* window_;
        ViewController* viewController_;
    };
    
    WindowOSX::WindowOSX() : pimpl_(std::make_unique <impl_> ())
    {
        NSRect frame = NSMakeRect(0, 0, 1024, 768);
        NSUInteger windowStyleMask = NSWindowStyleMaskTitled|NSWindowStyleMaskResizable|NSWindowStyleMaskClosable|NSWindowStyleMaskMiniaturizable;
        pimpl_->window_  = [[[NSWindow alloc] initWithContentRect : frame styleMask : windowStyleMask backing : NSBackingStoreBuffered defer : NO] retain];
        //pimpl_->viewController_ = [[ViewController alloc] init];
        //pimpl_->window_.rootViewController = pimpl_->viewController_;
        //[pimpl_->window_ addSubview : pimpl_->viewController_.view];
        [pimpl_->window_ setBackgroundColor : [NSColor whiteColor]];
        [pimpl_->window_ setTitle : @"Aha"];
        [pimpl_->window_ makeKeyAndOrderFront : pimpl_->window_];
        [pimpl_->window_ center];
    }
    
    WindowOSX::~WindowOSX()
    {
        [pimpl_->window_ release];
        pimpl_->window_ = nil;
    }
}
