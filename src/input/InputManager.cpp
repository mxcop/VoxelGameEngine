#include <GLFW/glfw3.h>
#include "../Camera.cpp"

class InputManager 
{
public:
    InputManager(void (*f)()) {
        this->f = f;
    }

    static void (*f)();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
            f();

        /*if (key == GLFW_KEY_W && action == GLFW_PRESS)
            cam.position += vec3(0.0, 0.0, 0.1);
        else if (key == GLFW_KEY_S && action == GLFW_PRESS)
            cam.position -= vec3(0.0, 0.0, 0.1);*/
    }
};