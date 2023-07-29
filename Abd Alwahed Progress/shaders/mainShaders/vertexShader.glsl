#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projectionMatrix = mat4(1.0f);
uniform vec3 color;
uniform vec3 lightPos; // light position in world space
uniform mat4 lightSpaceMatrix; // light space transformation matrix

out vec3 vertexColor;
out vec3 pos;
out vec3 normal;
out vec3 eyeDir;
out vec3 lightDir;
out vec4 fragPosLightSpace;

void main() {
   vertexColor = color;
   mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
   gl_Position = MVP * vec4(aPos, 1.0);

   pos = (modelMatrix * vec4(aPos, 1)).xyz;
   vec3 vertexPosition_cameraSpace = (viewMatrix * modelMatrix * vec4(aPos, 1)).xyz;
   eyeDir = vec3(0,0,0) - vertexPosition_cameraSpace;

   vec3 lightPosCamera = (viewMatrix * vec4(lightPos, 1)).xyz;
   lightDir = lightPosCamera + eyeDir;
   normal = (transpose(inverse(viewMatrix * modelMatrix)) * vec4(aNormal, 1)).xyz;

   fragPosLightSpace = lightSpaceMatrix * vec4(pos, 1.0);

}