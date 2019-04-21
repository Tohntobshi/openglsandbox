#include "controls.h"
#include <iostream>

using std::cout;

Controls::Controls(App &app) : app(app), io(ImGui::GetIO())
{
  app.addShader("strange", "../assets/strange.glsl");
  app.addVisualModel("cube", "strange", "../assets/cube.obj", "");
  hitboxModel = app.getVisualModel("cube");
}

void Controls::processEvent(SDL_Event e)
{
  if (e.type == SDL_KEYDOWN)
  {
    switch (e.key.keysym.scancode)
    {
    case SDL_SCANCODE_A:
      apressed = true;
      break;
    case SDL_SCANCODE_D:
      dpressed = true;
      break;
    case SDL_SCANCODE_W:
      wpressed = true;
      break;
    case SDL_SCANCODE_S:
      spressed = true;
      break;
    case SDL_SCANCODE_F:
      // app.changeCameraMode();
      break;
    case SDL_SCANCODE_T:
      // app.changeCameraMode(THIRD_PERSON);
      break;
    case SDL_SCANCODE_I:
      ipressed = true;
      break;
    case SDL_SCANCODE_K:
      kpressed = true;
      break;
    case SDL_SCANCODE_J:
      jpressed = true;
      break;
    case SDL_SCANCODE_L:
      lpressed = true;
      break;
    case SDL_SCANCODE_SPACE:
      if (activeModel != nullptr)
      {
        activeModel->jump(1.0);
      }
      break;
    }
  }
  if (e.type == SDL_KEYUP)
  {
    switch (e.key.keysym.scancode)
    {
    case SDL_SCANCODE_A:
      apressed = false;
      break;
    case SDL_SCANCODE_D:
      dpressed = false;
      break;
    case SDL_SCANCODE_W:
      wpressed = false;
      break;
    case SDL_SCANCODE_S:
      spressed = false;
      break;
    case SDL_SCANCODE_I:
      ipressed = false;
      break;
    case SDL_SCANCODE_K:
      kpressed = false;
      break;
    case SDL_SCANCODE_J:
      jpressed = false;
      break;
    case SDL_SCANCODE_L:
      lpressed = false;
      break;
    }
  }
  if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
  {
    mousepressed = true;
  }
  if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
  {
    mousepressed = false;
  }
  if (e.type == SDL_MOUSEMOTION && mousepressed && !io.WantCaptureMouse)
  {
    app.rotateCameraBy(e.motion.xrel, e.motion.yrel);
  }
}

