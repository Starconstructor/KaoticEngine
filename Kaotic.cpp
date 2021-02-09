#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "GlShader.h"

unsigned int vbo, array;

//camera data
float yaw   = 0.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float height = 3.0f;

glm::vec3 cameraPos = glm::vec3(0.f, 1.f, 0.f);

bool firstMouse = true;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 movementFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraRight = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 movementRight = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
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

      glm::vec3 front;
      glm::vec3 right;
      front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      front.y = sin(glm::radians(pitch));
      front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      cameraFront = glm::normalize(front);
      front.x = cos(glm::radians(yaw));
      front.z = sin(glm::radians(yaw));
      movementFront = glm::normalize(front);

      right = glm::cross(cameraFront, cameraUp);
      cameraRight = glm::normalize(right);
      right.y = 0.0f;
      movementRight = glm::normalize(right);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void errormsg(int error, const char* msg) {
    printf("Error: %s\n", msg);
}

int main() {
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
    Shader mainShad = Shader("shaders/vertex.vshad", "shaders/frag.fshad");
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
    glm::vec3 lPos = glm::vec3(0.f, 4.f, 3.f);
    float firstFrame, deltaT;
    int bol = 0;
    float camSpeed = 10.f;
    glm::mat4 view = glm::mat4(1.0f);

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
        cameraPos -= camSpeed * (float)deltaT * glm::vec3(movementFront.x, 0.0f, movementFront.z);
      }
      else if (glfwGetKey(game, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos += camSpeed * (float)deltaT * glm::vec3(movementFront.x, 0.0f, movementFront.z);
      }
      else if (glfwGetKey(game, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= camSpeed * (float)deltaT * glm::vec3(movementRight.x, 0.0f, movementRight.z);
      }
      else if (glfwGetKey(game, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += camSpeed * (float)deltaT * glm::vec3(movementRight.x, 0.0f, movementRight.z);
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

      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

      lPos.x = 1.f - sin(glfwGetTime()) * 5.f;
      lPos.z = cos(glfwGetTime()) * 5.f;

      mainShad.useShad();
      mainShad.setVec3("lightPos", lPos);
      mainShad.setVec3("camPosition", cameraPos);

      mainShad.setFloat("maximum", i);
      mainShad.setFloat("mousex", yaw);
      mainShad.setFloat("mousey", -pitch);
      mainShad.setFloat("rx", rx);

      mainShad.setFloat("iResolution.x", mode->width);
      mainShad.setFloat("iResolution.y", mode->height);
      mainShad.setVec3("Color", glm::vec3(1.f, 1.f, 1.f));
      glBindVertexArray(array);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glfwSwapBuffers(game);
      glfwPollEvents();
    }

    glfwDestroyWindow(game);
    glfwTerminate();
}
