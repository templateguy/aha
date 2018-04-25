//
//  Main.cpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 23/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#include "HelloTriangle.hpp"
#include "HelloTexture.hpp"
#include "HelloModel.hpp"
#include "HelloPBR.hpp"


void Main()
{
    aha::Application.init("Aha", 1280, 720);
    auto scene(HelloPBR::create());
    aha::Application.run(scene);
}
