//
//  UIOpenFileDialog.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 05/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Application.hpp"
#include "UIButton.hpp"
#include "UIRespondsToMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        class UIOpenFileDialog : public UIButton
        {
        public:
            UIOpenFileDialog(const std::vector <std::string>& fileTypes, bool multiple = false) : UIButton("Open", ICON_FOLDER), fileTypes_(fileTypes), multiple_(multiple)
            {
                setCallback(std::bind(&UIOpenFileDialog::onClicked_, this));
            }
            
            void setFileCallback(const std::function <void (std::vector <std::string>)>& fileCallback)
            {
                fileCallback_ = fileCallback;
            }
            
        protected:
            void onClicked_()
            {
                auto result(Application.openFileDialog(fileTypes_, multiple_));
                if(fileCallback_)
                {
                    fileCallback_(result);
                }
            }
            
            std::vector <std::string> fileTypes_{};
            bool multiple_{false};
            std::function <void (std::vector <std::string>)> fileCallback_;
        };
    }
}
