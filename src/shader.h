#pragma once
#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

class Shader
{
  private:
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;
  unordered_map<string, unsigned int> attributes;
  unordered_map<string, int> uniforms;
  public:
  int getShaderId();
  Shader(string filepath);
  unsigned int getAttributeLocation(string attr);
  int getUniformLocation(string uni);
  void use();
  ~Shader();
};
