#version 330 core

in vec2 TexCoords;
in float tileID;

uniform sampler2DArray layerTiles;

out vec4 FragColor;

void main()
{
    // vec4 result;
    // if (tileID > 0.0)
    //     result = texture(layerTiles, vec3(TexCoords.xy, tileID));
    // else
    //     result = vec4(255, 255, 0, 100);

    FragColor = texture(layerTiles, vec3(TexCoords.xy, tileID));
}
