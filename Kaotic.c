#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GlShader.h"
#include "Overlord.h"
#include "nappgui/inc/core/coreall.h"

unsigned int vbo, array;

//camera data
float yaw   = 0.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float height = 3.0f;
int firstmouse = 0;

vec3 cameraPos;

int firstMouse = 0;
vec3 cameraFront;
vec3 movementFront;
vec3 cameraRight;
vec3 movementRight;
vec3 cameraUp;

Overlord *root = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse == 0) {
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
    pitch -= yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

      vec3 front;
      vec3 right;
      front.x = sin(yaw);
      front.y = 1.0f;
      front.z = cos(yaw);
      movementFront = normalize(front);

      right = cross(movementFront, cameraUp);
      right.y = 1.0f;
      movementRight = normalize(right);
}

int main() {
  cameraPos = Vec3(0.f, 1.f, 0.f);

  movementFront = Vec3(0.0f, 0.0f, -1.0f);
  movementRight = Vec3(-1.0f, 0.0f, 0.0f);
  cameraUp = Vec3(0.0f, 1.0f, 0.0f);

  glfwInit();
  //sets up window//camera data
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  GLFWwindow* game = glfwCreateWindow(mode->width, mode->height, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL);
  glfwMakeContextCurrent(game);
  if (!game)
  {
    printf("ERROR: Window failed to initialize.\n");
    glfwTerminate();
    return -1;
  }
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    printf("ERROR: GLAD failed to load.\n");
    glfwTerminate();
    return -1;
  }
  glfwSetInputMode(game, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwMakeContextCurrent(game);
  glfwSetFramebufferSizeCallback(game, framebuffer_size_callback);
  glfwSetCursorPosCallback(game, mouse);

  float lastFrame = 0.0f;

  GLuint uTexture;
  glGenTextures(1, &uTexture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, uTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mode->width, mode->height, 0, GL_RGBA, GL_FLOAT, NULL);
  glBindImageTexture(0, uTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    unsigned int raymarched = cShader("shaders/raymarch.glsl", "");
    unsigned int raytrac = cShader("shaders/raytrace.glsl", "");
    unsigned int final = cShader("shaders/final.glsl", "");

    useShad(raymarched);
    setInt("uTexture", 0, raymarched);
    useShad(raytrac);
    setInt("uTexture", 0, raytrac);
    useShad(final);
    setInt("uTexture", 0, final);

    int i = 200;
    float x = 0.0f;
    float z = 0.0f;
    vec3 lPos = Vec3(0.f, 4.f, 3.f);
    float firstFrame, deltaT;
    int bol = 0;
    float camSpeed = 10000.f;
    int play = 1;
    unsigned int frames = 0;

    int w = 1, a = 1, s = 1, d = 1, k = 1, l = 1, rise = 1;

    gameObject ob;
    Material norm;
    Material ray;
    norm.Simplex = 0;
    norm.raytraced = 0;
    norm.roughness = 0.01f;
    ray.Simplex = 1;
    ray.raytraced = 1;
    ray.roughness = 0.01f;
    ob.color = Vec3(1.f, 1.f, 1.f);
    ob.SDF = 0;
    ob.mat = norm;
    ob.pos = Vec3(0.f, 0.f, 0.f);
    if (push_back(&root, ob)) return 1;
    else printf("Object created! ID:%i\n", root->size - 1);

    ob.color = Vec3(1.f, 1.f, 1.f);
    ob.SDF = 1;
    ob.mat = ray;
    ob.mat.Simplex = 1;
    ob.pos = lPos;
    if (push_back(&root, ob)) return 1;
    else printf("Object created! ID:%i\n", root->size - 1);

    ob.color = Vec3(0.f, 1.f, 1.f);
    ob.SDF = 1;
    ob.mat = ray;
    ob.pos = Vec3(0.f, 3.f, 6.f);
    if (push_back(&root, ob)) return 1;
    else printf("Object created! ID:%i\n", root->size - 1);

    ob.color = Vec3(1.f, 0.f, 0.f);
    ob.SDF = 2;
    ob.mat = ray;
    ob.pos = Vec3(2.f, 1.f, 6.f);
    if (push_back(&root, ob)) return 1;
    else printf("Object created! ID:%i\n", root->size - 1);

    ob.color = Vec3(1.f, 0.f, 1.f);
    ob.SDF = 3;
    ob.mat = ray;
    ob.pos = Vec3(-2.f, 1.f, 6.f);
    if (push_back(&root, ob)) return 1;
    else printf("Object created! ID:%i\n", root->size - 1);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, uTexture, 0);

    //game loop
    while (play != 0) {
      float currentFrame = glfwGetTime() / 1000.f;
      glClearColor(0.0, 1.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);

      if (bol = 0)
      {
        firstFrame = glfwGetTime() / 1000.f;
        bol = 1;
      }

      deltaT = (currentFrame - lastFrame);
      lastFrame = currentFrame;
    glfwPollEvents();

    if (glfwGetKey(game, GLFW_KEY_W) == GLFW_PRESS)
    {
      cameraPos = ad(cameraPos, mult(camSpeed * deltaT, Vec3(movementFront.x, 0.0f, movementFront.z)));
    }
    if (glfwGetKey(game, GLFW_KEY_S) == GLFW_PRESS)
    {
      cameraPos = sub(cameraPos, mult(camSpeed * deltaT, Vec3(movementFront.x, 0.0f, movementFront.z)));
    }
    if (glfwGetKey(game, GLFW_KEY_A) == GLFW_PRESS)
    {
      cameraPos = ad(cameraPos, mult(camSpeed * deltaT, Vec3(movementRight.x, 0.0f, movementRight.z)));
    }
    if (glfwGetKey(game, GLFW_KEY_D) == GLFW_PRESS)
    {
      cameraPos = sub(cameraPos, mult(camSpeed * deltaT, Vec3(movementRight.x, 0.0f, movementRight.z)));
    }
    if (glfwGetKey(game, GLFW_KEY_K) == GLFW_PRESS) {
        i--;
    }
    if (glfwGetKey(game, GLFW_KEY_L) == GLFW_PRESS) {
        i++;
    }
    if (glfwGetKey(game, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      cameraPos.y += 1.f;
    }
    if (glfwGetKey(game, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
      cameraPos.y -= 1.f;
    }
    if (glfwGetKey(game, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(game, 0);
      play = 0;
    }

    lPos.x = sin(glfwGetTime()) * 5.f;
    lPos.z = 4 + cos(glfwGetTime()) * 5.f;

    useShad(raymarched);
    for(int oo = 0; oo < root->size; oo++)
    {
      gameObject obj = root->everything[oo];
      if (obj.ID == 1)
      {
        obj.pos = lPos;
        obj.pos.y -= 3;
      }
      char str[40];
      sprintf(str, "objs[%i].color", oo);
      setVec3(str, obj.color, raymarched);
      sprintf(str, "objs[%i].pos", oo);
      setVec3(str, obj.pos, raymarched);
      sprintf(str, "objs[%i].ID", oo);
      setInt(str, obj.ID, raymarched);
      sprintf(str, "objs[%i].SDF", oo);
      setInt(str, obj.SDF, raymarched);
      sprintf(str, "objs[%i].mat.raytraced", oo);
      setInt(str, obj.mat.raytraced, raymarched);
    }

    setVec3("lightPos", lPos, raymarched);
    setVec3("camPosition", cameraPos, raymarched);

    setFloat("maximum", i, raymarched);
    setFloat("mousex", yaw, raymarched);
    setFloat("mousey", pitch, raymarched);

    setFloat("iResolution.x", mode->width, raymarched);
    setFloat("iResolution.y", mode->height, raymarched);
    setVec3("Color", Vec3(1.f, 1.f, 1.f), raymarched);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, uTexture);
    glDispatchCompute((unsigned int)mode->width/8, (unsigned int)mode->height/8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    useShad(raytrac);
    for(int oo = 0; oo < root->size; oo++)
    {
      gameObject obj = root->everything[oo];
      if (obj.ID == 1)
      {
        obj.pos = lPos;
        obj.pos.y -= 3;
      }
      char str[40];
      sprintf(str, "objs[%i].color", oo);
      setVec3(str, obj.color, raytrac);
      sprintf(str, "objs[%i].pos", oo);
      setVec3(str, obj.pos, raytrac);
      sprintf(str, "objs[%i].ID", oo);
      setInt(str, obj.ID, raytrac);
      sprintf(str, "objs[%i].SDF", oo);
      setInt(str, obj.SDF, raytrac);
      sprintf(str, "objs[%i].mat.raytraced", oo);
      setInt(str, obj.mat.raytraced, raytrac);
    }

    setVec3("lightPos", lPos, raytrac);
    setVec3("camPosition", cameraPos, raytrac);

    setFloat("mousex", yaw, raytrac);
    setFloat("mousey", pitch, raytrac);

    setFloat("iResolution.x", mode->width, raytrac);
    setFloat("iResolution.y", mode->height, raytrac);
    setVec3("Color", Vec3(1.f, 1.f, 1.f), raytrac);

    glBindTexture(GL_TEXTURE_2D, uTexture);
    glDispatchCompute((unsigned int)mode->width/8, (unsigned int)mode->height/8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    useShad(final);
    for(int oo = 0; oo < root->size; oo++)
    {
      gameObject obj = root->everything[oo];
      if (obj.ID == 1)
      {
        obj.pos = lPos;
        obj.pos.y -= 3;
      }
      char str[40];
      sprintf(str, "objs[%i].color", oo);
      setVec3(str, obj.color, final);
      sprintf(str, "objs[%i].pos", oo);
      setVec3(str, obj.pos, final);
      sprintf(str, "objs[%i].ID", oo);
      setInt(str, obj.ID, final);
      sprintf(str, "objs[%i].SDF", oo);
      setInt(str, obj.SDF, final);
      sprintf(str, "objs[%i].mat.Simplex", oo);
      setInt(str, obj.mat.Simplex, final);
      sprintf(str, "objs[%i].mat.raytraced", oo);
      setInt(str, obj.mat.raytraced, final);
      sprintf(str, "objs[%i].mat.roughness", oo);
      setFloat(str, obj.mat.roughness, final);
    }

    setVec3("lightPos", lPos, final);
    setVec3("camPosition", cameraPos, final);

    setFloat("maximum", i, final);
    setFloat("mousex", yaw, final);
    setFloat("mousey", pitch, final);

    setFloat("iResolution.x", mode->width, final);
    setFloat("iResolution.y", mode->height, final);
    setVec3("Color", Vec3(1.f, 1.f, 1.f), final);

    glBindTexture(GL_TEXTURE_2D, uTexture);
    glDispatchCompute((unsigned int)mode->width/8, (unsigned int)mode->height/8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, uTexture);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, mode->width, mode->height, 0, 0, mode->width, mode->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glfwSwapInterval(1);
    glfwSwapBuffers(game);
    //printf("%f\n", deltaT);
  }

    glfwTerminate();
}
