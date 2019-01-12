#pragma once
#include <unordered_map>

class Shader
{
  private:
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;
  std::unordered_map<std::string, unsigned int> attributes;
  std::unordered_map<std::string, int> uniforms;
  public:
  int getShaderId();
  Shader();
  Shader(char* file);
  unsigned int getAttributeLocation(std::string attr);
  int getUniformLocation(std::string uni);
  void use();
  ~Shader();
};
