#version 420 core
layout(binding=0) uniform sampler2D tex;
layout(binding=1) uniform sampler2D shadowMap;

out vec4 FragColor;
precision mediump float;

in vec4 v_col;
in vec3 v_frag_coord;
in vec3 v_normal;
in vec2 v_t;
in vec4 fragPosLightSpace;

uniform vec3 u_view_pos;
uniform float opacity;

uniform bool isWater;
uniform samplerCube cubemap;

uniform mat4 V;
uniform mat4 P;

struct Light {
    vec3 light_pos;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

uniform float shininess;
uniform vec3 materialColour;

float speculatCalculation(vec3 N, vec3 L, vec3 V) {
    vec3 R = reflect(-L, N);
    float cosTheta = dot(R, V);
    float spec = pow(max(cosTheta, 0.0), shininess);
    return light.specular_strength * spec;
}


float calculateShadowIntensity(vec4 shadowPos)
{
    vec3 shadowCoord = (shadowPos.xyz/shadowPos.w) * 0.5 + 0.5;
    float currentDepth = shadowCoord.z;
    float closestDepth = texture(shadowMap, shadowCoord.xy).r;

    float bias = 0.001;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, shadowCoord.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9;

    return 1-shadow/2;
}

void main() {
    vec3 N = normalize(v_normal);
    vec3 L = normalize(light.light_pos - v_frag_coord);

    float diffuse = light.diffuse_strength * max(dot(N, L), 0.0);
    float distance = length(light.light_pos - v_frag_coord);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float light = light.ambient_strength + diffuse * attenuation;

    FragColor = texture(tex, v_t) * vec4(vec3(light), 1.0);

    if (isWater) {
        vec3 u_view_pos_without_y = vec3(u_view_pos.x, 0, u_view_pos.z);
        vec3 V2 = normalize(vec3(0, 20, 0) + u_view_pos_without_y - v_frag_coord);
        vec3 reflection = reflect(-V2,N);

        FragColor = FragColor*0.5 + 0.5*vec4(texture(cubemap, reflection).rgb, 1.0);
    } else {
        float shadow = calculateShadowIntensity(fragPosLightSpace);
        FragColor = FragColor * shadow;
    }

    FragColor.a = opacity;
}

