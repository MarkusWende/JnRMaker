#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
flat in int instanceID;
in float tileID;

uniform sampler2D layerTiles;
uniform int numTiles;

const float offset = 1.0 / 900.0;															// for kernel post-processing (intensity)

void main()
{
    vec4 result = texture(layerTiles, TexCoords);
    if (tileID == 0.0)
        discard;
    else
        FragColor = result;										// No post-processing (comment everything else out)

    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);					// Inversion

	//FragColor = texture(screenTexture, TexCoords);										// Grayscale
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0);

	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );


    //float kernel[9] = float[](															// sharpen kernel
    //   -1, -1, -1,
    //   -1,  9, -1,
    //   -1, -1, -1
    //);

	//float kernel[9] = float[](															// blur kernel
    //1.0 / 16, 2.0 / 16, 1.0 / 16,
    //2.0 / 16, 4.0 / 16, 2.0 / 16,
    //1.0 / 16, 2.0 / 16, 1.0 / 16
	//);

	//float kernel[9] = float[](															// edge detection kernel
    //   1,  1, 1,
    //   1, -8, 1,
    //   1,  1, 1
    //);

    //vec3 sampleTex[9];
    //for(int i = 0; i < 9; i++)
    //{
    //    sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    //}
    //vec3 col = vec3(0.0);
    //for(int i = 0; i < 9; i++)
    //    col += sampleTex[i] * kernel[i];

    //FragColor = vec4(col, 1.0);
}
