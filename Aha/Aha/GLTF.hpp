//
//  GLTF.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 16/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <string>
#include <vector>
#include <map>
#include "external/variant/variant"


namespace aha
{
    class GLTF
    {
    public:
        class Value
        {
        public:
            using Binary = std::vector <unsigned char>;
            using Array = std::vector <Value>;
            using Object = std::map <std::string, Value>;
            
            enum class Type
            {
                None,
                Boolean,
                Integer,
                Number,
                String,
                Array,
                Object
            };
            
            Type type_;
            bool bool_;
            int int_;
            double number_;
            std::string string_;
            Binary binary_;
            Array array_;
            Object object_;
            mpark::variant <bool, int, double, std::string, Binary, Array, Object> value_;
        };
        
        using Boolean = bool;
        using Integer = int;
        using Number = double;
        using String = std::string;
        using Extensions = Value::Object;
        using Extras = Value;
        
        struct Asset
        {
            String version;
            String minVersion;
            String generator;
            String copyright;
            Extensions extensions;
            Extras extras;
        };
        
        struct Image
        {
            String uri;
            String mimeType;
            Integer bufferView;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct Sampler
        {
            Integer magFilter;
            Integer minFilter;
            Integer wrapS;
            Integer wrapT;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct Indices
        {
            Integer bufferView;
            Integer byteOffset;
            Integer componentType;
            Extensions extensions;
            Extras extras;
        };
        
        struct Values
        {
            Integer bufferView;
            Integer byteOffset;
            Extensions extensions;
            Extras extras;
        };
        
        struct Sparse
        {
            Integer count;
            std::vector <Indices> indices;
            std::vector <Values> values;
            Extensions extensions;
            Extras extras;
        };
        
        struct Accessor
        {
            Integer bufferView;
            Integer byteOffset;
            Integer componentType;
            Boolean normalized;
            Integer count;
            String type;
            std::vector <Number> max;
            std::vector <Number> min;
            Sparse sparse;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct CameraOrthographic
        {
            Number xmag;
            Number ymag;
            Number zfar;
            Number znear;
            Extensions extensions;
            Extras extras;
        };
        
        struct CameraPerspective
        {
            Number aspectRatio;
            Number yfov;
            Number zfar;
            Number znear;
            Extensions extensions;
            Extras extras;
        };
        
        struct Camera
        {
            CameraOrthographic orthographic;
            CameraPerspective perspective;
            String type;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct Buffer
        {
            String uri;
            Integer byteLength;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct BufferView
        {
            Integer buffer;
            Integer byteOffset;
            Integer byteLength;
            Integer byteStride;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct Skin
        {
            Integer inverseBindMatrices;
            Integer skeleton;
            std::vector <Integer> joints;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct Texture
        {
            Integer sampler;
            Integer source;
            String name;
            Extensions extensions;
            Extras extras;
        };
        
        struct TextureInfo
        {
            Integer index;
            Integer texCoord;
            Extensions extensions;
            Extras extras;
        };
        
        struct Scene
        {
            
        };
        
        struct Node
        {
            
        };
        
        struct Mesh
        {
            
        };
        
        struct Animation
        {
            
        };
        
        struct Material
        {
            
        };
        
    protected:
    };
}
