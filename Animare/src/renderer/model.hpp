#pragma once

#include "defines.hpp"

#include <tiny_gltf.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct material;

struct primitive {
    uint32_t first_index;
    uint32_t index_count;
    uint32_t vertex_count;
    uint32_t material_index;
    primitive(uint32_t first_index, uint32_t index_count, uint32_t vertex_count, uint32_t mat_index);
};

struct material {
    //Texture2D albedo_texture;
};

struct mesh {
    std::vector<primitive*> primitives;
    struct {
        glm::mat4 matrix;
        glm::mat4 joint_matrix[128];
        real joint_count = 0.0f;
    } skin_data;
    mesh(const glm::mat4& mat);
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
    mesh* _mesh;
    skin* _skin;
    int32_t skin_index = -1;
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;

    void update() {

    }
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
    uint32_t vertices_vbo;
    uint32_t indices_vbo;

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
    std::vector<node*> linear_nodes;

    std::vector<skin*> skins;
    //std::vector<Texture2D> textures;
    std::vector<material> materials;
    std::vector<animation> animations;

    struct loader_info {
        uint32_t* index_buffer;
        vertex* vertex_buffer;
        size_t vertex_pos = 0;
        size_t index_pos = 0;
    };

    void load(const std::string& path);
    void load_node(node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, loader_info _loader_info);
    void get_node_props(const tinygltf::Node& node, const tinygltf::Model& model, size_t& vertexCount, size_t& indexCount);
    void load_skins(tinygltf::Model& gltfModel);
    void load_textures(tinygltf::Model& gltfModel);
    void load_materials(tinygltf::Model& gltfModel);
    void load_animations(tinygltf::Model& gltfModel);
    void draw_node(node* _node);
    void draw();
    void getSceneDimensions();
    void update_animation(uint32_t index, float time);
    void destroy();
    node* findNode(node* parent, uint32_t index);
    node* nodeFromIndex(uint32_t index);

};