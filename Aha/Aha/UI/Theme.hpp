//
//  Theme.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 01/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Math/Color.hpp"
#include "Entypo.hpp"


namespace aha
{
    namespace ui
    {
        class Theme
        {
        public:
            Theme()
            {
                standardFontSize                 = 16;
                buttonFontSize                   = 20;
                textBoxFontSize                  = 20;
                iconScale                        = 1.25f;
                
                windowCornerRadius               = 2;
                windowHeaderHeight               = 30;
                windowDropShadowSize             = 10;
                buttonCornerRadius               = 2;
                tabBorderWidth                   = 0.75f;
                tabInnerMargin                   = 5;
                tabMinButtonWidth                = 20;
                tabMaxButtonWidth                = 160;
                tabControlWidth                  = 20;
                tabButtonHorizontalPadding       = 10;
                tabButtonVerticalPadding         = 2;
                
                dropShadow                       = Color(0, 0, 0, 128);
                transparent                      = Color(0, 0, 0, 0);
                borderDark                       = Color(29, 29, 29, 255);
                borderLight                      = Color(92, 92, 92, 255);
                borderMedium                     = Color(35, 35, 35, 255);
                textColor                        = Color(255, 255, 255, 160);
                disabledTextColor                = Color(255, 255, 255, 80);
                textColorShadow                  = Color(0, 0, 0, 160);
                iconColor                        = textColor;
                
                buttonGradientTopFocused         = Color(64, 64, 64, 255);
                buttonGradientBotFocused         = Color(48, 48, 48, 255);
                buttonGradientTopUnfocused       = Color(74, 74, 74, 255);
                buttonGradientBotUnfocused       = Color(58, 58, 58, 255);
                buttonGradientTopPushed          = Color(41, 41, 41, 255);
                buttonGradientBotPushed          = Color(29, 29, 29, 255);
                
                windowFillUnfocused              = Color(43, 43, 43, 230);
                windowFillFocused                = Color(45, 45, 45, 230);
                windowTitleUnfocused             = Color(220, 220, 220, 160);
                windowTitleFocused               = Color(255, 225, 255, 190);
                
                windowHeaderGradientTop          = buttonGradientTopUnfocused;
                windowHeaderGradientBot          = buttonGradientBotUnfocused;
                windowHeaderSepTop               = borderLight;
                windowHeaderSepBot               = borderDark;
                
                windowPopup                      = Color(50, 50, 50, 255);
                windowPopupTransparent           = Color(50, 50, 50, 0);
                
                checkBoxIcon                     = ENTYPO_ICON_CHECK;
                messageInformationIcon           = ENTYPO_ICON_INFO_WITH_CIRCLE;
                messageQuestionIcon              = ENTYPO_ICON_HELP_WITH_CIRCLE;
                messageWarningIcon               = ENTYPO_ICON_WARNING;
                messageAltButtonIcon             = ENTYPO_ICON_CIRCLE_WITH_CROSS;
                messagePrimaryButtonIcon         = ENTYPO_ICON_CHECK;
                popupChevronRightIcon            = ENTYPO_ICON_CHEVRON_RIGHT;
                popupChevronLeftIcon             = ENTYPO_ICON_CHEVRON_LEFT;
                tabHeaderLeftIcon                = ENTYPO_ICON_ARROW_BOLD_LEFT;
                tabHeaderRightIcon               = ENTYPO_ICON_ARROW_BOLD_RIGHT;
                textBoxUpIcon                    = ENTYPO_ICON_CHEVRON_UP;
                textBoxDownIcon                  = ENTYPO_ICON_CHEVRON_DOWN;
                
                /*fontNormal = nvgCreateFontMem(context, "sans", roboto_regular_ttf, roboto_regular_ttf_size, 0);
                fontBold = nvgCreateFontMem(context, "sans-bold", roboto_bold_ttf, roboto_bold_ttf_size, 0);
                fontIcons = nvgCreateFontMem(context, "icons", entypo_ttf, entypo_ttf_size, 0);
                if (fontNormal == -1 || fontBold == -1 || fontIcons == -1)
                {
                    throw std::runtime_error("Could not load fonts!");
                }*/
            }
            
            int fontNormal;
            int fontBold;
            int fontIcons;
            float iconScale;
            
            int standardFontSize{16};
            int buttonFontSize{20};
            int textBoxFontSize{20};
            int windowCornerRadius{2};
            int windowHeaderHeight{30};
            int windowDropShadowSize{10};
            int buttonCornerRadius{2};
            float tabBorderWidth{0.75f};
            int tabInnerMargin{5};
            int tabMinButtonWidth{20};
            int tabMaxButtonWidth{160};
            int tabControlWidth{20};
            int tabButtonHorizontalPadding{10};
            int tabButtonVerticalPadding{2};
            
            Color dropShadow;
            Color transparent;
            Color borderDark;
            Color borderLight;
            Color borderMedium;
            Color textColor;
            Color disabledTextColor;
            Color textColorShadow;
            Color iconColor;
            
            Color buttonGradientTopFocused;
            Color buttonGradientBotFocused;
            Color buttonGradientTopUnfocused;
            Color buttonGradientBotUnfocused;
            Color buttonGradientTopPushed;
            Color buttonGradientBotPushed;
            
            Color windowFillUnfocused;
            Color windowFillFocused;
            Color windowTitleUnfocused;
            Color windowTitleFocused;
            
            Color windowHeaderGradientTop;
            Color windowHeaderGradientBot;
            Color windowHeaderSepTop;
            Color windowHeaderSepBot;
            
            Color windowPopup;
            Color windowPopupTransparent;
            
            int checkBoxIcon;
            int messageInformationIcon;
            int messageQuestionIcon;
            int messageWarningIcon;
            int messageAltButtonIcon;
            int messagePrimaryButtonIcon;
            int popupChevronRightIcon;
            int popupChevronLeftIcon;
            int tabHeaderLeftIcon;
            int tabHeaderRightIcon;
            int textBoxUpIcon;
            int textBoxDownIcon;
        };
    }
}
