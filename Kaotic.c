#define GLEW_STATIC
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "GlShader.h"

unsigned int vbo, array;

//camera data
float yaw   = 0.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float height = 3.0f;

struct vec3 cameraPos;

int firstMouse = 0;
struct vec3 cameraFront;
struct vec3 movementFront;
struct vec3 cameraRight;
struct vec3 movementRight;
struct vec3 cameraUp;

void mouse(double xpos, double ypos) {
    float xoffset = xpos;
    float yoffset = ypos;

    float sensitivity = 0.001f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

      struct vec3 front;
      struct vec3 right;
      front.x = sin(yaw);
      front.y = 1.0f;
      front.z = cos(yaw);
      movementFront = normalize(front);

      right = cross(movementFront, cameraUp);
      right.y = 1.0f;
      movementRight = normalize(right);
}

int main() {

  cameraPos = vec3(0.f, 1.f, 0.f);

  movementFront = vec3(0.0f, 0.0f, -1.0f);
  movementRight = vec3(-1.0f, 0.0f, 0.0f);
  cameraUp = vec3(0.0f, 1.0f, 0.0f);

    SDL_Init(SDL_INIT_EVERYTHING);
    //sets up window//camera data
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(0, &mode);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    SDL_Window* game = SDL_CreateWindow("Kaotic", 0, 0, mode.w, mode.h, SDL_WINDOW_MOUSE_CAPTURE|SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
    SDL_GLContext glcontext = SDL_GL_CreateContext(game);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    if (!game) {
        printf("ERROR: Window failed to initialize.\n");
        SDL_Quit();
        exit(1);
    }
    if (!glcontext)
    {
      printf("ERROR: Window context failed to initialize.\n");
      SDL_Quit();
      exit(1);
    }
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
      printf("ERROR: GLAD failed to load.\n");
      SDL_Quit();
      exit(1);
    }

    float gameWindow[] =
    {
      -4.0f, -2.0f,
      0.0f, 10.0f,
      4.0f, -2.0f
    };

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    float lastFrame = 0.0f;
    Shader("shaders/vertex.vshad", "shaders/frag.fshad");

    glGenVertexArrays(1, &array);
    glGenBuffers(1, &vbo);
    glBindVertexArray(array);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gameWindow), gameWindow, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(array);
    int i = 100;
    float x = 0.0f;
    float z = 0.0f;
    float rx = 0.0f;
    struct vec3 lPos = vec3(0.f, 4.f, 3.f);
    float firstFrame, deltaT;
    int bol = 0;
    float camSpeed = 0.0001f;
    int play = 1;

    //game loop
    while (play != 0) {
      float currentFrame = SDL_GetTicks() / 1000.f;
      glClearColor(0.0, 1.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (bol = 0)
      {
        bol = 1;
        firstFrame = SDL_GetTicks() / 1000.f;
      }

      deltaT = (currentFrame - lastFrame);
      lastFrame = currentFrame;
      printf("%f %f\n", yaw, pitch);

      SDL_Event ev;
      while (SDL_PollEvent(&ev))
      {
        switch (ev.type)
        {
          case SDL_KEYDOWN:
            if (ev.key.keysym.sym == SDLK_w) {
              cameraPos = ad(cameraPos, mult(camSpeed * deltaT, vec3(movementFront.x, 0.0f, movementFront.z)));
            }
            if (ev.key.keysym.sym == SDLK_s) {
              cameraPos = sub(cameraPos, mult(camSpeed * deltaT, vec3(movementFront.x, 0.0f, movementFront.z)));
            }
            if (ev.key.keysym.sym == SDLK_a) {
              cameraPos = ad(cameraPos, mult(camSpeed * deltaT, vec3(movementRight.x, 0.0f, movementRight.z)));
            }
            if (ev.key.keysym.sym == SDLK_d) {
              cameraPos = sub(cameraPos, mult(camSpeed * deltaT, vec3(movementRight.x, 0.0f, movementRight.z)));
            }
            if (ev.key.keysym.sym == SDLK_k) {
                i--;
            }
            if (ev.key.keysym.sym == SDLK_l) {
                i++;
            }
            if (ev.key.keysym.sym == SDLK_LEFT) {
                rx-= 0.01;
            }
            if (ev.key.keysym.sym == SDLK_RIGHT) {
                rx+= 0.01;
            }
            if (ev.key.keysym.sym == SDLK_ESCAPE) {
                play = 0;
            }
          case SDL_MOUSEMOTION:
            mouse(ev.motion.xrel, ev.motion.yrel);
        }
      }

      lPos.x = 1.f - sin(SDL_GetTicks() / 1000.f) * 5.f;
      lPos.z = cos(SDL_GetTicks() / 1000.f) * 5.f;

      setVec3("lightPos", lPos);
      setVec3("camPosition", cameraPos);

      setFloat("maximum", i);
      setFloat("mousex", yaw);
      setFloat("mousey", pitch);
      setFloat("rx", rx);

      setFloat("iResolution.x", mode.w);
      setFloat("iResolution.y", mode.h);
      setVec3("Color", vec3(1.f, 1.f, 1.f));
      glBindVertexArray(array);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      SDL_GL_SetSwapInterval(1);
      SDL_GL_SwapWindow(game);
    }

    SDL_Quit();
}
