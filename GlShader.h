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

unsigned int cShader(const char* Path, const char* mods)
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
  return ID;
}

void useShad(unsigned int id)
{
  glUseProgram(id);
}
void setInt(const char* name, int value, unsigned int id)
{
  glUniform1i(glGetUniformLocation(id, name), value);
}
void setFloat(const char* name, float value, unsigned int id)
{
  glUniform1f(glGetUniformLocation(id, name), value);
}
/*void setMat4(const char* name, glm::mat4 gorg) {
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(gorg));
}
*/
void setVec3(const char* name, vec3 value, unsigned int id)
{
  glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}
#endif
