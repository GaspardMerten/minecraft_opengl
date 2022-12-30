#version 330 core
out vec4 FragColor;
precision mediump float;

in vec4 v_col;
in vec3 v_frag_coord;
in vec3 v_normal;
in vec2 v_t;
in vec4 fragPosLightSpace;

uniform vec3 u_view_pos;
uniform sampler2D tex;

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
uniform sampler2D shadowMap;

float speculatCalculation(vec3 N, vec3 L, vec3 V) {
    vec3 R = reflect(-L, N);
    float cosTheta = dot(R, V);
    float spec = pow(max(cosTheta, 0.0), shininess);
    return light.specular_strength * spec;
}


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main() {
    vec3 N = normalize(v_normal);
    vec3 L = normalize(light.light_pos - v_frag_coord);
    vec3 V = normalize(u_view_pos - v_frag_coord);

    float specular = speculatCalculation(N, L, V);
    float diffuse = light.diffuse_strength * max(dot(N, L), 0.0);
    float distance = length(light.light_pos - v_frag_coord);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float light = light.ambient_strength + diffuse * attenuation + specular * attenuation;
    float shadow = ShadowCalculation(fragPosLightSpace);

    FragColor = (1-shadow)* texture(tex, v_t) * vec4(vec3(light), 1.0);
}