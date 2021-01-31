#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader {
public:
  unsigned int ID;

  Shader(std::string* vPath, std::string* fPath) {
    //finding shaders and converting the code
    std::string vCode;
    std::string fCode;
    std::ifstream vSFile;
    std::ifstream fSFile;
    const char* vPatha = (const char*)(vPath);
    const char* fPatha = (const char*)(fPath);

    vSFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fSFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
      vSFile.open(vPatha);
      fSFile.open(fPatha);
      std::stringstream vShadStream, fShadStream;

      vShadStream << vSFile.rdbuf();
      fShadStream << fSFile.rdbuf();

      vSFile.close();
      fSFile.close();

      vCode = vShadStream.str();
      fCode = fShadStream.str();
    }
    catch (std::ifstream:: failure e) {
      printf("SHADER ERROR: File failed to read\n");
    }
    const char* vShadCode = vCode.c_str();
    const char* fShadCode = fCode.c_str();

    //compiling shaders
    unsigned int vertex, frag, geo;
    int yeet;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShadCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &yeet);
    checkCompileErrors(vertex, "VERTEX");

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShadCode, NULL);
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

  }
  Shader() {
  }

  void useShad() {
    glUseProgram(ID);
  }

  void setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setMat4(const std::string &name, glm::mat4 gorg) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(gorg));
  }
  void setVec3(const std::string &name, float val1, float val2, float val3) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), val1, val2, val3);
  }
  void setiVec3(const std::string &name, int val1, int val2, int val3) {
    glUniform3i(glGetUniformLocation(ID, name.c_str()), val1, val2, val3);
  }
  void setiVec3(const std::string &name, const glm::ivec3 &value) const {
    glUniform3iv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
private:
  void checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR:SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
    else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR:PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
};
#endif
