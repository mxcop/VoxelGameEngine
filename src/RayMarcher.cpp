#include <glm/glm.hpp>
using namespace glm;

/* Constant variables */
const int MAX_STEPS = 100;
const float MAX_DIST = 100.0;
const float SURF_DIST = 0.01;

static float Dist(vec3& a, vec3& b)
{
    return pow((a.x - b.x), 2.0) + pow((a.y - b.y), 2.0) + pow((a.z - b.z), 2.0);
}

static float DistSDF(vec3& a, vec3& b)
{
    vec3 q = abs(a) - b;
    return length(max(q.y, 0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f);
}

static float SceneSDF(vec3& point) 
{
    /* TEMP */
    vec3 voxels[5] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0} };

    /* Get the length of the voxels array */
    int length = sizeof(voxels) / sizeof(voxels[0]);

    /* Create variables for finding closest voxel */
    vec3 closest = voxels[0];
    float smallestDist = Dist(point, voxels[0]);

    /* Find the closest voxel by comparing distances */
    for (int i = 1; i < length; i++)
    {
        float dist = Dist(point, voxels[i]);
        if (dist < smallestDist)
        {
            closest = voxels[i];
            smallestDist = dist;
        }
    }

    /* Return the distance to the closest surface of the closest voxel */
    return DistSDF(point, closest);
}

static float RayMarch(vec3& origin, vec3& direction)
{
    float dO = 0.0; // Distance Origin

    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = origin + direction * dO;
        float ds = SceneSDF(p); // Distance to closest surface.
        dO += ds;
        if (dO > MAX_DIST || ds < SURF_DIST)
            break;
    }

    return dO;
}