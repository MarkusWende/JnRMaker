#version 410 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform DirLight dirLight;
uniform vec3 viewPos;
uniform Material material;
uniform vec3 color;
uniform float alpha;

uniform sampler2D ourTexture;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

float near = 0.1; 
float far  = 5.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec4 result = CalcDirLight(dirLight, norm, viewDir);
    
    
    //vec3 ambient = texture(ourTexture, TexCoords).rgb;

    //vec3 result = ambient;
    FragColor = result;

    //FragColor = vec4(result, alpha);
    //FragColor = texture(ourTexture, TexCoords);

	//float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	//FragColor = vec4(vec3(depth), 1.0);					// show depth buffer
}


vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec4 ambient  = vec4(light.ambient  * texture(material.diffuse, TexCoords).rgb, texture(material.diffuse, TexCoords).a);
    vec4 diffuse  = vec4(light.diffuse  * diff * texture(material.diffuse, TexCoords).rgb, texture(material.diffuse, TexCoords).a);
    vec4 specular = vec4(light.specular * spec * texture(material.specular, TexCoords).rgb, texture(material.specular, TexCoords).a);
    return (ambient + diffuse + specular);
}