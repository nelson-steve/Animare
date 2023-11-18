#include "animation_pool.hpp"

#include <tiny_gltf.h>
#include <glm/gtc/type_ptr.hpp>

void animation_pool::load_joint(joint* parent, const tinygltf::Node& gltf_node, uint32_t nodeIndex, const tinygltf::Model& model) {
	joint* new_joint = new joint{};
	new_joint->m_index = nodeIndex;
	new_joint->m_parent = parent;
	new_joint->m_name = gltf_node.name;
	new_joint->m_skin_index = gltf_node.skin;
	new_joint->m_matrix = glm::mat4(1.0f);

	std::cout << gltf_node.name << std::endl;

	// Generate local node matrix
	glm::vec3 translation = glm::vec3(0.0f);
	if (gltf_node.translation.size() == 3) {
		translation = glm::make_vec3(gltf_node.translation.data());
		new_joint->m_translation= translation;
	}
	glm::mat4 rotation = glm::mat4(1.0f);
	if (gltf_node.rotation.size() == 4) {
		glm::quat q = glm::make_quat(gltf_node.rotation.data());
		new_joint->m_rotation = glm::mat4(q);
	}
	glm::vec3 scale = glm::vec3(1.0f);
	if (gltf_node.scale.size() == 3) {
		scale = glm::make_vec3(gltf_node.scale.data());
		new_joint->m_scale = scale;
	}
	if (gltf_node.matrix.size() == 16) {
		new_joint->m_matrix = glm::make_mat4x4(gltf_node.matrix.data());
	};

	// Node with children
	if (gltf_node.children.size() > 0) {
		for (size_t i = 0; i < gltf_node.children.size(); i++) {
			load_joint(new_joint, model.nodes[gltf_node.children[i]], gltf_node.children[i], model);
		}
	}

	if (parent) {
		parent->m_children.push_back(new_joint);
	}
	else {
		m_joints.push_back(new_joint);
	}
	m_linear_joints.push_back(new_joint);
}

void animation_pool::load_skins(tinygltf::Model& gltfModel) {
	for (tinygltf::Skin& source : gltfModel.skins) {
		skin new_skin;
		new_skin.name = source.name;

		// Find skeleton root node
		if (source.skeleton > -1) {
			new_skin.root = node_from_index(source.skeleton);
		}

		// Find joint nodes
		for (int jointIndex : source.joints) {
			joint* _node = node_from_index(jointIndex);
			if (_node) {
				new_skin.joints.push_back(node_from_index(jointIndex));
			}
		}

		// Get inverse bind matrices from buffer
		if (source.inverseBindMatrices > -1) {
			const tinygltf::Accessor& accessor = gltfModel.accessors[source.inverseBindMatrices];
			const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];
			new_skin.inverse_bind_matrices.resize(accessor.count);
			memcpy(new_skin.inverse_bind_matrices.data(), &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::mat4));
		}

		m_skins.push_back(new_skin);
	}
}

bool animation_pool::create(const std::string& gltf_path) {
	tinygltf::Model gltf_model;
	tinygltf::TinyGLTF loader;

	std::string error;
	std::string warning;

	bool binary = false;
	size_t extpos = gltf_path.rfind('.', gltf_path.length());
	if (extpos != std::string::npos) {
		binary = (gltf_path.substr(extpos + 1, gltf_path.length() - extpos) == "glb");
	}

	bool fileLoaded = binary ? loader.LoadBinaryFromFile(&gltf_model, &error, &warning, gltf_path.c_str()) : loader.LoadASCIIFromFile(&gltf_model, &error, &warning, gltf_path.c_str());

	if (!fileLoaded) {
		std::cout << "gltf file could not be loaded: " << gltf_path << std::endl;
		return false;
	}

	const tinygltf::Scene& scene = gltf_model.scenes[gltf_model.defaultScene > -1 ? gltf_model.defaultScene : 0];

	for (size_t i = 0; i < scene.nodes.size(); i++) {
		const tinygltf::Node node = gltf_model.nodes[scene.nodes[i]];
		load_joint(nullptr, node, scene.nodes[i], gltf_model);
	}
	load_skins(gltf_model);
}

joint* animation_pool::find_joint(joint* parent, uint32_t index) {
	joint* node_found = nullptr;
	if (parent->m_index == index) {
		return parent;
	}
	for (auto& child : parent->m_children) {
		node_found = find_joint(child, index);
		if (node_found) {
			break;
		}
	}
	return node_found;
}

joint* animation_pool::node_from_index(uint32_t index) {
	joint* node_found = nullptr;
	for (auto& node : m_joints) {
		node_found = find_joint(node, index);
		if (node_found) {
			break;
		}
	}
	return node_found;
}