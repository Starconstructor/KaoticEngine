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
  int Simplex;
  int raytraced;
  float roughness;
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

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
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
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
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

float bunny(vec3 p) {
    if (length(p) > 1.) {
        return (length(p)-0.9);
    }
    vec4 f00=sin(p.y*vec4(-3.02,1.95,-3.42,-.60)+p.z*vec4(3.08,.85,-2.25,-.24)-p.x*vec4(-.29,1.16,-3.74,2.89)+vec4(-.71,4.50,-3.24,-3.50));
    vec4 f01=sin(p.y*vec4(-.40,-3.61,3.23,-.14)+p.z*vec4(-.36,3.64,-3.91,2.66)-p.x*vec4(2.90,-.54,-2.75,2.71)+vec4(7.02,-5.41,-1.12,-7.41));
    vec4 f02=sin(p.y*vec4(-1.77,-1.28,-4.29,-3.20)+p.z*vec4(-3.49,-2.81,-.64,2.79)-p.x*vec4(3.15,2.14,-3.85,1.83)+vec4(-2.07,4.49,5.33,-2.17));
    vec4 f03=sin(p.y*vec4(-.49,.68,3.05,.42)+p.z*vec4(-2.87,.78,3.78,-3.41)-p.x*vec4(-2.65,.33,.07,-.64)+vec4(-3.24,-5.90,1.14,-4.71));
    vec4 f10=sin(mat4(-.34,.06,-.59,-.76,.10,-.19,-.12,.44,.64,-.02,-.26,.15,-.16,.21,.91,.15)*f00+
        mat4(.01,.54,-.77,.11,.06,-.14,.43,.51,-.18,.08,.39,.20,.33,-.49,-.10,.19)*f01+
        mat4(.27,.22,.43,.53,.18,-.17,.23,-.64,-.14,.02,-.10,.16,-.13,-.06,-.04,-.36)*f02+
        mat4(-.13,.29,-.29,.08,1.13,.02,-.83,.32,-.32,.04,-.31,-.16,.14,-.03,-.20,.39)*f03+
        vec4(.73,-4.28,-1.56,-1.80))/1.0+f00;
    vec4 f11=sin(mat4(-1.11,.55,-.12,-1.00,.16,.15,-.30,.31,-.01,.01,.31,-.42,-.29,.38,-.04,.71)*f00+
        mat4(.96,-.02,.86,.52,-.14,.60,.44,.43,.02,-.15,-.49,-.05,-.06,-.25,-.03,-.22)*f01+
        mat4(.52,.44,-.05,-.11,-.56,-.10,-.61,-.40,-.04,.55,.32,-.07,-.02,.28,.26,-.49)*f02+
        mat4(.02,-.32,.06,-.17,-.59,.00,-.24,.60,-.06,.13,-.21,-.27,-.12,-.14,.58,-.55)*f03+
        vec4(-2.24,-3.48,-.80,1.41))/1.0+f01;
    vec4 f12=sin(mat4(.44,-.06,-.79,-.46,.05,-.60,.30,.36,.35,.12,.02,.12,.40,-.26,.63,-.21)*f00+
        mat4(-.48,.43,-.73,-.40,.11,-.01,.71,.05,-.25,.25,-.28,-.20,.32,-.02,-.84,.16)*f01+
        mat4(.39,-.07,.90,.36,-.38,-.27,-1.86,-.39,.48,-.20,-.05,.10,-.00,-.21,.29,.63)*f02+
        mat4(.46,-.32,.06,.09,.72,-.47,.81,.78,.90,.02,-.21,.08,-.16,.22,.32,-.13)*f03+
        vec4(3.38,1.20,.84,1.41))/1.0+f02;
    vec4 f13=sin(mat4(-.41,-.24,-.71,-.25,-.24,-.75,-.09,.02,-.27,-.42,.02,.03,-.01,.51,-.12,-1.24)*f00+
        mat4(.64,.31,-1.36,.61,-.34,.11,.14,.79,.22,-.16,-.29,-.70,.02,-.37,.49,.39)*f01+
        mat4(.79,.47,.54,-.47,-1.13,-.35,-1.03,-.22,-.67,-.26,.10,.21,-.07,-.73,-.11,.72)*f02+
        mat4(.43,-.23,.13,.09,1.38,-.63,1.57,-.20,.39,-.14,.42,.13,-.57,-.08,-.21,.21)*f03+
        vec4(-.34,-3.28,.43,-.52))/1.0+f03;
    f00=sin(mat4(-.72,.23,-.89,.52,.38,.19,-.16,-.88,.26,-.37,.09,.63,.29,-.72,.30,-.95)*f10+
        mat4(-.22,-.51,-.42,-.73,-.32,.00,-1.03,1.17,-.20,-.03,-.13,-.16,-.41,.09,.36,-.84)*f11+
        mat4(-.21,.01,.33,.47,.05,.20,-.44,-1.04,.13,.12,-.13,.31,.01,-.34,.41,-.34)*f12+
        mat4(-.13,-.06,-.39,-.22,.48,.25,.24,-.97,-.34,.14,.42,-.00,-.44,.05,.09,-.95)*f13+
        vec4(.48,.87,-.87,-2.06))/1.4+f10;
    f01=sin(mat4(-.27,.29,-.21,.15,.34,-.23,.85,-.09,-1.15,-.24,-.05,-.25,-.12,-.73,-.17,-.37)*f10+
        mat4(-1.11,.35,-.93,-.06,-.79,-.03,-.46,-.37,.60,-.37,-.14,.45,-.03,-.21,.02,.59)*f11+
        mat4(-.92,-.17,-.58,-.18,.58,.60,.83,-1.04,-.80,-.16,.23,-.11,.08,.16,.76,.61)*f12+
        mat4(.29,.45,.30,.39,-.91,.66,-.35,-.35,.21,.16,-.54,-.63,1.10,-.38,.20,.15)*f13+
        vec4(-1.72,-.14,1.92,2.08))/1.4+f11;
    f02=sin(mat4(1.00,.66,1.30,-.51,.88,.25,-.67,.03,-.68,-.08,-.12,-.14,.46,1.15,.38,-.10)*f10+
        mat4(.51,-.57,.41,-.09,.68,-.50,-.04,-1.01,.20,.44,-.60,.46,-.09,-.37,-1.30,.04)*f11+
        mat4(.14,.29,-.45,-.06,-.65,.33,-.37,-.95,.71,-.07,1.00,-.60,-1.68,-.20,-.00,-.70)*f12+
        mat4(-.31,.69,.56,.13,.95,.36,.56,.59,-.63,.52,-.30,.17,1.23,.72,.95,.75)*f13+
        vec4(-.90,-3.26,-.44,-3.11))/1.4+f12;
    f03=sin(mat4(.51,-.98,-.28,.16,-.22,-.17,-1.03,.22,.70,-.15,.12,.43,.78,.67,-.85,-.25)*f10+
        mat4(.81,.60,-.89,.61,-1.03,-.33,.60,-.11,-.06,.01,-.02,-.44,.73,.69,1.02,.62)*f11+
        mat4(-.10,.52,.80,-.65,.40,-.75,.47,1.56,.03,.05,.08,.31,-.03,.22,-1.63,.07)*f12+
        mat4(-.18,-.07,-1.22,.48,-.01,.56,.07,.15,.24,.25,-.09,-.54,.23,-.08,.20,.36)*f13+
        vec4(-1.11,-4.28,1.02,-.23))/1.4+f13;
    return dot(f00,vec4(.09,.12,-.07,-.03))+dot(f01,vec4(-.04,.07,-.08,.05))+
        dot(f02,vec4(-.01,.06,-.02,.07))+dot(f03,vec4(-.05,.07,.03,.04))-0.16;
}

