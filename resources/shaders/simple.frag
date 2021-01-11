#version 150

in  vec3 pass_Normal, pass_Position, pass_Camera;
in mat4 pass_ViewMatrix;
out vec4 out_Color;

uniform vec3 planetColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightIntensity;
uniform vec3 camera;

vec3 ambientLight = vec3(0.1, 0.1, 0.1);
vec3 diffuseLight = vec3(0.1, 0.1, 0.1);
vec3 specularLight = vec3(1.0, 1.0, 1.0);
float shininess = 3.0f;

void main() {
  vec3 normal = normalize(pass_Normal);
  vec3 vertex_position = pass_Position;
  
  vec3 light_position = (pass_ViewMatrix * vec4(lightPos, 1.0)).xyz;
  vec3 lightVec = light_position - vertex_position;

  vec3 lightDecay = (20.0 * lightColor) / (pow(length(lightVec), 2));

  lightVec = normalize(lightVec);

  vec3 camVec = -vertex_position;
  camVec = normalize(camVec);

  vec3 h = normalize(lightVec + camVec);

  float diffuse = max(dot(normal, lightVec), 0);
  float specular = pow(max(dot(h, normal), 0), shininess);

  vec3 ambientColor = ambientLight * planetColor;

  out_Color = vec4(ambientColor + (planetColor * diffuse + specular * specularLight) * lightDecay, 1.0); 
}
