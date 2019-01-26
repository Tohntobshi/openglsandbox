#include "shader.h"
#include <SDL2/SDL_opengles2.h>
#include <iostream>
#include <fstream>

void checkGLErrors();

Shader::Shader(std::string filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cout << "Cannot open file " << filepath << std::endl;
    exit(1);
  }
  std::string vertexSource;
  std::string fragmentSource;
  unsigned short which = 0;
  while(file)
  {
    std::string line;
    getline(file, line);
    if (line.substr(0, 9) == "#blockend")
    {
      which = 0;
      continue;
    }
    if (line.substr(0, 7) == "#vertex")
    {
      which = 1;
      continue;
    }
    if (line.substr(0, 9) == "#fragment")
    {
      which = 2;
      continue;
    }
    if (which == 1)
    {
      vertexSource += line;
    }
    if (which == 2)
    {
      fragmentSource += line;
    }
  }
  file.close();
  const char* _vertexSource = vertexSource.c_str();
  const char* _fragmentSource = fragmentSource.c_str();
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &_vertexSource, NULL);
  glCompileShader(vertexShader);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &_fragmentSource, NULL);
  glCompileShader(fragmentShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
}

Shader::~Shader()
{
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
}

void Shader::use()
{
  glUseProgram(shaderProgram);
}

unsigned int Shader::getAttributeLocation(std::string attr) {
  auto it = attributes.find(attr);
  if (it != attributes.end())
  {
    return it->second;
  }
  unsigned int location = glGetAttribLocation(shaderProgram, attr.c_str());
  glEnableVertexAttribArray(location);
  attributes.emplace(attr, location);
  return location;
}

int Shader::getUniformLocation(std::string uni) {
  auto it = uniforms.find(uni);
  if (it != uniforms.end())
  {
    return it->second;
  }
  int location = glGetUniformLocation(shaderProgram, uni.c_str());
  attributes.emplace(uni, location);
  return location;
}