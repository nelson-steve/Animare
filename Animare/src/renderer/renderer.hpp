#pragma once

#define GLFW_INCLUDE_NONE

struct GLFWwindow;

class renderer {
public:
    renderer();
    void update();
    void destroy();
private:
    GLFWwindow* m_window;
};