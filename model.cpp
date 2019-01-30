#define STB_IMAGE_IMPLEMENTATION
#include "model.h"
#include <SDL2/SDL_opengles2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stb_image.h>

void clearGlErrors();
void checkGLErrors();


Model::Model(Shader* shader, std::string filepath)
  : shader(shader)
{
  std::list<glm::vec3> verticesList;
  std::list<unsigned int> vertexElementsList;
  std::list<glm::vec3> normalsList;
  std::list<unsigned int> normalElementsList;
  facesCount = 0;
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cout << "Cannot open file " << filepath << std::endl;
    exit(1);
  }
  while(file)
  {
    std::string line;
    getline(file, line);
    
    if (line.substr(0, 2) == "v ")
    {
      float x;
      float y;
      float z;
      if (sscanf(line.c_str(), "v %f %f %f\n", &x, &y, &z) != 3)
      {
        std::cout << "Cannot read file " << filepath << std::endl;
        exit(1);
      }
      verticesList.push_back({ x, y, z });
    }
    else if (line.substr(0, 3) == "vn ")
    {
      float x;
      float y;
      float z;
      if (sscanf(line.c_str(), "vn %f %f %f\n", &x, &y, &z) != 3)
      {
        std::cout << "Cannot read file " << filepath << std::endl;
        exit(1);
      }
      normalsList.push_back({ x, y, z });
    }
    else if (line.substr(0, 2) == "f ")
    {
      facesCount++;
      unsigned int vel1, vel2, vel3, nel1, nel2, nel3;
      if (sscanf(line.c_str(), "f %d//%d %d//%d %d//%d\n", &vel1, &nel1, &vel2, &nel2, &vel3, &nel3) != 6)
      {
        std::cout << "Cannot read file " << filepath << std::endl;
        exit(1);
      }
      vertexElementsList.push_back(vel1 - 1);
      vertexElementsList.push_back(vel2 - 1);
      vertexElementsList.push_back(vel3 - 1);
      normalElementsList.push_back(nel1 - 1);
      normalElementsList.push_back(nel2 - 1);
      normalElementsList.push_back(nel3 - 1);
    }
  }
  file.close();
  std::vector<glm::vec3> vertices = { verticesList.begin(), verticesList.end() };
  std::vector<glm::vec3> normals = { normalsList.begin(), normalsList.end() };
  std::list<unsigned int>::iterator vertexElementsIterator = vertexElementsList.begin();
  std::list<unsigned int>::iterator normalElementsIterator = normalElementsList.begin();
  float* data = new float[facesCount * 3 * 2 * 3];
  for (unsigned int i = 0; i < facesCount * 3; i++)
  {
    glm::vec3& currentVertex = vertices[*vertexElementsIterator];
    glm::vec3& currentNormal = normals[*normalElementsIterator];
    data[i*6 + 0] = currentVertex.x;
    data[i*6 + 1] = currentVertex.y;
    data[i*6 + 2] = currentVertex.z;
    data[i*6 + 3] = currentNormal.x;
    data[i*6 + 4] = currentNormal.y;
    data[i*6 + 5] = currentNormal.z;
    vertexElementsIterator++;
    normalElementsIterator++;
  }
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * facesCount * 3 * 2 * 3, data, GL_STATIC_DRAW);
  delete[] data;
}

