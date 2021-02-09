#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 1;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

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
      front.x = cos(radians(yaw)) * cos(radians(pitch));
      front.y = sin(radians(pitch));
      front.z = sin(radians(yaw)) * cos(radians(pitch));
      cameraFront = normalize(front);
      front.x = cos(radians(yaw));
      front.z = sin(radians(yaw));
      movementFront = normalize(front);

      right = cross(cameraFront, cameraUp);
      cameraRight = normalize(right);
      right.y = 0.0f;
      movementRight = normalize(right);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void errormsg(int error, const char* msg) {
    printf("Error: %s\n", msg);
}

int main() {

  cameraPos = vec3(0.f, 1.f, 0.f);

  cameraFront = vec3(0.0f, 0.0f, -1.0f);
  movementFront = vec3(0.0f, 0.0f, -1.0f);
  cameraRight = vec3(-1.0f, 0.0f, 0.0f);
  movementRight = vec3(-1.0f, 0.0f, 0.0f);
  cameraUp = vec3(0.0f, 1.0f, 0.0f);

    glfwInit();
    //sets up window//camera data
    GLFWwindow* game;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_SAMPLES, 16);
    game = glfwCreateWindow(mode->width, mode->height, "Kaotic", glfwGetPrimaryMonitor(), NULL);
    glfwMakeContextCurrent(game);
    glfwSetInputMode(game, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(game, mouse_callback);

    if (!game) {
        printf("ERROR: Window failed to initialize.\n");
        glfwTerminate();
    }
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        exit(1);
    }

    glfwSetFramebufferSizeCallback(game, framebuffer_size_callback);
    glfwSetKeyCallback(game, key_callback);
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
    glfwSetErrorCallback(errormsg);

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
    float camSpeed = 10.f;

    //game loop
    while (!glfwWindowShouldClose(game)) {
      double currentFrame = glfwGetTime();
      glClearColor(0.0, 1.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (bol = 0)
      {
        bol = 1;
        firstFrame = glfwGetTime();
      }
      else
      {
        deltaT = currentFrame - firstFrame;
      }

      deltaT = (currentFrame - lastFrame);
      lastFrame = currentFrame;

      if (glfwGetKey(game, GLFW_KEY_W) == GLFW_PRESS) {
        sub(cameraPos, mult(camSpeed * (float)deltaT, vec3(movementFront.x, 0.0f, movementFront.z)));
      }
      else if (glfwGetKey(game, GLFW_KEY_S) == GLFW_PRESS) {
        ad(cameraPos, mult(camSpeed * (float)deltaT, vec3(movementFront.x, 0.0f, movementFront.z)));
      }
      else if (glfwGetKey(game, GLFW_KEY_A) == GLFW_PRESS) {
        sub(cameraPos, mult(camSpeed * (float)deltaT, vec3(movementRight.x, 0.0f, movementRight.z)));
      }
      else if (glfwGetKey(game, GLFW_KEY_D) == GLFW_PRESS) {
        ad(cameraPos, mult(camSpeed * (float)deltaT, vec3(movementRight.x, 0.0f, movementRight.z)));
      }
      if (glfwGetKey(game, GLFW_KEY_K) == GLFW_PRESS) {
          i--;
      }
      if (glfwGetKey(game, GLFW_KEY_L) == GLFW_PRESS) {
          i++;
      }
      if (glfwGetKey(game, GLFW_KEY_LEFT) == GLFW_PRESS) {
          rx-= 0.01;
      }
      if (glfwGetKey(game, GLFW_KEY_RIGHT) == GLFW_PRESS) {
          rx+= 0.01;
      }
      if (glfwGetKey(game, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
          glfwSetWindowShouldClose(game, GL_TRUE);
      }

      lPos.x = 1.f - sin(glfwGetTime()) * 5.f;
      lPos.z = cos(glfwGetTime()) * 5.f;

      setVec3("lightPos", lPos);
      setVec3("camPosition", cameraPos);

      setFloat("maximum", i);
      setFloat("mousex", yaw);
      setFloat("mousey", -pitch);
      setFloat("rx", rx);

      setFloat("iResolution.x", mode->width);
      setFloat("iResolution.y", mode->height);
      setVec3("Color", vec3(1.f, 1.f, 1.f));
      glBindVertexArray(array);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glfwSwapBuffers(game);
      glfwPollEvents();
    }

    glfwDestroyWindow(game);
    glfwTerminate();
}
