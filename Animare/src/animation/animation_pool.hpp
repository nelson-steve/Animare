#pragma once

#include "clip.hpp"
#include "joint.hpp"

#include <tiny_gltf.h>

#include <vector>

class joint;

struct animation {

};

struct skin {
    std::string name;
    joint* root = nullptr;
    std::vector<glm::mat4> inverse_bind_matrices;
    std::vector<joint*> joints;
};

class animation_pool {
public:
    animation_pool() {}
    bool create(const std::string& gltf_path);
private:
    void load_joint(joint* parent, const tinygltf::Node& gltf_node, uint32_t nodeIndex, const tinygltf::Model& model);
    void load_skins(tinygltf::Model& gltfModel);
    joint* find_joint(joint* parent, uint32_t index);
    joint* node_from_index(uint32_t index);
private:
    std::vector<int16_t> m_animation_indexes;
    int16_t m_default_animation = 0;
    std::vector<skin> m_skins;
    std::vector<joint*> m_joints;
    std::vector<joint*> m_linear_joints;
    std::vector<clip> m_animations;
};