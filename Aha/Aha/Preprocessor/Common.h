//
//  Common.h
//  Aha
//
//  Created by Priyanshi Thakur on 26/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once
#
#
#define AHA_EXPAND(x) x
#
#
#define AHA_STRINGIFY(x) AHA_STRINGIFY_I(x)
#define AHA_STRINGIFY_I(x) #x
#
#
#define AHA_CONCAT(x, y) AHA_CONCAT_I(x, y)
#define AHA_CONCAT_I(x, y) x ## y
#
#
#define AHA_IF(cond, x, y) AHA_IF_I(cond, x, y)
#define AHA_IF_I(cond, x, y) AHA_CONCAT(AHA_IF_, cond)(x, y)
#define AHA_IF_0(x, y) y
#define AHA_IF_1(x, y) x
