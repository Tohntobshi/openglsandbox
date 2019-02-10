#include <iostream>
#include <SDL2/SDL_opengles2.h>

void clearGlErrors()
{
    #ifndef __EMSCRIPTEN__
    while(glGetError())
    {
        // do nothing
    }
    #endif
}
void checkGLErrors()
{   
    #ifndef __EMSCRIPTEN__
    int err;
    while(err = glGetError())
    {
        std::cout << "Error " << std::hex << err << std::endl;
    }
    #endif
}