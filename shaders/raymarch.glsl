#version 430 core
layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba32f, binding = 0) uniform image2D uTexture;

ivec2 FragCoord = ivec2(gl_GlobalInvocationID.xy);

vec4 color;
uniform vec3 Color;
uniform vec3 lightPos, camPosition;
uniform float maximum;
uniform float mousex, mousey;

#define SIZE 5
#define MAXDIST 100

vec3 normie;
int id;
int tID;
vec3 albedo;
vec3 fragPos;
float roughness;

int scened = 0;

struct material
{
  int raytraced;
};

struct gameObject
{
  vec3 color;
  vec3 pos;
  int SDF;
  int ID;
  material mat;
};

uniform gameObject objs[SIZE];

uniform struct {
  float x;
  float y;
} iResolution;

float cubSDF (vec3 p)
{
  return max(abs(p.x), max(abs(p.y), abs(p.z))) - 0.5;
}

float spherSDF (vec3 p)
{
  return length(p) - 1 * 1;
}

float floorSDF (vec3 p)
{
  return p.y;
}

float fractal (vec3 p)
{
  return length(p) - 1 * 1;
}

float SDF(gameObject thing, vec3 pos1, vec3 pos2)
{
  if (thing.SDF == 0)
  {
    return floorSDF(pos1 - pos2);
  }
  if (thing.SDF == 1)
  {
    return spherSDF(pos1 - pos2);
  }
  if (thing.SDF == 2)
  {
    return cubSDF(pos1 - pos2);
  }
  if (thing.SDF == 3)
  {
    return fractal(pos1 - pos2);
  }
}

float sceneDistance(vec3 smol)
{
  float ultrasmol = 100.0;
  float dist;
  for (int i = 0; i < SIZE; i++)
  {
    dist = SDF(objs[i], smol, objs[i].pos);
    if (ultrasmol > dist)
    {
      ultrasmol = dist;
    }
  }
  return ultrasmol;
}

void NormalGrab() {
  vec2 e = vec2(1.0, -1.0) * 0.0005;
  normie = normalize(vec3(e.xyy * sceneDistance(fragPos + e.xyy) + e.yyx * sceneDistance(fragPos + e.yyx) + e.yxy * sceneDistance(fragPos + e.yxy) + e.xxx * sceneDistance(fragPos + e.xxx)));
}
float Raymarch(vec3 origin, vec3 rayDir)
{
  float distance = 0.01;
  vec3 smallest;
  float sceneDist;
  for (int i = 0; i < maximum; i++)
  {
    smallest = origin + rayDir * distance;
    sceneDist = sceneDistance(smallest);
    distance += sceneDist;
    if (distance > MAXDIST || distance < 0.0001) break;
    //rayDir = reflect(pathdir, normalize(nrand3(matprops.w, surfnormal)));
  }
  fragPos = origin + rayDir * distance;
  return distance;
}


void main()
{
  vec2 rez = vec2(iResolution.x, iResolution.y);
  vec2 uv = (FragCoord.xy - 0.5 * rez.xy) / rez.x;

  vec3 rayOrigin = camPosition;
  vec3 rayDir = vec3(uv.x, uv.y, 1.0);

  mat3 xRot;
  xRot[0] = vec3(1, 0, 0);
  xRot[1] = vec3(0, cos(mousey), -sin(mousey));
  xRot[2] = vec3(0, sin(mousey), cos(mousey));

  mat3 yRot;
  yRot[0] = vec3(cos(mousex), 0, sin(mousex));
  yRot[1] = vec3(0, 1, 0);
  yRot[2] = vec3(-sin(mousex), 0, cos(mousex));

  mat3 zRot;
  zRot[0] = vec3(cos(mousey), -sin(mousey), 0);
  zRot[1] = vec3(sin(mousey), cos(mousey), 0);
  zRot[2] = vec3(0, 0, 1);

  rayDir *= xRot;
  rayDir *= yRot;

  float raymarched = Raymarch(rayOrigin, rayDir);

  color = vec4(fragPos, 1.0);

  if (raymarched < MAXDIST) imageStore(uTexture, FragCoord, color);
  else imageStore(uTexture, FragCoord, vec4(1000.0, 1000.0, 1000.0, 1.0));
}
