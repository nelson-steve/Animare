#include "renderer.hpp"

#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

renderer::renderer() {
    if (!glfwInit())
        assert(false);
    m_window = glfwCreateWindow(1280, 720, "Animare", NULL, NULL);
    if (!m_window)
        assert(false);

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        assert(false);
}

void renderer::update() {
    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.2, 0.2, 2.0, 1);


        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void renderer::destroy() {
    glfwTerminate();
}