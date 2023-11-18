#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <vector>

class joint {
public:
    joint() {}
    ~joint() {
        for (auto joint : m_children) {
            delete joint;
        }
    }

    void set_joint_name(const std::string& name) {
        m_name = name;
    }

    void set_parent(joint* p) {
        m_parent = p;
    }

    void add_children(joint* j) {
        m_children.push_back(j);
    }

    const glm::mat4& get_local_matrix() {
        return glm::translate(glm::mat4(1.0f), m_translation) * glm::mat4(m_rotation) * glm::scale(glm::mat4(1.0f), m_scale) * m_matrix;
    }

    const glm::mat4& get_global_matrix() {
        glm::mat4 m = get_local_matrix();
        joint* p = m_parent;
        while (p) {
            m = p->get_local_matrix() * m;
            p = p->m_parent;
        }
        return m;
    }

public:
    joint* m_parent;
    uint32_t m_index;
    std::vector<joint*> m_children;
    glm::mat4 m_matrix;
    std::string m_name;
    //skin* _skin;
    int32_t m_skin_index = -1;
    glm::vec3 m_translation{};
    glm::quat m_rotation{};
    glm::vec3 m_scale{ 1.0f };
};