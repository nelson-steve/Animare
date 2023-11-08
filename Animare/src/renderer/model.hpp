#pragma once

#include <tiny_gltf.h>
#include <glm/glm.hpp>

struct primitive {
    uint32_t first_index;
    uint32_t index_count;
    uint32_t vertex_counts;
    uint32_t material_index;
};

struct material {
    //Texture2D albedo_texture;
};

struct mesh {
    std::vector<primitive> primitives;

};

struct node;

struct skin {
    std::string name;
    node* root = nullptr;
    std::vector<glm::mat4> inverse_bind_matrices;
    std::vector<node*>joints;
};

struct node {
    node* parent;
    uint32_t index;
    std::vector<node*> children;
    glm::mat4 matrix;
    std::string name;
    mesh mesh;
    skin _skin;
    int32_t skin_index = -1;
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct animation_channel {
    enum path_type { TRANSLATION, ROTATION, SCALE };
    path_type path;
    node* node;
    uint32_t sampler_index;
};

struct animation_sampler {
    enum interpolation_type { LINEAR, STEP, CUBICSPLINE };
    interpolation_type interpolation;
    std::vector<float> inputs;
    std::vector<glm::vec4> outputsVec4;
};

struct animation {
    std::string name;
    std::vector<animation_sampler> samplers;
    std::vector<animation_channel> channels;
    float start = std::numeric_limits<float>::max();
    float end = std::numeric_limits<float>::min();
};

struct model {
    uint32_t vao;

    struct vertex {
        glm::vec3 pos;
        glm::vec4 joint0;
        glm::vec4 weight0;
    };

    struct vertices {
        uint32_t vbo;
    } vertices;

    struct indices {
        uint32_t vbo;
    } indices;

    std::vector<node*> nodes;
    std::vector<node*> linea_nodes;

    std::vector<skin*> skins;
    //std::vector<Texture2D> textures;
    std::vector<material> materials;
    std::vector<animation> animations;

    struct loader_info {
        uint32_t* index_buffer;
        vertex* vertex_buffer;
        size_t index_pos = 0;
        size_t index_pos = 0;
    };

    model(const std::string& path);
private:
    tinygltf::Model m_model;

};