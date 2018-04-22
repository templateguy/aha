//
//  AppDelegate.m
//  HelloWorldOSX
//
//  Created by Priyanshi Thakur on 18/02/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#import "AppDelegate.h"


void Main();


@implementation AppDelegate

- (void) applicationDidFinishLaunching : (NSNotification *) aNotification
{
    // Insert code here to initialize your application
    NSLog(@"Application Did Finish Launching");
    Main();
}


- (void) applicationWillTerminate : (NSNotification *) aNotification
{
    // Insert code here to tear down your application
    NSLog(@"Application Will Terminate");
}

@end
