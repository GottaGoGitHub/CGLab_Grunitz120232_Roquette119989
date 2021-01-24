#version 150

in vec2 pass_Texture;
in vec3 pass_Normal, pass_Position, pass_Camera;
in mat4 pass_ViewMatrix;
out vec4 out_Color;

uniform vec3 planetColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightIntensity;
uniform vec3 camera;
uniform sampler2D texture_simp;

vec3 ambientLight = vec3(0.5, 0.5, 0.5);
vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
vec3 specularLight = vec3(1.0, 1.0, 1.0);
float shininess = 20.0f;

void main() {
  vec4 planet_texture = texture(texture_simp, pass_Texture);

  vec3 normal = normalize(pass_Normal);
  vec3 vertex_position = pass_Position;
  
  vec3 light_position = (pass_ViewMatrix * vec4(lightPos, 1.0)).xyz;
  vec3 lightVec = light_position - vertex_position;
  lightVec = normalize(lightVec);

  vec3 camera_position = (pass_ViewMatrix * vec4(pass_Camera, 1.0)).xyz;
  vec3 camVec = camera_position - vertex_position;
  camVec = normalize(camVec);

  vec3 h = normalize(lightVec + camVec);

  vec3 diffuse = max(dot(normal, lightVec), 0) * diffuseLight;
  vec3 specular = pow(max(dot(h, normal), 0), shininess) * specularLight;

  vec4 texture_color = vec4((ambientLight + diffuse) * planet_texture.rgb + specular * lightColor, 1.0);

  out_Color = vec4((ambientLight + diffuse) * planetColor * lightIntensity + specular * lightColor, 1.0);
  //out_Color = texture_color;
}
