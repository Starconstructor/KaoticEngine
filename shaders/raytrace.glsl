#version 460 core
layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba32f, binding = 0) uniform image2D uTexture;

ivec2 FragCoord = ivec2(gl_GlobalInvocationID.xy);

vec4 color;
uniform vec3 Color;
uniform vec3 lightPos, camPosition;
uniform float maximum;
uniform float mousex, mousey;

#define SIZE 5
#define MAXDIST 1000

vec3 normie;
int id;
int tID;
vec3 diffcolor;
vec3 fragPos;

int scened = 0;

struct material
{
  int Simplex;
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

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v) {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  i1.y = 1.0 - i1.x;
  //i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

void matGrab(int i)
{
  float val = snoise(fragPos.xy * 5) * snoise(fragPos.yz * 5);
  if (objs[i].mat.Simplex == 1) diffcolor *= val;
  else diffcolor *= 1.f;
}

float spherSDF (vec3 p)
{
  return dot(p, p) - 1 * 1;
}

int bol = 0;

float Raytrace(vec3 origin, vec3 rayDir)
{
  float discriminant = 0.0;
  for (int i = 0; i < SIZE; i++)
  {
    if (objs[i].mat.raytraced == 1)
    {
      vec3 vec = origin - objs[i].pos;
      float a = pow(length(rayDir), 2);
      float b = dot(vec, rayDir);
      float c = spherSDF(vec);
      discriminant = b*b - a*c;

      if (discriminant < 0) return 0.0;
      else
      {
        diffcolor = objs[i].color;
        matGrab(i);
        float distance = (-b - sqrt(discriminant)) / a;
        fragPos = origin + rayDir * distance;
        normie = normalize(fragPos - objs[i].pos);
        return distance;
      }
    }
  }
}

vec3 lighting(vec3 rayOrigin) {
  vec3 lPos = lightPos;
  vec3 lightDeg = normalize(lPos - fragPos);
  float diff = clamp(dot(normie, lightDeg), 0.0, 1.0);

  vec3 camVec = normalize(rayOrigin - fragPos);
  vec3 halfVec = (lightDeg + camVec) / length(lightDeg + camVec);

  float roughness = 0.01;
  float alpha = acos(dot(normie, halfVec));
  float cosi = float(pow(cos(alpha), 2.0));
  float tang = (1 - cosi) / (cosi * roughness);
  float Beckmann = float(exp(-tang) / (3.14 * roughness * pow(cosi, 2.0)));

  float Schlick = 0.13 + (1.0 - 0.13) * pow(1.0 - cos(dot(halfVec, camVec)), 5.0);

  float viewNorm = dot(camVec, normie);

  float eq1 = (2 * dot(halfVec, normie) * viewNorm) / dot(camVec, halfVec);
  float eq2 = (2 * dot(halfVec, normie) * dot(lightDeg, normie)) / dot(camVec, halfVec);
  float geo = min(min(1, eq1), eq2);

  float topHalf = Beckmann * Schlick * geo;
  float bottomHalf = float(3.14 * viewNorm * dot(normie, lightDeg));

  float final = topHalf / bottomHalf;

  float spec = final;

  vec3 lightDir = lPos - fragPos;

  //float shadowCast = Raymarch(fragPos + normie * 0.001, lightDeg);
  //if (shadowCast < length(lightDir))
  //{
  //  diff *= 0.1;
  //  spec = 0.0;
  //}

  vec3 diffuse = diff * diffcolor;

  vec3 specular = spec * Color;

  vec3 ultimate = (diffuse + specular);
  return ultimate;
}

void main() {
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
  scened = 1;
  vec3 final = lighting(rayOrigin);

  float dist = length(fragPos - rayOrigin) / 10;

  //color = vec4(fragPos, 1.0);
  //color = vec4(dist, dist, dist, 1.0);
  color = vec4(final, 1.0);

  if (raytrace != 0.0) imageStore(uTexture, FragCoord, color);
}
