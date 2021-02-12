#ifndef MAT_H
#define MAT_H

#include <math.h>

typedef struct
{
  float x;
  float y;
  float z;
} vec3;

vec3 Vec3(float x, float y, float z)
{
  vec3 vec;
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

vec3 normalize(vec3 bigAss)
{
  float length = sqrt((bigAss.x * bigAss.x) + (bigAss.y * bigAss.y) + (bigAss.z * bigAss.z));
  return Vec3(bigAss.x / length, bigAss.y / length, bigAss.z / length);
}

vec3 cross(vec3 a, vec3 b)
{
  vec3 c;
  c.x = (a.y * b.z) - (a.z * b.y);
  c.y = (a.z * b.x) - (a.x * b.z);
  c.z = (a.x * b.y) - (a.y * b.x);
  return c;
}

vec3 mult(float f, vec3 vec)
{
  vec3 vector = vec;
  vector.x *= f;
  vector.y *= f;
  vector.z *= f;
  return vector;
}

vec3 ad(vec3 a, vec3 b)
{
  vec3 vector = a;
  vector.x += b.x;
  vector.y += b.y;
  vector.z += b.z;
  return vector;
}

vec3 sub(vec3 a, vec3 b)
{
  vec3 vector = a;
  vector.x -= b.x;
  vector.y -= b.y;
  vector.z -= b.z;
  return vector;
}

#endif
