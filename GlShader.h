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

  void Shader(const char* vPath, const char* fPath) {
    //finding shaders and converting the code
    FILE *vShad;
    FILE *fShad;

    vShad = fopen(vPath, "r");
    fShad = fopen(fPath, "r");

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

  /*void cShader() {
    int tex_w = 512, tex_h = 512;
    GLuint tex_output;
    glGenTextures(1, &tex_output);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_output);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT,
     NULL);
    glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    printf("max global (total) work group counts x:%i y:%i z:%i\n", work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

  }*/

  void setInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
  }
  void setFloat(const char* name, float value) {
    glUniform1f(glGetUniformLocation(ID, name), value);
  }
  /*void setMat4(const char* name, glm::mat4 gorg) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(gorg));
  }
  */
  void setVec3(const char* name, struct vec3 value) {
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
  }
#endif
