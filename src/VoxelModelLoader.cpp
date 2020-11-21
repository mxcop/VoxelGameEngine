#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace glm;

static std::vector<std::string> explode(const std::string& s)
{
    std::string buff{ "" };
    std::vector<std::string> v;

    for (auto n : s)
    {
        if (n != ' ') buff += n; else
            if (n == ' ' && buff != "") { v.push_back(buff); buff = ""; }
    }
    if (buff != "") v.push_back(buff);

    return v;
}

static std::vector<vec3> LoadVoxelModel(const std::string& filepath)
{
    /* Load the shader file using a filestream */
    std::ifstream stream(filepath);

    /* Define some variables */
    std::vector<vec3> voxels;
    std::string line;
    bool fileStart = false;

    /* Loop over every line in the shader file */
    while (getline(stream, line))
    {
        if (fileStart == true) {
            /* Get the data from the file line */
            std::vector<std::string> values = explode(line);

            /* Add voxel on current line to the vector */
            voxels.push_back(vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2])));
        }
        else {
            /* If the header has ended start reading the file */
            if (line.find("end_header") != std::string::npos) {
                fileStart = true;
            }
        }
    }

    /* Return the voxels as an array */
    return voxels;
}