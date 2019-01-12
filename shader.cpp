#include "shader.h"
#include <SDL2/SDL_opengles2.h>


Shader::Shader()
{
  const char* vertexSource = R"glsl(
    attribute vec2 position;
    // attribute vec3 color;
    // varying vec3 Color;
    // uniform mat4 viewProj;
    // uniform mat4 model;
    void main()
    {
      // Color = color;
      // gl_Position = viewProj * model * vec4(position, 0.0, 1.0);
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )glsl";
  const char* fragmentSource = R"glsl(
    precision mediump float;
    // varying vec3 Color;
    void main()
    {
      // gl_FragColor = vec4(Color, 1.0);
      gl_FragColor = vec4(1.0,1.0,1.0,1.0);
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