void Controls::update()
{
  if (!io.WantCaptureKeyboard)
  {
    if (apressed)
    {
      app.moveCameraSideways(-1.0);
    }
    if (dpressed)
    {
      app.moveCameraSideways(1.0);
    }
    if (wpressed)
    {
      app.moveCameraStraight(1.0f);
    }
    if (spressed)
    {
      app.moveCameraStraight(-1.0f);
    }

    if (activeModel != nullptr)
    {
      if (ipressed && lpressed)
      {
        activeModel->move(vec3(1.0f, 0.0f, 1.0f));
      }
      else if (kpressed && lpressed)
      {
        activeModel->move(vec3(-1.0f, 0.0f, 1.0f));
      }
      else if (jpressed && kpressed)
      {
        activeModel->move(vec3(-1.0f, 0.0f, -1.0f));
      }
      else if (ipressed && jpressed)
      {
        activeModel->move(vec3(1.0f, 0.0f, -1.0f));
      }
      else if (ipressed)
      {
        activeModel->move(vec3(1.0f, 0.0f, 0.0f));
      }
      else if (kpressed)
      {
        activeModel->move(vec3(-1.0f, 0.0f, 0.0f));
      }
      else if (jpressed)
      {
        activeModel->move(vec3(0.0f, 0.0f, -1.0f));
      }
      else if (lpressed)
      {
        activeModel->move(vec3(0.0f, 0.0f, 1.0f));
      }
    }
  }
  app.update();
  const vector<string> &shaderNames = app.getShaderNames();
  const vector<string> &visModelNames = app.getVisModelNames();
  const vector<string> &phModelNames = app.getPhModelNames();
  // shader window
  ImGui::Begin("Controls");
  if (ImGui::CollapsingHeader("Add shader"))
  {
    ImGui::InputText("shader path", shaderPath, sizeof(char) * 128);
    ImGui::InputText("shader name", shaderName, sizeof(char) * 128);
    if (ImGui::Button("Add shader!"))
    {
      app.addShader(shaderName, shaderPath);
    }
  }
  // visual model window
  if (ImGui::CollapsingHeader("Add visual model"))
  {
    if (ImGui::BeginCombo("shader", choosenShader))
    {
      for (const string &name : shaderNames)
      {
        bool is_selected = (name == choosenShader);
        if (ImGui::Selectable(name.c_str(), is_selected))
          strcpy(choosenShader, name.c_str());
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::InputText("model path", modelPath, sizeof(char) * 128);
    ImGui::InputText("texture path", texPath, sizeof(char) * 128);
    ImGui::InputText("model name", modelName, sizeof(char) * 128);
    if (ImGui::Button("Add visual model!"))
    {
      app.addVisualModel(modelName, choosenShader, modelPath, texPath);
    }
  }

  // physical model window
  if (ImGui::CollapsingHeader("Add physical model"))
  {
    if (ImGui::BeginCombo("vis model", choosenModel))
    {
      for (const string &name : visModelNames)
      {
        bool is_selected = (name == choosenModel);
        if (ImGui::Selectable(name.c_str(), is_selected))
          strcpy(choosenModel, name.c_str());
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::InputText("ph model name", phModelName, sizeof(char) * 128);
    ImGui::Checkbox("falling", &fallCheckbox);
    ImGui::Checkbox("collidable", &collidableCheckbox);
    if (ImGui::Button("Add physical model!"))
    {
      app.addPhysicalModel(phModelName, choosenModel, vec3(0.0f), fallCheckbox, collidableCheckbox);
    }
  }
  // choose active model
  if (ImGui::CollapsingHeader("Choose and configure active model"))
  {
    if (ImGui::BeginCombo("model", choosenPhModel))
    {
      for (const string &name : phModelNames)
      {
        bool is_selected = (name == choosenPhModel);
        if (ImGui::Selectable(name.c_str(), is_selected))
          strcpy(choosenPhModel, name.c_str());
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::Checkbox("show hitbox", &showActiveModelHitbox);
    if (ImGui::Button("Choose!"))
    {
      activeModel = app.getPhysicalModel(choosenPhModel);
    }
    // hitbox
    if (showActiveModelHitbox && activeModel != nullptr && hitboxModel != nullptr)
    {
      mat4 hitboxMat(1.0f);
      hitboxMat = glm::translate(hitboxMat, activeModel->position);
      hitboxMat = glm::scale(hitboxMat, {activeModel->width, activeModel->height, activeModel->depth});
      hitboxModel->bind();
      hitboxModel->draw(glm::value_ptr(hitboxMat));
      ImGui::SliderFloat("hitbox width", &(activeModel->width), 0.1f, 10.0f);
      ImGui::SliderFloat("hitbox height", &(activeModel->height), 0.1f, 10.0f);
      ImGui::SliderFloat("hitbox depth", &(activeModel->depth), 0.1f, 10.0f);
    }
    if (activeModel != nullptr)
    {
      ImGui::Checkbox("collidable", &(activeModel->collidable));
      ImGui::Checkbox("falling", &(activeModel->falling));
      ImGui::SliderFloat("visual scale", &(activeModel->scale), 0.1f, 10.0f);
    }
  }
  // save/load
  if (ImGui::CollapsingHeader("Save/load world"))
  {
    ImGui::InputText("file", worldPath, sizeof(char) * 128);
    if (ImGui::Button("Save!"))
    {
      app.saveConfiguration(worldPath);
    }
    if (ImGui::Button("Load!"))
    {
      app.loadConfiguration(worldPath);
    }
  }
  ImGui::End();
}