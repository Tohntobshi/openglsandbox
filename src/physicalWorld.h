#pragma once
#include <memory>
#include <list>
#include "physicalModel.h"

using std::shared_ptr;
using std::list;

class PhysicalWorld {
private:
  list<shared_ptr<PhysicalModel>> nonColidableModels;
  list<shared_ptr<PhysicalModel>> passiveColidableModels;
  list<shared_ptr<PhysicalModel>> activeColidableModels;
public:
  void addNonColidableModel(shared_ptr<PhysicalModel> model);
  void addPassiveColidableModel(shared_ptr<PhysicalModel> model);
  void addActiveColidableModel(shared_ptr<PhysicalModel> model);
  void update();
};