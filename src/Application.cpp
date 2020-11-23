#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include "Camera.cpp"
#include "VoxelModelLoader.cpp"
//#include "input/InputManager.cpp"
#include "shaders/Shader.cpp"

static const int screenWidth  = 1920; // 1280
static const int screenHeight = 1080; // 720

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "GLFW couldn't initialize!" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "Voxel Graphics Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Window couldn't initialize!" << std::endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glew */
    if (glewInit() != GLEW_OK)
        std::cout << "Glew couldn't initialize!" << std::endl;
    else
        std::cout << glGetString(GL_VERSION) << std::endl;

    /* Hide the cursor */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Create a camera */
    WorldCamera cam = WorldCamera(vec3(0.0, 3.0, 8.0), vec3(15.0, 180.0, 0.0));

    /* Create input manager */
    //InputManager input = InputManager(moveForward);

    //glfwSetKeyCallback(window, input.key_callback);

    float positions[12]{
        -1.0, -1.0,
        -1.0, 1.0,
        1.0, -1.0,

        -1.0, 1.0,
        1.0, 1.0,
        1.0, -1.0
    };

    /* Generate buffer */
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    /* Bind the triangle buffer */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /* Create a buffer for our triangle */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    /* Shaders */
    ShaderProgramSource source = ParseShader("res/shaders/RayMarching.shader");
    unsigned int shader = CreateShaderVF(source.vertexSource, source.fragmentSource);

    /* Load voxel model */
    std::vector<vec3> mushroom = LoadVoxelModel("res/models/mushroom.ply");

    // std::cout << source.fragmentSource << std::endl;
    //int outputSize = screenWidth * screenHeight * 3;
    //unsigned char* output = new unsigned char[outputSize];
    ////int count = 0;

    //for (int i = 0; i < screenWidth * screenHeight; i++)
    //{
    //    //count += 1;
    //    //output[i] = 255;
    //    output[i * 3 + 0] = abs(sin(i / 100.0)) * 255;
    //    output[i * 3 + 1] = abs(sin(i / 200.0)) * 255;
    //    output[i * 3 + 2] = abs(sin(i / 300.0)) * 255;
    //}
    ////std::cout << count << std::endl;

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT/* | GL_TEXTURE_2D*/);

        /* Calculate the frame */
        //VoxelColor* screen = cam.CalculateFrame(screenWidth, screenHeight);

        //unsigned char* screenData = VoxelColorData(screen);
        //unsigned char* screenData = cam.CalculateFrameC(screenWidth, screenHeight);
        
        /* Put the screen data into a texture */
        //GLuint texture_map;
        //glEnable(GL_TEXTURE_2D);
        //glGenTextures(1, &texture_map);
        //glBindTexture(GL_TEXTURE_2D, texture_map);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, screenData);
        
        /* Player input */
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.position -= vec3(0.0, 0.0, 0.1);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.position += vec3(0.0, 0.0, 0.1);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.position += vec3(0.1, 0.0, 0.0);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.position -= vec3(0.1, 0.0, 0.0);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        cam.Rotate(vec3(ypos - mouseY, xpos - mouseX, 0.0) / 10.0f);

        mouseX = xpos;
        mouseY = ypos;

        /* Rotate camera */
        cam.Rotate(vec3(0.0, 0.0, 0.0));

        /* Setup and run the shader */
        glUseProgram(shader);
        
        vec3* camPos = &cam.GetPosition();
        vec3 voxels[3] = { vec3(0.0, 1.0, 0.0), vec3(0.0, 4.0, 0.0), vec3(3.0, 1.0, 0.0) };
        std::vector<int> voxelCount = std::vector<int>();
        voxelCount.push_back(3);
        float time = (float)glfwGetTime();

        glUniform1f(glGetUniformLocation(shader, "u_time"), time);
        glUniform1i(glGetUniformLocation(shader, "u_voxelCount"), mushroom.size());
        glUniform2f(glGetUniformLocation(shader, "u_resolution"), screenWidth, screenHeight);
        glUniform3f(glGetUniformLocation(shader, "u_cameraPosition"), camPos->x, camPos->y, camPos->z);
        glUniformMatrix3fv(glGetUniformLocation(shader, "u_cameraOrientation"), 1, GL_FALSE, &cam.GetRotation()[0][0]);
        glUniform3fv(glGetUniformLocation(shader, "u_voxels"), mushroom.size(), glm::value_ptr(mushroom[0]));

        /* Draw a quad that covers the screen */
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}