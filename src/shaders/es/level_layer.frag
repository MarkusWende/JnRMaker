#version 300 es
precision highp float;
precision highp sampler2DArray;

in vec2 TexCoords;
in float tileID;

uniform sampler2DArray layerTiles;

out vec4 FragColor;

void main()
{
    if (tileID > 0.0)
    {
        FragColor = texture(layerTiles, vec3(TexCoords.xy, (tileID-1.0)));;
    }
    else
    {
        discard;
    }
}
