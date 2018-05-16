//
//  Layout.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 01/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Math/Vec2.hpp"


namespace aha
{
    namespace ui
    {
        class Widget;
        
        /// The different kinds of alignments a layout can perform.
        enum class Alignment
        {
            Minimum = 0, ///< Take only as much space as is required.
            Middle,      ///< Center align.
            Maximum,     ///< Take as much space as is allowed.
            Fill         ///< Fill according to preferred sizes.
        };
        
        /// The direction of data flow for a layout.
        enum class Orientation
        {
            Horizontal = 0, ///< Layout expands on horizontal axis.
            Vertical        ///< Layout expands on vertical axis.
        };
        
        class Layout
        {
        public:
            virtual ~Layout()
            {
                ;
            }
            
            virtual void performLayout(NVGcontext* context, Widget* widget) const = 0;
            virtual Vec2f getPreferredSize(NVGcontext* context, const Widget* widget) const = 0;
        };
        
        
        class BoxLayout : public Layout
        {
        public:
            BoxLayout(Orientation orientation = Orientation::Horizontal, Alignment alignment = Alignment::Middle, int margin = 0, int spacing = 0) : orientation_(orientation), alignment_(alignment), margin_(margin), spacing_(spacing)
            {
                ;
            }
            
            Orientation getOrientation() const
            {
                return orientation_;
            }
            
            void setOrientation(Orientation orientation)
            {
                orientation_ = orientation;
            }
            
            Alignment getAlignment() const
            {
                return alignment_;
            }
            
            void setAlignment(Alignment alignment)
            {
                alignment_ = alignment;
            }
            
            int getMargin() const
            {
                return margin_;
            }
            
            void setMargin(int margin)
            {
                margin_ = margin;
            }
            
            int getSpacing() const
            {
                return spacing_;
            }
            
            void setSpacing(int spacing)
            {
                spacing_ = spacing;
            }
            
            virtual Vec2f getPreferredSize(NVGcontext* context, const Widget* widget) const override;
            virtual void performLayout(NVGcontext* context, Widget* widget) const override;
            
        protected:
            Orientation orientation_;
            Alignment alignment_;
            int margin_;
            int spacing_;
        };
        
        
        class GroupLayout : public Layout
        {
        public:
            GroupLayout(int margin = 15, int spacing = 6, int groupSpacing = 14, int groupIndent = 20): margin_(margin), spacing_(spacing), groupSpacing_(groupSpacing), groupIndent_(groupIndent)
            {
                ;
            }
            
            int getMargin() const
            {
                return margin_;
            }
            
            void setMargin(int margin)
            {
                margin_ = margin;
            }
            
            int getSpacing() const
            {
                return spacing_;
            }
            
            void setSpacing(int spacing)
            {
                spacing_ = spacing;
            }
            
            int getGroupIndent() const
            {
                return groupIndent_;
            }
            
            void setGroupIndent(int groupIndent)
            {
                groupIndent_ = groupIndent;
            }
            
            int getGroupSpacing() const
            {
                return groupSpacing_;
            }
            
            void setGroupSpacing(int groupSpacing)
            {
                groupSpacing_ = groupSpacing;
            }
            
            virtual Vec2f getPreferredSize(NVGcontext* context, const Widget* widget) const override;
            virtual void performLayout(NVGcontext* context, Widget* widget) const override;
            
        protected:
            int margin_;
            int spacing_;
            int groupSpacing_;
            int groupIndent_;
        };
        
        
        class GridLayout : public Layout
        {
        public:
            GridLayout(Orientation orientation = Orientation::Horizontal, int resolution = 2, Alignment alignment = Alignment::Middle, float margin = 0.0f, float spacing = 0.0f) : orientation_(orientation), resolution_(resolution), margin_(margin)
            {
                defaultAlignment_[0] = defaultAlignment_[1] = alignment;
                spacing_ = Vec2f(spacing, spacing);
            }
            
            Orientation getOrientation() const
            {
                return orientation_;
            }
            
            void setOrientation(Orientation orientation)
            {
                orientation_ = orientation;
            }
            
            int getResolution() const
            {
                return resolution_;
            }
            
            void setResolution(int resolution)
            {
                resolution_ = resolution;
            }
            
            float getSpacing(int axis) const
            {
                return axis? spacing_.height : spacing_.width;
            }
            
            void setSpacing(int axis, float spacing)
            {
                if(axis)
                {
                    spacing_.height = spacing;
                }
                else
                {
                    spacing_.width = spacing;
                }
            }
            
            void setSpacing(float spacing)
            {
                spacing_.width = spacing_.height = spacing;
            }
            
