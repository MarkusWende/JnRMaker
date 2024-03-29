#version 300 es
precision lowp float;
precision lowp sampler2DArray;

in vec2 TexCoords;
in float tileID;

uniform sampler2DArray layerTiles;

out vec4 FragColor;

void main()
{
    FragColor = texture(layerTiles, vec3(vec2(TexCoords.x, TexCoords.y), tileID));
}
