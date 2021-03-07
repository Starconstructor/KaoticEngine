#ifndef OVERLORD_H
#define OVERLORD_H

typedef struct
{
  int Simplex;
  int raytraced;
} Material;

typedef struct
{
  vec3 color;
  vec3 pos;
  int SDF;
  float ID;
  Material mat;
} gameObject;

typedef struct
{
  int size;
  gameObject everything[];
} Overlord;

int push_back(Overlord **o, gameObject ob)
{
  size_t x = o[0] ? o[0]->size : 0, y = x + 1;

  if ((x & y) == 0)
  {
    void *temp = realloc(*o, sizeof(**o) + (x + y) * sizeof(o[0]->everything[0]));
    if (!temp) return 1;
    o[0] = temp;
  }
  gameObject obj;
  obj = ob;
  obj.ID = x;
  o[0]->everything[x] = obj;
  o[0]->size = y;
  return 0;
}

#endif
