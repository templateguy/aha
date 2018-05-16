//
//  Main.cpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 23/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//


#include "HelloUI.hpp"
#include "HelloDemo.hpp"


void Main()
{
    aha::Application.init();
    auto scene(HelloDemo::create());
    aha::Application.run(scene);
}
