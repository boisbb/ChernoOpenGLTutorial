#shader vertex
#version 330 core


layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_TexCoord;


//layout(location = 0) in vec3 a_Pos;
//layout(location = 1) in vec3 a_Color;
//layout(location = 2) in vec2 a_TexCoord;
//layout(location = 3) in vec3 a_Normal;

out vec3 v_Color;
out vec2 v_TexCoord;
out vec3 v_CurrentPosition;
out vec3 v_Normal;

uniform mat4 u_CameraMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_Translation;
uniform mat4 u_Rotation;
uniform mat4 u_Scale;

void main(){
  v_CurrentPosition = vec3(u_ModelMatrix * u_Translation * -u_Rotation * u_Scale * vec4(a_Pos, 1.0f));
  gl_Position = u_CameraMatrix * vec4(v_CurrentPosition, 1.0);
  v_Color = a_Color;
  v_TexCoord = mat2(0.0, -1.0, 1.0, 0.0) * a_TexCoord;
  v_Normal = a_Normal;
};

#shader fragment
#version 450 core

in vec3 v_Color;
in vec2 v_TexCoord;
in vec3 v_CurrentPosition;
in vec3 v_Normal;

out vec4 color;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

vec4 nonPointLight() {
  float ambient = 0.2f;

  vec3 normal = normalize(v_Normal);
  vec3 lightDirection = normalize(u_LightPosition - v_CurrentPosition);
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(u_CameraPosition - v_CurrentPosition);
  vec3 reflectionDirection = reflect(-lightDirection, normal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * specularLight;
  
  return (texture(diffuse0, v_TexCoord) * (diffuse + ambient) + texture(specular0, v_TexCoord).r * specular) * u_LightColor;
}

vec4 pointLight() {
  vec3 lightVec = u_LightPosition - v_CurrentPosition;
  float dist = length(lightVec);

  // constant that controlls how fast the intensity dies out
  float a = 1.0;

  // constant that controlls how far the light reaches
  float b = 0.04;

  float ambient = 0.2f;

  float inten = 1.0f / (a * dist + b * dist + 1.0f);

  vec3 normal = normalize(v_Normal);
  vec3 lightDirection = normalize(lightVec);
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(u_CameraPosition - v_CurrentPosition);
  vec3 reflectionDirection = reflect(-lightDirection, normal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * specularLight;
  
  return (texture(diffuse0, v_TexCoord) * (diffuse * inten + ambient) + texture(specular0, v_TexCoord).r * specular * inten) * u_LightColor;
}

vec4 directLight() {
  float ambient = 0.2f;
  vec3 normal = normalize(v_Normal);
  vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(u_CameraPosition - v_CurrentPosition);
  vec3 reflectionDirection = reflect(-lightDirection, normal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * specularLight;
  
  return (texture(diffuse0, v_TexCoord) * (diffuse + ambient) + texture(specular0, v_TexCoord).r * specular) * u_LightColor;
}

vec4 spotLight() {
  float outerCone = 0.90f;
  float innerCone = 0.95f;

  float ambient = 0.2f;

  vec3 normal = normalize(v_Normal);
  vec3 lightDirection = normalize(u_LightPosition - v_CurrentPosition);
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(u_CameraPosition - v_CurrentPosition);
  vec3 reflectionDirection = reflect(-lightDirection, normal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * specularLight;

  float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
  float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
  
  return (texture(diffuse0, v_TexCoord) * (diffuse * inten + ambient) + texture(specular0, v_TexCoord).r * specular * inten) * u_LightColor;
}

void main(){
  

  color = spotLight();
  //color = texture(diffuse0, v_TexCoord) * u_LightColor * (diffuse + ambient + specular);
  color[3] = 1.0f;
};
