//
//  Aha.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <type_traits>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <chrono>
#include <limits>
#include <cmath>

#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>

#include "File.hpp"

#include "external/stb/stb_image.h"
#include "external/stb/stb_image_write.h"
#include "external/stb/stb_truetype.h"

#include "external/json/json.hpp"

#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include "external/nanovg/nanovg.h"
#include "external/nanovg/nanovg_gl.h"

#include "external/tiny_obj_loader/tiny_obj_loader.h"
#include "external/tiny_gltf/tiny_gltf.h"
#include "external/trackball/trackball.h"

#include "Application.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "ControlledCamera.hpp"
#include "Timer.hpp"
#include "OBJModel.hpp"
#include "PBROBJModel.hpp"
#include "PBRQuad.hpp"
#include "HDRCubemap.hpp"
#include "GLTF.hpp"

// UI
#include "UI/UI.hpp"
#include "UI/UIPanel.hpp"
#include "UI/UIWindow.hpp"
#include "UI/UILabel.hpp"
#include "UI/UISlider.hpp"
#include "UI/UICheckBox.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIOpenFileDialog.hpp"
