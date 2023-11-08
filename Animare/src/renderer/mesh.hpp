#pragma once
#if 0
#include <string>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tiny_gltf.h>

class mesh {
public:
    mesh(const std::string& model_path);

    void bindModel();
    void drawModel();
private:
    uint32_t m_vao, m_vbo, v_ebo;
    std::map<int, GLuint> m_vbos;
    tinygltf::Model model;
};
#endif