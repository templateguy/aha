//
//  Aha.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "stb/stb_image.h"

#ifndef STB_TRUETYPE_IMPLEMENTATION
#   define STBTT_STATIC
#   define STB_TRUETYPE_IMPLEMENTATION
#endif
#include "stb/stb_truetype.h"

#ifndef NANOVG_GL3_IMPLEMENTATION
#   define NANOVG_GL3_IMPLEMENTATION
#endif

#include "Application.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "PBRModel.hpp"
#include "Timer.hpp"
#include "OBJModel.hpp"

// UI
#include "UI/UI.hpp"
#include "UI/UIPanel.hpp"
#include "UI/UIWindow.hpp"
#include "UI/UILabel.hpp"
#include "UI/UISlider.hpp"
#include "UI/UICheckBox.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIOpenFileDialog.hpp"
