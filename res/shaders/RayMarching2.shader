#shader vertex
#version 450 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}


#shader fragment
#version 450 core

precision highp float;
uniform float u_time;
//uniform int u_voxelCount;
uniform vec2 u_resolution;
//uniform vec3 u_cameraPosition;
//uniform mat3 u_cameraOrientation;
//uniform vec3 u_voxels[];

out vec4 fragColor;

// Constants
#define PI 3.1415925359
#define TWO_PI 6.2831852
#define MAX_STEPS 100 // Mar Raymarching steps
#define MAX_DIST 100. // Max Raymarching distance
#define SURF_DIST .01 // Surface Distance

float GetDist(vec3 p)
{
    vec4 s = vec4(0, 1, 6, 1); //Sphere xyz is position w is radius
    float sphereDist = length(p - s.xyz) - s.w;
    float planeDist = p.y;
    float d = min(sphereDist, planeDist);
    return d;
}

vec3 rotate(vec3 point, vec2 center, float theta) {
    point = point.zxy;
    float rotatedX = cos(theta) * (point.x - center.x) - sin(theta) * (point.y - center.y) + center.x;
    float rotatedY = sin(theta) * (point.x - center.x) + cos(theta) * (point.y - center.y) + center.y;
    return vec3(rotatedX, rotatedY, point.z);
}

float GetCubeDist(vec3 p)
{
    vec3 b = vec3(1, 1, 1);
    vec4 s = vec4(0, 1, 6, 0);

    vec3 q = abs(p - s.xyz) - b - s.w;
    //vec3 qr = rotate(q, vec2(0.0, 0.0), sin(u_time));
    float boxDist = length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
    float planeDist = p.y;
    float d = min(boxDist, planeDist);

    return d;
}

float RayMarch(vec3 ro, vec3 rd)
{
    float dO = 0.; //Distane Origin
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + rd * dO;
        float ds = GetCubeDist(p); // ds is Distance Scene
        dO += ds;
        if (dO > MAX_DIST || ds < SURF_DIST)
            break;
    }
    return dO;
}

vec3 GetNormal(vec3 p)
{
    float d = GetCubeDist(p); // Distance
    vec2 e = vec2(.01, 0); // Epsilon
    vec3 n = d - vec3(
        GetCubeDist(p - e.xyy),
        GetCubeDist(p - e.yxy),
        GetCubeDist(p - e.yyx));

    return normalize(n);
}

float GetLight(vec3 p)
{
    // Directional light
    vec3 lightPos = vec3(5. * sin(u_time), 5., 5.0 * cos(u_time)); // Light Position
    vec3 l = normalize(lightPos - p); // Light Vector
    vec3 n = GetNormal(p); // Normal Vector

    float dif = dot(n, l); // Diffuse light
    dif = clamp(dif, 0., 1.); // Clamp so it doesnt go below 0

    // Shadows
    float d = RayMarch(p + n * SURF_DIST * 2., l);

    if (d < length(lightPos - p)) dif *= .1;

    return dif;
}

void main()
{
    //vec2 uv = (gl_FragCoord.xy - .5 * u_resolution.xy) / u_resolution.y;

    //vec3 ro = vec3(0, 2, 0); // Ray Origin/Camera
    //vec3 rd = normalize(vec3(uv.xy, 1.)); // Ray Direction

    //float d = RayMarch(ro, rd); // Distance

    //vec3 p = ro + rd * d;
    //float dif = GetLight(p); // Diffuse lighting
    //d *= .2;
    //vec3 color = vec3(dif);
    //color += GetNormal(p);
    //float color = GetLight(p);
    
    // Set the output color
    float c = (sin(u_time * 45) + 1) / 2.0;
    fragColor = vec4(c, c, c, 1.0);
}
