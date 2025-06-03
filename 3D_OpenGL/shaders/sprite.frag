#version 330 core

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTex;
    sampler2D specularTex;
};

struct PointLight
{
    vec3 position;
    float intensity;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

in vec2 vs_texcoord;
in vec3 vs_normal;
in vec3 vs_position;
in vec3 vs_color;

out vec4 color;

// Uniforms
uniform Material material;
uniform PointLight pointLight;

uniform vec3 spriteColor;

uniform vec3 lightPos0;
uniform vec3 camPosition;

// Functions
vec3 calculateAmbient(Material material);
vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 lightPos0);
vec3 calculateSpecular(Material material, vec3 vs_position, vec3 lightPos0, vec3 camPosition);

void main()
{    
    // Ambient Light
    vec3 ambientFinal = calculateAmbient(material);

    // Diffused Light
    vec3 diffuseFinal = calculateDiffuse(material, vs_position, lightPos0);

    // Specular Light
    vec3 specularFinal = calculateSpecular(material, vs_position, lightPos0, camPosition);

    // Attenuation
    float distance = length(lightPos0 - vs_position);
    //                              constant            linear      quadratic
    float attenuation = 1.f / (1.f + 0.045f * distance + 0.0075f * (distance * distance));

    //Final light
    ambientFinal *= attenuation;
    diffuseFinal *= attenuation;
    specularFinal *= attenuation;
    color = texture(material.diffuseTex, vs_texcoord) * vec4(vs_color, 1.0)
            * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
} 

vec3 calculateAmbient(Material material)
{
    return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 lightPos0)
{
    vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
    vec3 diffuseColor = vec3(1.f);
    float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
    return material.diffuse * diffuse;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 lightPos0, vec3 camPosition)
{
    vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
    vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
    vec3 posToViewDirVec = normalize(camPosition - vs_position);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 30);
    return material.specular * specularConstant * texture(material.specularTex, vs_texcoord).rgb;
}