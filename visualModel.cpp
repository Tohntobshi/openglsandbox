#define STB_IMAGE_IMPLEMENTATION
#include "visualModel.h"
#include <SDL2/SDL_opengles2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stb_image.h>
#include <list>

using std::list;
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;

void clearGlErrors();
void checkGLErrors();


VisualModel::VisualModel(shared_ptr<Shader> shader, string filepath)
  : shader(shader)
{
  list<glm::vec3> verticesList;
  list<unsigned int> vertexElementsList;
  list<glm::vec3> normalsList;
  list<unsigned int> normalElementsList;
  facesCount = 0;
  ifstream file(filepath);
  if (!file.is_open())
  {
    cout << "Cannot open file " << filepath << endl;
    exit(1);
  }
  while(file)
  {
    string line;
    getline(file, line);
    
    if (line.substr(0, 2) == "v ")
    {
      float x;
      float y;
      float z;
      if (sscanf(line.c_str(), "v %f %f %f\n", &x, &y, &z) != 3)
      {
        cout << "Cannot read file " << filepath << endl;
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
        cout << "Cannot read file " << filepath << endl;
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
        cout << "Cannot read file " << filepath << endl;
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
  vector<glm::vec3> vertices = { verticesList.begin(), verticesList.end() };
  vector<glm::vec3> normals = { normalsList.begin(), normalsList.end() };
  list<unsigned int>::iterator vertexElementsIterator = vertexElementsList.begin();
  list<unsigned int>::iterator normalElementsIterator = normalElementsList.begin();
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

VisualModel::VisualModel(shared_ptr<Shader> shader, string objpath, string texpath)
  : shader(shader), textured(true)
{
  list<glm::vec3> verticesList;
  list<unsigned int> vertexElementsList;
  list<glm::vec2> texcoordsList;
  list<unsigned int> texcoordElementsList;
  list<glm::vec3> normalsList;
  list<unsigned int> normalElementsList;
  facesCount = 0;
  ifstream file(objpath);
  if (!file.is_open())
  {
    cout << "Cannot open file " << objpath << endl;
    exit(1);
  }
  while(file)
  {
    string line;
    getline(file, line);
    
    if (line.substr(0, 2) == "v ")
    {
      float x;
      float y;
      float z;
      if (sscanf(line.c_str(), "v %f %f %f\n", &x, &y, &z) != 3)
      {
        cout << "Cannot read file " << objpath << endl;
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
        cout << "Cannot read file " << objpath << endl;
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
        cout << "Cannot read file " << objpath << endl;
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
        cout << "Cannot read file " << objpath << endl;
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
  vector<glm::vec3> vertices = { verticesList.begin(), verticesList.end() };
  vector<glm::vec3> normals = { normalsList.begin(), normalsList.end() };
  vector<glm::vec2> texcoords = { texcoordsList.begin(), texcoordsList.end() };
  list<unsigned int>::iterator vertexElementsIterator = vertexElementsList.begin();
  list<unsigned int>::iterator normalElementsIterator = normalElementsList.begin();
  list<unsigned int>::iterator texcoordElementsIterator = texcoordElementsList.begin();
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
    cout << "Cannot read file " << texpath << endl;
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  stbi_image_free(image);  
}

VisualModel::~VisualModel()
{
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &texture);
}

void VisualModel::bind()
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

void VisualModel::draw(float* viewMatrix)
{
  // bind();
  int uniformLocation = shader->getUniformLocation("model");
  glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, viewMatrix);
  glDrawArrays(GL_TRIANGLES, 0, facesCount * 3);
}


// Occurence::Occurence(float x, float y, float z, float pitch, float yaw, float roll, float scale)
// {
//   model = glm::mat4(1.0f);
//   model = glm::translate(model, glm::vec3(x, y, z));
//   model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
//   model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
//   model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
//   model = glm::scale(model, glm::vec3(scale));
// }

// void Occurence::update(float x, float y, float z, float pitch, float yaw, float roll, float scale)
// {
//   model = glm::mat4(1.0f);
//   model = glm::translate(model, glm::vec3(x, y, z));
//   model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
//   model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
//   model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
//   model = glm::scale(model, glm::vec3(scale));
// }

// const float* Occurence::getMatrix() const
// {
//   return glm::value_ptr(model);
// }