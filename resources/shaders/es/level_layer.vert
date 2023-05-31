#version 300 es

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec2 aTileTrans;
layout (location = 3) in float aTileID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
flat out int instanceID;
out float tileID;

void main()
{
    vec2 offset = aTileTrans;
    tileID = aTileID;
    gl_Position = projection * view * model * vec4(aPos + offset, 0.0, 1.0);
    TexCoords = aTexCoords;
    instanceID = gl_InstanceID;
}
