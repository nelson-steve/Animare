#pragma once

#include "defines.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>

class pose {
public:
    pose() {};
    void add_joint_matrix(const glm::mat4& mat) { m_joint_matrices.push_back(mat); }
    void set_joint_count(int16_t count) { m_joint_count = count; }

    real get_joint_count() const { return m_joint_count; }
    const glm::mat4& get_joint_matrix(int16_t index) { return m_joint_matrices[index]; }
    const std::vector<glm::mat4>& get_joint_matrices() const { return m_joint_matrices; }
private:
    glm::mat4 m_matrix;
    std::vector<glm::mat4> m_joint_matrices;
    real m_joint_count = 0.0f;
};