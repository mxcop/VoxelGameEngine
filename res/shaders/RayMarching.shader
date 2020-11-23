#shader vertex
#version 430 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}


#shader fragment
#version 430 core

precision highp float;
uniform float u_time;
uniform int u_voxelCount;
uniform vec2 u_resolution;
uniform vec3 u_cameraPosition;
uniform mat3 u_cameraOrientation;
uniform vec3 u_voxels[160];

out vec4 fragColor;

//layout(std430, binding = 1) buffer layoutName
//{
//    vec3 u_voxels[];
//};

//uniform vector<vec3> u_voxels;

// Constants
#define PI 3.1415925359
#define TWO_PI 6.2831852
#define MAX_STEPS 32 // Mar Raymarching steps
#define MAX_DIST 100. // Max Raymarching distance
#define SURF_DIST .01 // Surface Distance

/// SUMMARY : gets the distance from a to b;
float GetDist(vec3 a, vec3 b)
{
    return /*min(*/pow((a.x - b.x), 2.0) + pow((a.y - b.y), 2.0) + pow((a.z - b.z), 2.0)/*, a.y)*/;
}

/// vec3 p  : position to check against;
/// vec3 cp : center of the box;
/// SUMMARY : gets the signed distance from p to the box;
float GetBoxDist(vec3 p, vec3 cp)
{
    // vec3 b : bounds of the box;
    vec3 b = vec3(.1, .1, .1);

    // vec3 q : subtracts both positions and the bounds;
    vec3 q = abs(p - cp) - b;

    // get the signed distance for the box;
    float boxDist = length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
    // get the signed distance for the ground plane;
    //float planeDist = p.y;

    // get the minimum of these distances;
    //float d = min(boxDist, planeDist);

    return boxDist;
}

/// vec3 p  : position to check against;
/// SUMMARY : gets the signed distance from p to the scene;
float GetSceneDist(vec3 p)
{
    // smallest distance.
    float d = GetDist(p, u_voxels[0]);
    vec3 v = u_voxels[0] * .1;
    v += vec3(0.0, 1.0, 0.0);

    for (int i = 1; i < u_voxelCount; i++)
    {
        //if (u_voxels[i] == vec3(0., 0., 0.))
        //    break;

        vec3 voxel = u_voxels[i] * .1;
        voxel += vec3(0.0, 1.0, 0.0);

        float vd = GetDist(p, voxel);

        if (vd < d) {
            d = vd;
            v = voxel;
        }
    }

    return min(GetBoxDist(p, v), p.y);

    //for (int k = 0; k < 10; ++k)
    //    if (u_voxelCount == k)
    //        return min(GetBoxDist(p, u_voxels[k + 2]), p.y);
    //return min(GetBoxDist(p, u_voxels[0]), p.y);
}

/// vec3 ro : ray origin;
/// vec3 rd : ray direction;
/// SUMMARY : performs the ray march algorithm;
float RayMarch(vec3 ro, vec3 rd)
{
    float dO = 0.; //Distane Origin
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + rd * dO;
        float ds = GetSceneDist(p); // ds is Distance Scene ! PROBLEM !
        dO += ds;
        if (dO > MAX_DIST || ds < SURF_DIST)
            break;
    }
    return dO;
}

vec3 GetNormal(vec3 p)
{
    float d = GetSceneDist(p); // Distance
    vec2 e = vec2(.01, 0); // Epsilon
    vec3 n = d - vec3(
        GetSceneDist(p - e.xyy),
        GetSceneDist(p - e.yxy),
        GetSceneDist(p - e.yyx));

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

    //if (d >= MAX_DIST) dif = 0.0;
    if (d < length(lightPos - p)) dif *= .2;

    return dif;
}

void main()
{
    //int i = u_voxelCount;
    //gl_FragColor = vec4(i, i, i, 1.0);
    //if (u_voxels.length() > 0) {
        //vec2 uv = (gl_FragCoord.xy - .5 * u_resolution.xy) / u_resolution.y;

        vec2 uv = (gl_FragCoord.xy - .5 * u_resolution.xy) / u_resolution.y;

        vec3 ro = u_cameraPosition;
        vec3 rd = u_cameraOrientation * normalize(vec3(uv, 1.0));

        //vec3 ro = u_cameraPosition;
        //ro.yz *= Rot(25.2);
        //ro.xz *= Rot(u_time * .2 - u_AspectRatio.x * 6.2831);

        //vec3 rd = R(uv, ro, vec3(0, 0, 0), .3);

        //vec3 ro = vec3(0, 1, 0); // Ray Origin/Camera
        //vec3 rd = normalize(vec3(uv.x, uv.y, 1)); // Ray Direction

        //vec4 vm = viewMatrix(ro, ro, vec3(0, .9, 0));

        //vec4 mo = vec4(ro, 1.0) * vm;
        //vec4 md = vec4(rd, 1.0) * vm;

        float d = RayMarch(ro, rd); // Distance ! PROBLEM !
        //float d = 255.;
        vec3 p = ro + rd * d;
        float dif = GetLight(p); // Diffuse lighting
        //d *= .02;
        vec3 color = vec3(dif);
        //color += GetNormal(p);
        //float color = GetLight(p);

        // Set the output color
        //gl_FragColor = vec4(color, 1.0);
        fragColor = vec4(color, 1.0);
    //}
    //else {
    //    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //}

        /*if (u_voxelCount == 3) {
            fragColor = vec4(0.0, 0.0, 1.0, 1.0);
        }
        else {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }*/
}
