#include <glm/glm.hpp>
#include <vector>
#include "../Color.cpp"

/// <summary>
/// Converts eulerAngles into a rotation matrix.
/// </summary>
/// <param name="theta">of the eulerAngles.</param>
/// <returns>The rotation matrix.</returns>
static glm::mat3x3 eulerAnglesToRotationMatrix(glm::vec3 theta)
{
    // Calculate rotation about x axis
    glm::mat3x3 R_x = {
        1, 0, 0,
        0, cos(theta[0]), -sin(theta[0]),
        0, sin(theta[0]), cos(theta[0])
    };

    // Calculate rotation about y axis
    glm::mat3x3 R_y = {
        cos(theta[1]), 0, sin(theta[1]),
        0, 1, 0,
        -sin(theta[1]), 0, cos(theta[1])
    };

    // Calculate rotation about z axis
    glm::mat3x3 R_z = {
        cos(theta[2]), -sin(theta[2]), 0,
        sin(theta[2]), cos(theta[2]), 0,
        0, 0, 1
    };


    // Combined rotation matrix
    glm::mat3x3 R = R_z * R_y * R_x;

    return R;
}

static unsigned char* VoxelColorData(VoxelColor* input) {
    int inputSize = (sizeof(input) / sizeof(input[0]));
    int outputSize = inputSize * 3;
    unsigned char* output = new unsigned char[outputSize];
    
    for (int i = 0; i < inputSize; i++)
    {
        output[i * 3 + 0] = input[i].r();
        output[i * 3 + 1] = input[i].g();
        output[i * 3 + 2] = input[i].b();
    }
    
    return output;
}