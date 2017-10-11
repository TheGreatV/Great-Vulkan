#version 450


layout(push_constant) uniform myConstants
{
    float myConstantElement;
};


layout (set = 0, binding = 0) uniform myUniformBuffer
{
    float myElement;
};


layout (location = 0) in vec2 vPos;
layout (location = 1) in float vColorR;
layout (location = 2) in vec2 vColorGB;


layout (location = 0) out vec3 fColor;


void main() {
    // mat2 m = mat2()


	gl_Position = vec4(vPos * myElement * myConstantElement, 0.0f, 1.0f);
    fColor = vec3(vColorR, vColorGB);
}
