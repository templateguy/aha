//
//  AppDelegate.mm
//  Aha
//
//  Created by Priyanshi Thakur on 23/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#import "AppDelegate.h"
#include "../Application.h"


void Main();


@interface AppDelegate ()
- (void) run_;
- (void) startRunLoop_;
@end


@implementation AppDelegate

- (void) applicationDidFinishLaunching : (NSNotification *) aNotification
{
    // Insert code here to initialize your application
    NSLog(@"Application Did Finish Launching");
    [self startRunLoop_];
    Main();
}


- (void) applicationWillTerminate : (NSNotification *) aNotification
{
    // Insert code here to tear down your application
    NSLog(@"Application Will Terminate");
}


- (void) run_
{
    Application.run();
}


- (void) startRunLoop_
{
    
}

@end
