#version 460 core
layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba32f, binding = 0) uniform image2D uTexture;

ivec2 FragCoord = ivec2(gl_GlobalInvocationID.xy);

vec4 color;
uniform vec3 Color;
uniform vec3 lightPos, camPosition;
uniform float maximum;
uniform float mousex, mousey;

float discriminant = 0.00;

#define SIZE 5

vec3 normie;
int id;
int tID;
vec3 albedo;
vec3 fragPos;
float roughness;

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

float distance;

float spher(vec3 p, vec3 rayDir)
{
  float distanc = 1.0;
  float a = dot(rayDir, rayDir);
  float b = 2.0*dot(rayDir, p);
  float c = dot(p, p) - 1 * 1;
  discriminant = b*b - 4*a*c;
  distanc = (-b - sqrt(discriminant)) / (2.0*a);
  return distanc;
}

float Raytrace(vec3 origin, vec3 rayDir)
{
  float distanc = 1.0;
  for (int i = 0; i < SIZE; i++)
  {
    if (objs[i].mat.raytraced == 1)
    {
      vec3 vec = origin - objs[i].pos;
      distanc = spher(vec, rayDir);

      if (discriminant > 0.0)
      {
        albedo = objs[i].color;
        fragPos = origin + rayDir * distanc;
        normie = normalize(fragPos - objs[i].pos);
        distance = distanc;
      }
      return discriminant;
    }
  }
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

  float raytrace = Raytrace(rayOrigin, rayDir);
  if (raytrace > 0.0)
  {
    vec3 cum = imageLoad(uTexture, FragCoord).rgb;
    float dist = length(fragPos - camPosition);
    float dick = length(cum - camPosition);

    if (dick > dist)
    {
      color = vec4(fragPos, 1.0);
      imageStore(uTexture, FragCoord, color);
    }
  }
}
