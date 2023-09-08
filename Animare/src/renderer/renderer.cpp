#include "renderer.hpp"

#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

float vertices[] = {
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f
};

renderer::renderer() {
    if (!glfwInit())
        assert(false);
    m_window = glfwCreateWindow(1280, 720, "Animare", NULL, NULL);
    if (!m_window)
        assert(false);

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        assert(false);

    m_shader = new shader("../shaders/vertex.glsl", "../shaders/pixel.glsl");

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}

void renderer::update() {
    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.8, 0.2, 0.6, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader->bind();
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void renderer::destroy() {
    delete m_shader;
    glfwTerminate();
}