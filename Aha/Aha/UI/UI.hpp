//
//  UI.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 29/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <memory>
#include "../glm/glm.hpp"
#include "../Policies/HasPosition.hpp"
#include "../Policies/HasSize.hpp"
#include "../Policies/HasParent.hpp"
#include "../Policies/HasChildren.hpp"
#include "RespondsToUIMouseEvents.hpp"


#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729

struct NVGContext;

namespace aha
{
    namespace ui
    {
        class UI : public HasPosition <glm::vec2>, public HasSize <glm::vec2>, public HasParent <UI>, public HasChildren <std::shared_ptr <UI>>
        {
        public:
            virtual void render(NVGcontext* vg) const = 0;
            
            glm::vec2 getPositionInWorldSpace() const
            {
                glm::vec2 position(position_);
                for(auto parent(getParent()); parent; parent = parent->getParent())
                {
                    position += parent->getPosition();
                }
                return position;
            }
            
            glm::vec2 getPositionInViewSpace(const glm::vec2& positionInWorldSpace) const
            {
                glm::vec2 position(positionInWorldSpace);
                position -= position_;
                for(auto parent(getParent()); parent; parent = parent->getParent())
                {
                    position -= parent->getPosition();
                }
                return position;
            }
            
            static char* cpToUTF8(int cp, char* str)
            {
                int n = 0;
                if (cp < 0x80) n = 1;
                else if (cp < 0x800) n = 2;
                else if (cp < 0x10000) n = 3;
                else if (cp < 0x200000) n = 4;
                else if (cp < 0x4000000) n = 5;
                else if (cp <= 0x7fffffff) n = 6;
                str[n] = '\0';
                switch (n)
                {
                    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
                    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
                    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
                    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
                    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
                    case 1: str[0] = cp;
                }
                return str;
            }
        };
    }
}
