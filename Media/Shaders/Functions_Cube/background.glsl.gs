#version 450


layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


layout(push_constant) uniform globalConstants
{
    mat4    viewProjectionInverseMatrix;
};


layout (location = 0) out   vec4    fView;


void main() {
	gl_Position = vec4(-1.0f, -1.0f, 0.0f, 1.0f); fView = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = vec4(+1.0f, -1.0f, 0.0f, 1.0f); fView = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = vec4(-1.0f, +1.0f, 0.0f, 1.0f); fView = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
	gl_Position = vec4(+1.0f, +1.0f, 0.0f, 1.0f); fView = viewProjectionInverseMatrix * vec4(gl_Position.xy, 1.0f, gl_Position.w); EmitVertex();
    
    EndPrimitive();
}