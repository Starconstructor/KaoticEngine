#ifndef MAT_H
#define MAT_H

#include <math.h>

struct vec3
{
  float x;
  float y;
  float z;
};

struct vec3 vec3(float x, float y, float z)
{
  struct vec3 vec;
  vec.x = x;
  vec.y = y;
  vec.z = z;
  return vec;
};

float radians(float deg)
{
  float pi = 3.14159f;
  float rad = deg * pi / 180;
  return rad;
}

float degrees(float rad)
{
  float pi = 3.14159f;
  float deg = rad * 180 / pi;
}

struct vec3 normalize(struct vec3 bigAss)
{
  float length = sqrt((bigAss.x * bigAss.x) + (bigAss.y * bigAss.y) + (bigAss.z * bigAss.z));
  return vec3(bigAss.x / length, bigAss.y / length, bigAss.z / length);
}

struct vec3 cross(struct vec3 a, struct vec3 b)
{
  struct vec3 c;
  c.x = (a.y * b.z) - (a.z * b.y);
  c.y = (a.z * b.x) - (a.x * b.z);
  c.z = (a.x * b.y) - (a.y * b.x);
  return c;
}

struct vec3 mult(float f, struct vec3 vec)
{
  struct vec3 vector = vec;
  vector.x *= f;
  vector.y *= f;
  vector.z *= f;
  return vector;
}

struct vec3 ad(struct vec3 a, struct vec3 b)
{
  struct vec3 vector = a;
  vector.x += b.x;
  vector.y += b.y;
  vector.z += b.z;
  return vector;
}

struct vec3 sub(struct vec3 a, struct vec3 b)
{
  struct vec3 vector = a;
  vector.x -= b.x;
  vector.y -= b.y;
  vector.z -= b.z;
  return vector;
}

#endif