            float getMargin() const
            {
                return margin_;
            }
            
            void setMargin(float margin)
            {
                margin_ = margin;
            }
            
            Alignment getAlignment(int axis, int item) const
            {
                if (item < (int) alignment_[axis].size())
                    return alignment_[axis][item];
                else
                    return defaultAlignment_[axis];
            }
            
            void setColAlignment(Alignment value)
            {
                defaultAlignment_[0] = value;
            }
            
            void setRowAlignment(Alignment value)
            {
                defaultAlignment_[1] = value;
            }
            
            void setColAlignment(const std::vector<Alignment>& value)
            {
                alignment_[0] = value;
            }
            
            void setRowAlignment(const std::vector<Alignment>& value)
            {
                alignment_[1] = value;
            }
            
            virtual Vec2f getPreferredSize(NVGcontext* context, const Widget* widget) const override;
            virtual void performLayout(NVGcontext* context, Widget* widget) const override;
            
        protected:
            void computeLayout(NVGcontext* context, const Widget* widget, std::vector<int>* grid) const;
            
            Orientation orientation_;
            Alignment defaultAlignment_[2];
            std::vector <Alignment> alignment_[2];
            int resolution_;
            Vec2f spacing_;
            float margin_;
        };
        
        class AdvancedGridLayout : public Layout
        {
        public:
            struct Anchor
            {
                uint8_t pos[2];
                uint8_t size[2];
                Alignment align[2];
                
                Anchor()
                {
                    pos[0] = pos[1] = (uint8_t) 0;
                    size[0] = size[1] = 1;
                    align[0] = align[1] = Alignment::Fill;
                }
                
                Anchor(int x, int y, Alignment horiz = Alignment::Fill, Alignment vert = Alignment::Fill)
                {
                    pos[0] = (uint8_t) x;
                    pos[1] = (uint8_t) y;
                    size[0] = size[1] = 1;
                    align[0] = horiz;
                    align[1] = vert;
                }
                
                Anchor(int x, int y, int w, int h, Alignment horiz = Alignment::Fill, Alignment vert = Alignment::Fill)
                {
                    pos[0] = (uint8_t) x;
                    pos[1] = (uint8_t) y;
                    size[0] = (uint8_t) w;
                    size[1] = (uint8_t) h;
                    align[0] = horiz;
                    align[1] = vert;
                }
                
                operator std::string() const
                {
                    char buf[50];
                    snprintf(buf, 50, "Format[pos=(%i, %i), size=(%i, %i), align=(%i, %i)]", pos[0], pos[1], size[0], size[1], (int) align[0], (int) align[1]);
                    return buf;
                }
            };
            
            AdvancedGridLayout(const std::vector<int>& columns = {}, const std::vector<int>& rows = {}, float margin = 0.0f) : columns_(columns), rows_(rows), margin_(margin)
            {
                 columnStretch_.resize(columns_.size(), 0);
                 rowStretch_.resize(rows_.size(), 0);
            }
            
            float getMargin() const
            {
                return margin_;
            }
            
            void setMargin(float margin)
            {
                margin_ = margin;
            }
            
            int getColumnCount() const
            {
                return (int) columns_.size();
            }
            
            int getRowCount() const
            {
                return (int) rows_.size();
            }
            
            void appendRow(int size, float stretch = 0.0f)
            {
                rows_.push_back(size);
                rowStretch_.push_back(stretch);
            }
            
            void appendColumn(int size, float stretch = 0.0f)
            {
                columns_.push_back(size);
                columnStretch_.push_back(stretch);
            };
            
            void setRowStretch(int index, float stretch)
            {
                rowStretch_.at(index) = stretch;
            }
            
            void setColumnStretch(int index, float stretch)
            {
                columnStretch_.at(index) = stretch;
            }
            
            void setAnchor(const Widget* widget, const Anchor& anchor)
            {
                anchors_[widget] = anchor;
            }
            
            Anchor anchor(const Widget* widget) const
            {
                auto it(anchors_.find(widget));
                if(it == anchors_.end())
                {
                    throw std::runtime_error("Widget was not registered with the grid layout!");
                }
                return it->second;
            }
            
            virtual Vec2f getPreferredSize(NVGcontext* context, const Widget* widget) const override;
            virtual void performLayout(NVGcontext* context, Widget* widget) const override;
            
        protected:
            void computeLayout(NVGcontext* context, const Widget* widget, std::vector<int>* grid) const;
            
            std::vector<int> columns_;
            std::vector<int> rows_;
            std::vector<float> columnStretch_;
            std::vector<float> rowStretch_;
            std::unordered_map<const Widget*, Anchor> anchors_;
            float margin_;
        };
    }
}
