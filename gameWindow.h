#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <functional>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <thread>
#include <chrono>
#include <unordered_map>
#include <list>
#endif

typedef std::function<void (SDL_Event)> eventCallback;

class EventSubscription
{
private:
  unsigned int id;
  bool unsubbed;
public:
  EventSubscription(unsigned int id);
  EventSubscription();
  void unsubscribe();
};

class GameWindow
{
private:
  static unsigned int callbackCounter;
  static SDL_Window* window;
  static SDL_GLContext context;
  static std::function<void ()> funcToIterate;
  static std::unordered_map<unsigned int, eventCallback> callbacks;
  static std::list<unsigned int> callbacksToRemove;
  static void removeCallback(unsigned int id);
public:
  static EventSubscription addEventListener(eventCallback);
  static void createWindow(int width, int height);
  static void destroyWindow();
  static void startLoop(std::function<void ()> func);
  static void iterate();
  friend EventSubscription;
};
