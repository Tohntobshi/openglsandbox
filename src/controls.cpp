#include "controls.h"
#include <iostream>
#include <stdexcept>
#include <SDL2/SDL_opengles2.h>

using std::cout;
using std::runtime_error;

Controls::Controls(App &app) : app(app), io(ImGui::GetIO())
{
  app.addShader("hitbox", "../assets/hitbox.glsl");
  app.addVisualModel("cube", "hitbox", "../assets/cube.obj", "");
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
    case SDL_SCANCODE_U:
      upressed = true;
      break;
    case SDL_SCANCODE_O:
      opressed = true;
      break;
    case SDL_SCANCODE_Q:
      qpressed = true;
      break;
    case SDL_SCANCODE_Z:
      zpressed = true;
      break;
    case SDL_SCANCODE_COMMA:
      commapressed = true;
      break;
    case SDL_SCANCODE_PERIOD:
      dotpressed = true;
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
    case SDL_SCANCODE_U:
      upressed = false;
      break;
    case SDL_SCANCODE_O:
      opressed = false;
      break;
    case SDL_SCANCODE_Q:
      qpressed = false;
      break;
    case SDL_SCANCODE_Z:
      zpressed = false;
      break;
    case SDL_SCANCODE_COMMA:
      commapressed = false;
      break;
    case SDL_SCANCODE_PERIOD:
      dotpressed = false;
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

void Controls::passInputsToApp()
{
  if (cameraFollowsActiveModel && activeModel != nullptr)
  {
    app.moveCameraTo(activeModel->position);
    app.camera_mode = THIRD_PERSON;
  }
  if (!io.WantCaptureKeyboard)
  {
    if (apressed)
    {
      app.moveCameraSideways(-0.1f);
    }
    if (dpressed)
    {
      app.moveCameraSideways(0.1f);
    }
    if (wpressed)
    {
      app.moveCameraStraight(0.1f);
    }
    if (spressed)
    {
      app.moveCameraStraight(-0.1f);
    }
    if (qpressed)
    {
      app.moveCameraBy(vec3(0.0, 0.1, 0.0));
    }
    if (zpressed)
    {
      app.moveCameraBy(vec3(0.0, -0.1, 0.0));
    }
    if (activeModel != nullptr)
    {
      // if (ipressed && lpressed)
      // {
      //   activeModel->move(vec3(1.0f, 0.0f, 1.0f));
      // }
      // else if (kpressed && lpressed)
      // {
      //   activeModel->move(vec3(-1.0f, 0.0f, 1.0f));
      // }
      // else if (jpressed && kpressed)
      // {
      //   activeModel->move(vec3(-1.0f, 0.0f, -1.0f));
      // }
      // else if (ipressed && jpressed)
      // {
      //   activeModel->move(vec3(1.0f, 0.0f, -1.0f));
      // }
      if (commapressed)
      {
        activeModel->move(vec3(-1.0, 0.0,0.0));
      }
      if (dotpressed)
      {
        activeModel->move(vec3(1.0, 0.0,0.0));
      }
      if (ipressed)
      {
        activeModel->position += vec3(0.0f, 0.1f, 0.0f);
      }
      else if (kpressed)
      {
        activeModel->position += vec3(0.0f, -0.1f, 0.0f);
      }
      else if (jpressed)
      {
        activeModel->position += vec3(-0.1f, 0.0f, 0.0f);
      }
      else if (lpressed)
      {
        activeModel->position += vec3(0.1f, 0.0f, 0.0f);
      }
      else if (upressed)
      {
        activeModel->position += vec3(0.0f, 0.0f, 0.1f);
      }
      else if (opressed)
      {
        activeModel->position += vec3(0.0f, 0.0f, -0.1f);
      }
    }
  }
}

void Controls::drawGUI()
{
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
      try
      {
        app.addShader(shaderName, shaderPath);
        strcpy(shaderPath, "../assets/");
        strcpy(shaderName, "");
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
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
    ImGui::Checkbox("use 2d plane as model", &usePlaneObj);
    if (!usePlaneObj)
    {
      ImGui::InputText("model path", modelPath, sizeof(char) * 128);
    }
    ImGui::InputText("texture path", texPath, sizeof(char) * 128);
    ImGui::InputText("model name", modelName, sizeof(char) * 128);
    if (ImGui::Button("Add visual model!"))
    {
      try
      {
        app.addVisualModel(
          modelName,
          choosenShader,
          usePlaneObj ? "../assets/plane.obj" : modelPath,
          texPath
        );
        strcpy(modelPath, "../assets/");
        strcpy(texPath, "../assets/");
        strcpy(modelName, "");
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
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
    ImGui::Checkbox("falling", &fallCheckbox); ImGui::SameLine();
    ImGui::Checkbox("collidable", &collidableCheckbox); ImGui::SameLine();
    ImGui::Checkbox("active", &activeCheckbox);
    if (ImGui::Button("Add physical model!"))
    {
      try
      {
        app.addPhysicalModel(phModelName, choosenModel, app.camera_position, fallCheckbox, collidableCheckbox, activeCheckbox);
        strcpy(phModelName, "");
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
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
        {
          strcpy(choosenPhModel, name.c_str());
          activeModel = app.getPhysicalModel(choosenPhModel);
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::Checkbox("show hitbox", &showActiveModelHitbox);
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
      ImGui::Checkbox("collidable model", &(activeModel->collidable)); ImGui::SameLine();
      ImGui::Checkbox("falling model", &(activeModel->falling)); ImGui::SameLine();
      ImGui::Checkbox("active model", &(activeModel->active));
      ImGui::SliderFloat("visual scale", &(activeModel->scale), 0.1f, 10.0f);
      // ImGui::InputFloat("x pos", )
    }
  }
  // copy physical model
  if (ImGui::CollapsingHeader("Copy model"))
  {
    if (ImGui::BeginCombo("model to copy", choosenPhModelToCopy))
    {
      for (const string &name : phModelNames)
      {
        bool is_selected = (name == choosenPhModelToCopy);
        if (ImGui::Selectable(name.c_str(), is_selected))
        {
          strcpy(choosenPhModelToCopy, name.c_str());
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::InputText("copy model name", phModelName, sizeof(char) * 128);
    if (ImGui::Button("Copy model!") && strcmp(choosenPhModelToCopy, "") != 0)
    {
      auto modToCpy = app.getPhysicalModel(choosenPhModelToCopy);
      try
      {
        app.addPhysicalModel(
          phModelName, 
          modToCpy->visModId,
          app.camera_position,
          modToCpy->falling,
          modToCpy->collidable,
          modToCpy->active,
          modToCpy->width,
          modToCpy->height,
          modToCpy->depth,
          modToCpy->pitch,
          modToCpy->yaw,
          modToCpy->roll,
          modToCpy->scale
        );
      }
      catch (runtime_error e)
      {
        cout << e.what() << "\n";
      }
    }
  }
  // camera options
  if (ImGui::CollapsingHeader("Camera options"))
  {
    if (ImGui::RadioButton("first person", app.camera_mode == FIRST_PERSON))
    {
      app.camera_mode = FIRST_PERSON;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("third person", app.camera_mode == THIRD_PERSON))
    {
      app.camera_mode = THIRD_PERSON;
    }
    ImGui::Checkbox("follow active model", &cameraFollowsActiveModel);
    ImGui::SliderFloat("camera distance", &app.camera_distance, 1.0f, 50.0f);
    ImGui::ColorEdit4("Backround color", backGroundColor);
    if (ImGui::Button("Apply background color!"))
    {
      glClearColor(backGroundColor[0], backGroundColor[1], backGroundColor[2], backGroundColor[3]);
    }
  }
  // save/load
  if (ImGui::CollapsingHeader("Save/load world"))
  {
    ImGui::InputText("file", worldPath, sizeof(char) * 128);
    if (ImGui::Button("Save!"))
    {
      try
      {
        app.saveConfiguration(worldPath);
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
    }
    if (ImGui::Button("Load!"))
    {
      try
      {
        app.loadConfiguration(worldPath);
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
    }
  }
  ImGui::End();
}

void Controls::update()
{
  if (gameMode)
  {
    passInputsToAppInGameMode();
  }
  else
  {
    passInputsToApp();
    drawGUI();
  }
  app.update();
}

void Controls::passInputsToAppInGameMode()
{
}