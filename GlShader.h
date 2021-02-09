#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string.h>
#include <stdio.h>


class Shader {
public:
  unsigned int ID;

  Shader(const char* vPath, const char* fPath) {
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
  }
  Shader() {
  }

  void useShad() {
    glUseProgram(ID);
  }

  void setBool(const char* name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name), (int)value);
  }
  void setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name), value);
  }
  void setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name), value);
  }
  void setMat4(const char* name, glm::mat4 gorg) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(gorg));
  }
  void setVec3(const char* name, float val1, float val2, float val3) {
    glUniform3f(glGetUniformLocation(ID, name), val1, val2, val3);
  }
  void setiVec3(const char* name, int val1, int val2, int val3) {
    glUniform3i(glGetUniformLocation(ID, name), val1, val2, val3);
  }
  void setiVec3(const char* name, const glm::ivec3 &value) const {
    glUniform3iv(glGetUniformLocation(ID, name), 1, &value[0]);
  }
  void setVec3(const char* name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]);
  }
private:
  void checkCompileErrors(unsigned int shader, const char* type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR:SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n---------------------------------------------------" << std::endl;
      }
    }
    else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR:PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n---------------------------------------------------" << std::endl;
      }
    }
  }
};
#endif
