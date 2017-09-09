//
//  AppOSX.m
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "AppOSX.h"


namespace aha
{
    class ApplicationState_
    {
    public:
        void applicationDidFinishLaunching()
        {
            NSLog(@"Application Did Finish Launching");
        }
        
        void applicationWillTerminate()
        {
            NSLog(@"Application Will Terminate");
        }
    };
}


@interface AppDelegate : NSObject <NSApplicationDelegate>
- (id) initWithApplicationState : (std::shared_ptr <aha::ApplicationState_>) applicationState;
@end

@interface AppDelegate ()
{
    std::shared_ptr <aha::ApplicationState_> applicationState_;
}
@end

@implementation AppDelegate

- (id) initWithApplicationState : (std::shared_ptr <aha::ApplicationState_>) applicationState;
{
    if(self = [super init])
    {
        applicationState_ = applicationState;
    }
    return self;
}

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
    if(applicationState_)
    {
        applicationState_->applicationDidFinishLaunching();
    }
}

- (void) applicationWillTerminate:(NSNotification *)aNotification
{
    if(applicationState_)
    {
        applicationState_->applicationWillTerminate();
    }
}

@end


namespace aha
{
    AppOSX::AppOSX() : state_(std::make_shared <ApplicationState_> ())
    {
        [NSApplication sharedApplication];
        [NSApp setDelegate : [[AppDelegate alloc] initWithApplicationState : state_]];
        [NSApp finishLaunching];
    }
    
    AppOSX::~AppOSX() = default;
    
    void AppOSX::run()
    {
        [NSApp run];
    }
}
