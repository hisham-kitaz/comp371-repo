#ifndef FRAGSHADERS_H
#define FRAGSHADERS_H
std::string getVertexShaderSource();
std::string getFragmentShaderSource();
std::string getDepthVertexShaderSource();
std::string getDepthFragmentShaderSource();
std::string getTextureVertexShaderSource();
std::string getTextureFragmentShaderSource();
int compileAndLinkShaders(std::string vertexShaderSource, std::string fragmentShaderSource);
GLuint loadTexture(const char* filename);
#endif
