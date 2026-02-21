#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

float shadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.0005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }

    return shadow / 9.0;
}

void main()
{
    vec3 texColor = texture(ourTexture, TexCoord).rgb;
    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float ambientStrength = 0.2;
    vec3  ambient = ambientStrength * lightColor;

    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3  viewDir          = normalize(cameraPos - FragPos);
    vec3  reflectDir       = reflect(-lightDir, norm);
    float spec             = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3  specular         = specularStrength * spec * lightColor;

    float shadow = shadowCalculation(FragPosLightSpace, norm, lightDir);

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor;
    FragColor   = vec4(result, 1.0);
}