#version 150

in  vec3 pass_Normal;
in mat4 pass_ViewMatrix, pass_NormalMatrix, pass_ModelMatrix;
out vec4 out_Color;

uniform vec3 planetColor;

void main() {
  out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
}
