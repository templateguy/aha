//
//  main.m
//  HelloWorldOSX
//
//  Created by Priyanshi Thakur on 23/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "../../../Aha/Aha/OSX/AppDelegate.h"


int main(int argc, const char * argv[])
{
    [NSApplication sharedApplication];
    [NSApp setDelegate : [[AppDelegate alloc] init]];
    [NSApp run];
    return 0;
}