float cubSDF (vec3 p)
{
  return max(abs(p.x), max(abs(p.y), abs(p.z))) - 0.5;
}

float spherSDF (vec3 p)
{
  return length(p) - 1;
}

float floorSDF (vec3 p)
{
  return p.y;
}

float fractal (vec3 p)
{
  return 0.0;
}

float SDF(gameObject thing, vec3 pos)
{
  if (thing.SDF == 0)
  {
    return floorSDF(pos);
  }
  if (thing.SDF == 1)
  {
    return spherSDF(pos);
  }
  if (thing.SDF == 2)
  {
    return cubSDF(pos);
  }
  if (thing.SDF == 3)
  {
    return bunny(pos);
  }
  if (thing.SDF == 4)
  {
    return fractal(pos);
  }
}

float sceneDistance(vec3 smol)
{
  float ultrasmol = 100.0;
  float dist;
  for (int i = 0; i < SIZE; i++)
  {
    dist = SDF(objs[i], smol - objs[i].pos);
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
  float distance = 0.0001;
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
  return distance;
}


void matGrab(int i)
{
  albedo = objs[i].color;
  NormalGrab();
  float val = snoise(normie.xy * 5);
  if (objs[i].mat.Simplex == 1) albedo *= val;
  else albedo *= 1.f;
  roughness = objs[i].mat.roughness;
  scened = 1;
}

float CookTorrance(vec3 camVec, vec3 lightDeg, vec3 halfVec)
{
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
  return topHalf / bottomHalf;
}

vec3 lighting(vec3 rayOrigin, float dist) {
  for (int i = 0; i < SIZE; i++)
  {
    if (0.001 > SDF(objs[i], fragPos - objs[i].pos) && scened == 0) matGrab(i);
  }
  vec3 lPos = lightPos;
  vec3 lightDeg = normalize(lPos - fragPos);

  float diff = clamp(dot(normie, lightDeg), 0.0, 1.0);

  vec3 camVec = normalize(rayOrigin - fragPos);
  vec3 halfVec = (lightDeg + camVec) / length(lightDeg + camVec);

  float spec = CookTorrance(camVec, lightDeg, halfVec);

  vec3 lightDir = fragPos - lPos;

  float shadowCast = Raymarch(fragPos + normie * 0.001, lightDeg);
  if (shadowCast < length(lightDir))
  {
    diff *= 0.3;
    spec = 0.0;
  }

  vec3 diffuse = diff * albedo;

  vec3 specular = spec * Color;

  vec3 ultimate = (diffuse + specular);
  if (dist > MAXDIST)
  {
    ultimate = vec3(1.0, 0.0, 1.0);
  }
  return ultimate;
}

void main()
{
  vec3 cum = imageLoad(uTexture, FragCoord).rgb;
  fragPos = cum;
  vec3 final = lighting(camPosition, length(fragPos - camPosition));
  color = vec4(final, 1.0);
  imageStore(uTexture, FragCoord, color);
}
