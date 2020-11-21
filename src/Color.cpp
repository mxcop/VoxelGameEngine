#pragma once

#include <glm/glm.hpp>
#include <vector>
using namespace std;
using namespace glm;


class VoxelColor 
{
public:
    float r() { return rgb[0]; }
    float g() { return rgb[1]; }
    float b() { return rgb[2]; }
    float* color() { return rgb; }

    VoxelColor() {
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = 0;
    }

    VoxelColor(float r, float g, float b) {
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
    }

    void setColor(float r, float g, float b) {
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
    }

    inline float clamp_value(float value) {
        value = (value < 0.0f) ? 0.0f : value;
        value = (value > 1.0f) ? 1.0f : value;

        return value;
    }
private:
    float rgb[3];
};