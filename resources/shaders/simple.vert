#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Texture;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

out vec2 pass_Texture;
out vec3 pass_Normal, pass_Position, pass_Camera;
out mat4 pass_ViewMatrix;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_ViewMatrix = ViewMatrix;
	pass_Camera = (ViewMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
	pass_Position = ((ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0f)).xzy;
	pass_Texture = in_Texture;
}
