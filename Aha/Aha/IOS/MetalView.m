//
//  MetalView.m
//  Aha
//
//  Created by Priyanshi Thakur on 02/10/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import "MetalView.h"


@interface MetalView()
{
    id <MTLDevice> device_;
}
@end

@implementation MetalView

+ (id) layerClass
{
    return [CAMetalLayer class];
}

- (instancetype) initWithCoder : (NSCoder *) aDecoder
{
    if((self = [super initWithCoder : aDecoder]))
    {
        CAMetalLayer* layer = (CAMetalLayer*) [self layer];
        device_ = MTLCreateSystemDefaultDevice();
        layer.device = device_;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    }
    return self;
}

- (void) didMoveToWindow
{
    [self redraw];
}

@end
