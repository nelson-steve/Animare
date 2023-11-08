#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

class joint {
public:
    joint() {
        parent = nullptr;
    }
    ~joint() {
        for (auto joint : m_children) {
            delete joint;
        }
    }

    void set_joint_name(const std::string& name) {
        m_name = name;
    }
    void set_parent(joint* p) {
        parent = p;
    }
    void add_children(joint* j) {
        m_children.push_back(j);
    }
private:
    std::string m_name;
    joint* parent;
    glm::mat4 transform;
    std::vector<joint*> m_children;
};

class pose {
public:
    pose();
    ~pose() {
        delete root;
    }
private:
    joint* root;
};