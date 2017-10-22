#version 450


layout (set = 0, binding = 0) uniform samplerCube imageEnvironment;


layout (location = 0) in  vec4 fView;


layout (location = 0) out vec4 oColor;


void main() {
    vec3    view = normalize(fView.xyz);
    vec4    dataEnvironment = textureLod(imageEnvironment, view, 0.0f);
    vec3    environment = dataEnvironment.xyz;
    
    oColor = vec4(environment, 1.0f);
}