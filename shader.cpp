#include "shader.h"
#include <SDL2/SDL_opengles2.h>
#include <iostream>

void checkGLErrors();

Shader::Shader()
{
  const char* vertexSource = R"glsl(
    attribute vec3 positionAttr;
    attribute vec3 normalAttr;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    varying vec3 color;
    void main()
    {
      vec3 norm = normalize(vec3(model * vec4(normalAttr, 0.0)));
      vec3 lightDir = normalize(vec3(-1.0, -1.0, 0.0));
      color = vec3(1.0) * (-0.5 * (dot(norm, lightDir) - 1.0));
      gl_Position = projection * view * model * vec4(positionAttr, 1.0);
    }
  )glsl";
  const char* fragmentSource = R"glsl(
    precision mediump float;
    varying vec3 color;
    void main()
    {
      gl_FragColor = vec4(color, 1.0);
    }
  )glsl";

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
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