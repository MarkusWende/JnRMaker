#version 300 es
precision highp float;
precision lowp sampler2DArray;

in vec2 TexCoords;
in float tileID;

uniform sampler2DArray layerTiles;

out vec4 FragColor;

void main()
{
    vec4 result = texture(layerTiles, vec3(TexCoords.xy, tileID));
    //vec4 result = vec4(1.0, 1.0, 0.0, 1.0);
    if (tileID == -1.0)
    {
        discard;
        //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else
        FragColor = result;
}