Model::Model(Shader* shader, std::string objpath, std::string texpath)
  : shader(shader), textured(true)
{
  std::list<glm::vec3> verticesList;
  std::list<unsigned int> vertexElementsList;
  std::list<glm::vec2> texcoordsList;
  std::list<unsigned int> texcoordElementsList;
  std::list<glm::vec3> normalsList;
  std::list<unsigned int> normalElementsList;
  facesCount = 0;
  std::ifstream file(objpath);
  if (!file.is_open())
  {
    std::cout << "Cannot open file " << objpath << std::endl;
    exit(1);
  }
  while(file)
  {
    std::string line;
    getline(file, line);
    
    if (line.substr(0, 2) == "v ")
    {
      float x;
      float y;
      float z;
      if (sscanf(line.c_str(), "v %f %f %f\n", &x, &y, &z) != 3)
      {
        std::cout << "Cannot read file " << objpath << std::endl;
        exit(1);
      }
      verticesList.push_back({ x, y, z });
    }
    if (line.substr(0, 3) == "vt ")
    {
      float x;
      float y;
      if (sscanf(line.c_str(), "vt %f %f\n", &x, &y) != 2)
      {
        std::cout << "Cannot read file " << objpath << std::endl;
        exit(1);
      }
      texcoordsList.push_back({ x, y });
    }
    else if (line.substr(0, 3) == "vn ")
    {
      float x;
      float y;
      float z;
      if (sscanf(line.c_str(), "vn %f %f %f\n", &x, &y, &z) != 3)
      {
        std::cout << "Cannot read file " << objpath << std::endl;
        exit(1);
      }
      normalsList.push_back({ x, y, z });
    }
    else if (line.substr(0, 2) == "f ")
    {
      facesCount++;
      unsigned int vel1, vel2, vel3, nel1, nel2, nel3, tel1, tel2, tel3;
      if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vel1, &tel1, &nel1, &vel2, &tel2, &nel2, &vel3, &tel3, &nel3) != 9)
      {
        std::cout << "Cannot read file " << objpath << std::endl;
        exit(1);
      }
      vertexElementsList.push_back(vel1 - 1);
      vertexElementsList.push_back(vel2 - 1);
      vertexElementsList.push_back(vel3 - 1);
      normalElementsList.push_back(nel1 - 1);
      normalElementsList.push_back(nel2 - 1);
      normalElementsList.push_back(nel3 - 1);
      texcoordElementsList.push_back(tel1 - 1);
      texcoordElementsList.push_back(tel2 - 1);
      texcoordElementsList.push_back(tel3 - 1);
    }
  }
  file.close();
  std::vector<glm::vec3> vertices = { verticesList.begin(), verticesList.end() };
  std::vector<glm::vec3> normals = { normalsList.begin(), normalsList.end() };
  std::vector<glm::vec2> texcoords = { texcoordsList.begin(), texcoordsList.end() };
  std::list<unsigned int>::iterator vertexElementsIterator = vertexElementsList.begin();
  std::list<unsigned int>::iterator normalElementsIterator = normalElementsList.begin();
  std::list<unsigned int>::iterator texcoordElementsIterator = texcoordElementsList.begin();
  float* data = new float[facesCount * 3 /* points*/ * (3 + 3 + 2) /* vert + norm + texcoord*/];
  for (unsigned int i = 0; i < facesCount * 3; i++)
  {
    glm::vec3& currentVertex = vertices[*vertexElementsIterator];
    glm::vec3& currentNormal = normals[*normalElementsIterator];
    glm::vec2& currentTexCoord = texcoords[*texcoordElementsIterator];
    data[i*8 + 0] = currentVertex.x;
    data[i*8 + 1] = currentVertex.y;
    data[i*8 + 2] = currentVertex.z;
    data[i*8 + 3] = currentNormal.x;
    data[i*8 + 4] = currentNormal.y;
    data[i*8 + 5] = currentNormal.z;
    data[i*8 + 6] = currentTexCoord.x;
    data[i*8 + 7] = currentTexCoord.y;
    vertexElementsIterator++;
    normalElementsIterator++;
    texcoordElementsIterator++;
  }
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * facesCount * 3 * (3 + 3 + 2), data, GL_STATIC_DRAW);
  delete[] data;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrCh;
  unsigned char* image = stbi_load(texpath.c_str(), &width, &height, &nrCh, 4);
  if (!image)
  {
    std::cout << "Cannot read file " << texpath << std::endl;
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  stbi_image_free(image);  
}

Model::~Model()
{
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &texture);
}

void Model::bind()
{
  shader->use();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindTexture(GL_TEXTURE_2D, texture);
  if (textured)
  {
    glVertexAttribPointer(shader->getAttributeLocation("positionAttr"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
    glVertexAttribPointer(shader->getAttributeLocation("normalAttr"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(shader->getAttributeLocation("texcoordAttr"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  }
  else
  {
    glVertexAttribPointer(shader->getAttributeLocation("positionAttr"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glVertexAttribPointer(shader->getAttributeLocation("normalAttr"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  }
}

void Model::draw()
{
  bind();
  int uniformLocation = shader->getUniformLocation("model");
  for (Occurence& occurence: occurences)
  {
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, occurence.getMatrix());
    glDrawArrays(GL_TRIANGLES, 0, facesCount * 3); //draw arrays
  }
}

void Model::addOccurence(float x, float y, float z, float pitch, float yaw, float roll, float scale)
{
  occurences.emplace_back(x, y, z, pitch, yaw, roll, scale);
}

void Model::clearOccurences()
{
  occurences.clear();
}

Occurence::Occurence(float x, float y, float z, float pitch, float yaw, float roll, float scale)
  // : x(x), y(y), z(z), pitch(pitch), yaw(yaw), roll(roll), scale(scale)
{
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(x, y, z));
  model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(scale));
}

const float* Occurence::getMatrix() const
{
  return glm::value_ptr(model);
}