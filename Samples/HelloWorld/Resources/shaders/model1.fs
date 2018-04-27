varying vec2 TexCoords;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

void main()
{
    gl_FragColor = texture2D(albedoMap, TexCoords);
}
