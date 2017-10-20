#version 450


layout (set = 0, binding = 0) uniform myUniformBuffer
{
    mat4 rotateMatrix;
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 modelViewProjectionMatrix;
};


layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;
layout (location = 2) in vec3 vTan;
layout (location = 3) in vec3 vBin;
layout (location = 4) in vec3 vNor;


layout (location = 0) out vec3 fPos;
layout (location = 1) out vec2 fTex;
layout (location = 2) out mat3 fTBN;


void main() {
	gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0f);
    
    fPos = (modelMatrix * vec4(vPos, 1.0f)).xyz;
    fTex = vTex;
    fTBN = mat3(
        (rotateMatrix * vec4(vTan, 1.0f)).xyz,
        (rotateMatrix * vec4(vBin, 1.0f)).xyz,
        (rotateMatrix * vec4(vNor, 1.0f)).xyz
    );
}
