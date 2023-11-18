#pragma once

#include "defines.hpp"
#include "animation/clip.hpp"

#include <tiny_gltf.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class shader;

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
        glm::mat4 joint_matrix[32];
        real joint_count = 0.0f;
    } skin_data;
    mesh(const glm::mat4& mat);
};

struct node;

struct skin {
    std::string name;
    node* root = nullptr;
    std::vector<glm::mat4> inverse_bind_matrices;
    std::vector<node*> joints;
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
    glm::vec3 translation{};
    glm::quat rotation{};
    glm::vec3 scale{ 1.0f };

    const glm::mat4& get_local_matrix() {
        return glm::translate(glm::mat4(1.0f), translation) * glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale) * matrix;
    }
    const glm::mat4& get_matrix() {
        glm::mat4 m = get_local_matrix();
        node* p = parent;
        while (p) {
            m = p->get_local_matrix() * m;
            p = p->parent;
        }
        return m;
    }
    void update(shader* _shader);
};

struct model {
    shader* _shader;
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

    glm::mat4 aabb;

    std::vector<node*> nodes;
    std::vector<node*> linear_nodes;

    std::vector<skin*> skins;
    //std::vector<Texture2D> textures;
    std::vector<material> materials;
    clip m_animation_clip;

    struct Dimensions {
        glm::vec3 min = glm::vec3(FLT_MAX);
        glm::vec3 max = glm::vec3(-FLT_MAX);
    } dimensions;


    struct loader_info {
        uint32_t* index_buffer;
        vertex* vertex_buffer;
        size_t vertex_pos = 0;
        size_t index_pos = 0;
    };

    tinygltf::Model _model;

    model(){}
    void load(const std::string& path, shader* _shader);
    void load_node(node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, loader_info _loader_info);
    void get_node_props(const tinygltf::Node& node, const tinygltf::Model& model, size_t& vertexCount, size_t& indexCount);
    void load_skins(tinygltf::Model& gltfModel);
    void load_textures(tinygltf::Model& gltfModel);
    void load_materials(tinygltf::Model& gltfModel);
    void draw_node(node* _node);
    void draw();
    void destroy();
    node* get_node(uint32_t index) {
        for (auto& n : linear_nodes) {
            if (n->index == index) {
                return n;
            }
        }
        assert(false);
    }
    node* find_node(node* parent, uint32_t index);
    node* node_from_index(uint32_t index);

};