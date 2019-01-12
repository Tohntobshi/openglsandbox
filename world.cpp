#include "world.h"
#include <SDL2/SDL_opengles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


World::World()
{ 
}

World::~World()
{
  for(Model* object: objects)
  {
    delete object;
  }
  for(Shader* shader: shaders)
  {
    delete shader;
  }
}

void World::addModel(Model* model)
{
  objects.emplace_back(model);
}

void World::addShader(Shader* shader)
{
  shaders.emplace_back(shader);
}

void World::draw()
{
  for(Model* object: objects)
  {
    object->draw();
  }
}
