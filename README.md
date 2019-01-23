# openglsandbox

my opengl sandbox

to build for web (emscripten needed):
em++ main.cpp gameWindow.cpp glErrChecks.cpp model.cpp shader.cpp world.cpp -s USE_SDL=2 -std=c++17 -Os --closure 1 -sENVIRONMENT=web -o ./dist/a.out.js --preload-file assets

to build for desktop and run: cmake . && make && ./hellogl

app is here https://my-opengl-sandbox.firebaseapp.com/
