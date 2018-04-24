attribute vec3 aPos;
attribute vec2 aTexCoords;
attribute vec3 aNormal;

varying vec2 TexCoords;
varying vec3 WorldPos;
varying vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    vec4 temp = model * vec4(aNormal, 1.0);
    Normal = vec3(temp.x, temp.y, temp.z);
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
