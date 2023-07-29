#version 330 core

uniform vec3 lightPos;
uniform sampler2D shadowMap;

in vec3 vertexColor;
in vec3 pos;
in vec3 normal;
in vec3 eyeDir;
in vec3 lightDir;
in vec4 fragPosLightSpace;

out vec3 FragColor;

void main() {

   vec3 lightColor = vec3(1,1,1);
   float lightPower = 10.0f;

   vec3 diffuseColor = vertexColor;
   vec3 specularColor = vec3(0.7, 0.7, 0.7);
   vec3 ambientColor = vec3(0.3, 0.3, 0.3) * vertexColor;

   float dist = length(lightPos - pos);
   vec3 n = normalize(normal);
   vec3 l = normalize(lightDir);

   float cosTheta = clamp(dot(n,l), 0.0f, 1.0f);

   vec3 E = normalize(eyeDir);
   vec3 R = reflect(-l, n);

   float cosAlpha = clamp(dot(E, R), 0.0f, 1.0f);

   vec3 shadowTextureCoord = fragPosLightSpace.xyz * 0.5 + 0.5;
   float shadow = 1.0f;
   if (shadowTextureCoord.z > texture(shadowMap, shadowTextureCoord.xy).r + 0.005f) {
     shadow = 0.5;
   }

   FragColor =
     ambientColor +
     shadow * (diffuseColor * lightColor * lightPower * cosTheta / (1 + dist * 0.5 + dist * dist * 0.05) +
     specularColor * lightColor * lightPower * pow(cosAlpha, 5.0f) / (1 + dist * 0.5 + dist * dist * 0.05));
}