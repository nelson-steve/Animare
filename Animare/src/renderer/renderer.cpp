#include "renderer.hpp"

#include "animation/bezier.hpp"

#include <assert.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "tiny_gltf.h"

float vertices[] = {
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f
};

glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
bezier<glm::vec3> curve;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

renderer::renderer() {
    // glfw initialization
    if (!glfwInit())    
        assert(false);
    // window creation
    m_window = glfwCreateWindow(1280.0f, 720.0f, "Animare", NULL, NULL);
    if (!m_window)
        assert(false);

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    // loading glfw functions with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        assert(false);

    m_camera = new camera();

    // creating shaders
    m_shader = new shader("../assets/shaders/vertex.glsl", "../assets/shaders/pixel.glsl");
    m_shader->bind();

    std::string model_path = "../assets/models/DamagedHelmet/glTF/DamagedHelmet.gltf";
    m_mesh = new mesh(model_path);
    
    m_mesh->bindModel();
    curve.point1 = glm::vec3(-3, 0, 0);
    curve.point2 = glm::vec3(3, 0, 0);
    curve.control1 = glm::vec3(-2, 2, 0);
    curve.control2 = glm::vec3(2, 2, 0);

    //m_mesh->drawModel();

}

void renderer::render() {
    line l1(curve.point1, curve.control1);
    line l2(curve.control1, curve.control2);
    line l3(curve.point2, curve.control2);
    //std::vector<glm::vec3> points1;
    //points1.push_back(curve.point1);
    //points1.push_back(curve.control1);
    //
    //points1.push_back(curve.control1);
    //points1.push_back(curve.control2);
    //
    //points1.push_back(curve.point2);
    //points1.push_back(curve.control2);
    //line l1(points1);

    while (!glfwWindowShouldClose(m_window))
    {
        real current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera->Update(delta_time, m_window);

        {
            //glm::mat4 model = glm::mat4(1.0f);
            //m_shader->bind();
            //m_shader->set_uniform_mat4("u_model", model);
            //m_shader->set_uniform_mat4("u_view", m_camera->GetView());
            //m_shader->set_uniform_mat4("u_projection", m_camera->GetProjection());
        }

        l1.draw(m_camera, red);
        l2.draw(m_camera, green);
        l3.draw(m_camera, red);

        for (int i = 0; i < 19; ++i) {
            float t0 = (float)i / 19.0f;
            float t1 = (float)(i + 1) / 19.0f;
            glm::vec3 thisPoint = curve.Interpolate(curve, t0);
            glm::vec3 nextPoint = curve.Interpolate(curve, t1);
            line _line(thisPoint, nextPoint);
            _line.draw(m_camera, blue);
            //DrawLine(thisPoint, nextPoint, blue);
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void renderer::destroy() {
    delete m_shader;
    delete m_mesh;
    delete m_camera;
    glfwTerminate();
}

line::line(const glm::vec3& p1, const glm::vec3& p2) {
    std::vector<glm::vec3> points = {
        p1,
        p2
    };
    m_points = points;
    m_shader = new shader("../assets/shaders/vertex.glsl", "../assets/shaders/pixel.glsl");
    m_shader->bind();

    //glGenVertexArrays(1, &m_vao);
    //glGenBuffers(1, &m_vbo);
    //glBindVertexArray(m_vao);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(points), points.data(), GL_STATIC_DRAW);
    //
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

line::line(const std::vector<glm::vec3>& points) {
    m_points = points;
    m_vertex_count = points.size();
    m_shader = new shader("../assets/shaders/vertex.glsl", "../assets/shaders/pixel.glsl");
    m_shader->bind();
}

void line::draw(camera* cam, const glm::vec3& color) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_shader->bind();
    m_shader->set_uniform_vec3("u_color", color);
    m_shader->set_uniform_mat4("u_model", model);
    m_shader->set_uniform_mat4("u_view", cam->GetView());
    m_shader->set_uniform_mat4("u_projection", cam->GetProjection());
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glDrawArrays(GL_LINES, 0, 6);

    glDeleteBuffers(1, &m_vbo);
}