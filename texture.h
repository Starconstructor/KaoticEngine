#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
  int InternalFormat;
  int ImageFormat;
  int Wrap_S;
  int Wrap_T;
  int Filter_Min;
  int Filter_Max;
  Texture(int TInternalFormat, int TImageFormat, int TWrap_S, int TWrap_T, int TFilter_Min, int TFilter_Max) {
    InternalFormat = TInternalFormat;
    ImageFormat = TImageFormat;
    Wrap_S = TWrap_S;
    Wrap_T = TWrap_T;
    Filter_Min = TFilter_Min;
    Filter_Max = TFilter_Max;
  }
  Texture() {
  }

  unsigned int Generate(const char *path) {
    int n = 4;
    int width, height;
    unsigned char* data = stbi_load(path, &width, &height, &n, 0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, ImageFormat, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      return texture;
    }
    else {
      std::cout << "Failure to extract data from image\n";
      return 1;
    }
    stbi_image_free(data);
  }
};
#endif
