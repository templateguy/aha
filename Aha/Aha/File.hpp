//
//  File.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 16/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <fstream>
#include <string>


namespace aha
{
    class File
    {
    public:
        File() = default;
        
        File(const std::string& fileName) : fileName_(fileName)
        {
            read();
        }
        
        void read()
        {
            if(!fileName_.empty())
            {
                std::ifstream in(fileName_, std::ios::in | std::ios::binary);
                if(in)
                {
                    in.seekg(0, std::ios::end);
                    content_.resize(in.tellg());
                    in.seekg(0, std::ios::beg);
                    in.read(&content_[0], content_.size());
                    in.close();
                }
            }
        }
        
        inline void read(const std::string& fileName)
        {
            setFileName(fileName);
            read();
        }
        
        inline const std::string& getFileName() const
        {
            return fileName_;
        }
        
        void setFileName(const std::string& fileName)
        {
            if(fileName_ != fileName)
            {
                fileName_ = fileName;
                content_.erase(content_.begin(), content_.end());
                content_.clear();
            }
        }
        
        inline const std::string& getContent() const
        {
            return content_;
        }
        
    protected:
        std::string fileName_;
        std::string content_;
    };
}
