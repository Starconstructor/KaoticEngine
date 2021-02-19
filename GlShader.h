#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include "mat.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

  unsigned int ID;

  void checkCompileErrors(unsigned int shader, const char* type)
  {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        printf("ERROR:SHADER_COMPILATION_ERROR of type: %s\n%s\n---------------------------------------------------\n", type, infoLog);
        exit(1);
      }
    }
    else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        printf("ERROR:SHADER_COMPILATION_ERROR of type: %s\n%s\n---------------------------------------------------\n", type, infoLog);
        exit(1);
      }
    }
  }

  void Shader(const char* vPath, const char* fPath, const char* mods) {
    //finding shaders and converting the code
    FILE *vShad;
    FILE *fShad;

    vShad = fopen(vPath, "rw");
    fShad = fopen(fPath, "rw");

    /*fseek(fShad, 0, SEEK_END);
    long fLenx = ftell(fShad);
    char* test = (char*)malloc(fLenx + 1);
    fseek(fShad, 142, SEEK_SET);
    fprintf(fShad, "%s\n", mods);*/

    fseek(vShad, 0, SEEK_END);
    fseek(fShad, 0, SEEK_END);
    long vLen = ftell(vShad);
    long fLen = ftell(fShad);
    fseek(vShad, 0, SEEK_SET);
    fseek(fShad, 0, SEEK_SET);

    char* vShadCode = (char*)malloc(vLen + 1);
    char* fShadCode = (char*)malloc(fLen + 1);

    fread(vShadCode, 1, vLen, vShad);
    fread(fShadCode, 1, fLen, fShad);
    fclose(vShad);
    fclose(fShad);

    //compiling shaders
    unsigned int vertex, frag;
    int yeet;

    vShadCode[vLen] = 0;
    fShadCode[fLen] = 0;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vCode = vShadCode;
    glShaderSource(vertex, 1, &vCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &yeet);
    checkCompileErrors(vertex, "VERTEX");

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fCode = fShadCode;
    glShaderSource(frag, 1, &fCode, NULL);
    glCompileShader(frag);
    checkCompileErrors(frag, "FRAGMENT");

    //linking shaders and setting up shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, frag);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(frag);
    free(vShadCode);
    free(fShadCode);
    glUseProgram(ID);
  }

void cShader(const char* Path, const char* mods)
{
  //finding shaders and converting the code
  FILE *Shad;

  Shad = fopen(Path, "rw");

  /*fseek(Shad, 0, SEEK_END);
  long Lenx = ftell(Shad);
  char* test = (char*)malloc(Lenx + 1);
  fseek(Shad, 142, SEEK_SET);
  fprintf(Shad, "%s\n", mods);*/

  fseek(Shad, 0, SEEK_END);
  long Len = ftell(Shad);
  fseek(Shad, 0, SEEK_SET);

  char* ShadCode = (char*)malloc(Len + 1);

  fread(ShadCode, 1, Len, Shad);
  fclose(Shad);

  //compiling shaders
  unsigned int comp;
  int yeet;

  ShadCode[Len] = 0;

  int workGrpCnt[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGrpCnt[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGrpCnt[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGrpCnt[2]);
  printf("max global (total) work group counts x:%i y:%i z:%i\n", workGrpCnt[0], workGrpCnt[1], workGrpCnt[2]);

  int workGrpSize[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGrpSize[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGrpSize[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGrpSize[2]);
  printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n", workGrpSize[0], workGrpSize[1], workGrpSize[2]);

  int workGrpInvoc;
  glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workGrpInvoc);
  printf("max local work group invocations %i\n", workGrpInvoc);

  comp = glCreateShader(GL_COMPUTE_SHADER);
  const char* Code = ShadCode;
  glShaderSource(comp, 1, &Code, NULL);
  glCompileShader(comp);
  checkCompileErrors(comp, "COMPUTE");

  //linking shaders and setting up shader program
  ID = glCreateProgram();
  glAttachShader(ID, comp);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  glDeleteShader(comp);
  free(ShadCode);
  glUseProgram(ID);
}

void setInt(const char* name, int value)
{
  glUniform1i(glGetUniformLocation(ID, name), value);
}
void setFloat(const char* name, float value)
{
  glUniform1f(glGetUniformLocation(ID, name), value);
}
/*void setMat4(const char* name, glm::mat4 gorg) {
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(gorg));
}
*/
void setVec3(const char* name, vec3 value)
{
  glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}
#endif
