//
//  WindowIOS.mm
//  Aha
//
//  Created by Priyanshi Thakur on 22/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "WindowIOS.h"
#import "ViewController.h"


namespace aha
{
    struct WindowIOS::impl_
    {
        UIWindow* window_;
        ViewController* viewController_;
    };
    
    WindowIOS::WindowIOS() : pimpl_(std::make_unique <impl_> ())
    {
        pimpl_->window_ = [[UIWindow alloc] initWithFrame : [[UIScreen mainScreen] bounds]];
        pimpl_->viewController_ = [[ViewController alloc] init];
        pimpl_->window_.backgroundColor = [UIColor redColor];
        pimpl_->window_.rootViewController = pimpl_->viewController_;
        [pimpl_->window_ addSubview : pimpl_->viewController_.view];
        [pimpl_->window_ makeKeyAndVisible];
    }
    
    WindowIOS::~WindowIOS()
    {
        
    }
}
