#include "physicalWorld.h"

void PhysicalWorld::addActiveColidableModel(shared_ptr<PhysicalModel> model)
{
  activeColidableModels.push_back(model);
}

void PhysicalWorld::addPassiveColidableModel(shared_ptr<PhysicalModel> model)
{
  passiveColidableModels.push_back(model);
}

void PhysicalWorld::addNonColidableModel(shared_ptr<PhysicalModel> model)
{
  nonColidableModels.push_back(model);
}

void PhysicalWorld::update()
{
  for (auto model: activeColidableModels)
  {
    model->update();
  }
  for (auto model: passiveColidableModels)
  {
    model->update();
  }
  for (auto model: nonColidableModels)
  {
    model->update();
  }
}