#include "model.hpp"

#include "shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

primitive::primitive(uint32_t _first_index, uint32_t _index_count, uint32_t _vertex_count, uint32_t mat_index)
	: first_index(_first_index), index_count(_index_count), vertex_count(_vertex_count), material_index(mat_index)  {
}

mesh::mesh(const glm::mat4& mat) {
	skin_data.matrix = mat;
}

void node::update(shader* _shader) {
	if (_mesh) {
		glm::mat4 m = get_matrix();
		if (_skin) {
			_mesh->skin_data.matrix = m;
			glm::mat4 inverseTransform = glm::inverse(m);
			size_t numJoints = (_skin->joints.size() >= 32) ? 32 : _skin->joints.size();
			for (size_t i = 0; i < numJoints; i++) {
				node* joint_node = _skin->joints[i];
				glm::mat4 jointMat = joint_node->get_matrix() * _skin->inverse_bind_matrices[i];
				jointMat = inverseTransform * jointMat;
				_mesh->skin_data.joint_matrix[i] = jointMat;
			}
			_mesh->skin_data.joint_count = (float)numJoints;

			_shader->bind();

			for (int i = 0; i < _mesh->skin_data.joint_count; i++) {
				_shader->set_uniform_mat4("joint_matrix[" + std::to_string(i) + "]", _mesh->skin_data.joint_matrix[i]);
			}
			_shader->set_uniform_mat4("matrix", _mesh->skin_data.matrix);
			_shader->set_uniform_float("joint_count", _mesh->skin_data.joint_count);
		}
	}

	for (auto& child : children) {
		child->update(_shader);
	}
}

void model::load(const std::string& path, shader* _shader) {
	this->_shader = _shader;
	tinygltf::Model gltf_model;
	tinygltf::TinyGLTF loader;

	std::string error;
	std::string warning;

	bool binary = false;
	size_t extpos = path.rfind('.', path.length());
	if (extpos != std::string::npos) {
		binary = (path.substr(extpos + 1, path.length() - extpos) == "glb");
	}

	bool fileLoaded = binary ? loader.LoadBinaryFromFile(&gltf_model, &error, &warning, path.c_str()) : loader.LoadASCIIFromFile(&gltf_model, &error, &warning, path.c_str());

	loader_info _loader_info{};
	size_t _vertex_count = 0;
	size_t _index_count = 0;

	if (fileLoaded) {
		_model = gltf_model;
		load_textures(gltf_model);
		load_materials(gltf_model);

		const tinygltf::Scene& scene = gltf_model.scenes[gltf_model.defaultScene > -1 ? gltf_model.defaultScene : 0];

		// Get vertex and index buffer sizes up-front
		for (size_t i = 0; i < scene.nodes.size(); i++) {
			get_node_props(gltf_model.nodes[scene.nodes[i]], gltf_model, _vertex_count, _index_count);
		}
		_loader_info.vertex_buffer = new vertex[_vertex_count];
		_loader_info.index_buffer = new uint32_t[_index_count];

		// TODO: scene handling with no default scene
		for (size_t i = 0; i < scene.nodes.size(); i++) {
			const tinygltf::Node node = gltf_model.nodes[scene.nodes[i]];
			load_node(nullptr, node, scene.nodes[i], gltf_model, _loader_info);
		}
		load_skins(gltf_model);

		for (auto _node : linear_nodes) {
			// Assign skins
			if (_node->skin_index > -1) {
				_node->_skin = skins[_node->skin_index];
			}
			// Initial pose
			if (_node->_mesh) {
				_node->update(this->_shader);
			}
		}
	}
	else {
		// TODO: throw
		std::cout << "Could not load gltf file: " << error << std::endl;
		return;
	}

	size_t vertex_buffer_size = _vertex_count * sizeof(vertex);
	size_t index_buffer_size = _index_count * sizeof(uint32_t);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, _loader_info.vertex_buffer, GL_STATIC_DRAW);

	auto size = sizeof(_loader_info.index_buffer);
	glGenBuffers(1, &indices_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, _loader_info.index_buffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(7 * sizeof(float)));

	//glVertexAttribPointer(vaa, size, accessor.componentType,
	//	accessor.normalized ? GL_TRUE : GL_FALSE,
	//	byteStride, BUFFER_OFFSET(accessor.byteOffset));


	assert(vertex_buffer_size > 0);

	delete[] _loader_info.vertex_buffer;
	delete[] _loader_info.index_buffer;
}

void model::load_node(node* parent, const tinygltf::Node& gltf_node, uint32_t nodeIndex, const tinygltf::Model& model, loader_info _loader_info) {
	node* new_node = new node{};
	new_node->index = nodeIndex;
	new_node->parent = parent;
	new_node->name = gltf_node.name;
	new_node->skin_index = gltf_node.skin;
	new_node->matrix = glm::mat4(1.0f);

	std::cout << gltf_node.name << std::endl;

	// Generate local node matrix
	glm::vec3 translation = glm::vec3(0.0f);
	if (gltf_node.translation.size() == 3) {
		translation = glm::make_vec3(gltf_node.translation.data());
		new_node->translation = translation;
	}
	glm::mat4 rotation = glm::mat4(1.0f);
	if (gltf_node.rotation.size() == 4) {
		glm::quat q = glm::make_quat(gltf_node.rotation.data());
		new_node->rotation = glm::mat4(q);
	}
	glm::vec3 scale = glm::vec3(1.0f);
	if (gltf_node.scale.size() == 3) {
		scale = glm::make_vec3(gltf_node.scale.data());
		new_node->scale = scale;
	}
	if (gltf_node.matrix.size() == 16) {
		new_node->matrix = glm::make_mat4x4(gltf_node.matrix.data());
	};

	// Node with children
	if (gltf_node.children.size() > 0) {
		for (size_t i = 0; i < gltf_node.children.size(); i++) {
			load_node(new_node, model.nodes[gltf_node.children[i]], gltf_node.children[i], model, _loader_info);
		}
	}

	// Node contains mesh data
	if (gltf_node.mesh > -1) {
		const tinygltf::Mesh _mesh = model.meshes[gltf_node.mesh];
		mesh* new_mesh = new mesh(new_node->matrix);
		for (size_t j = 0; j < _mesh.primitives.size(); j++) {
			const tinygltf::Primitive& _primitive = _mesh.primitives[j];
			uint32_t vertexStart = static_cast<uint32_t>(_loader_info.vertex_pos);
			uint32_t indexStart = static_cast<uint32_t>(_loader_info.index_pos);
			uint32_t indexCount = 0;
			uint32_t vertexCount = 0;
			glm::vec3 posMin{};
			glm::vec3 posMax{};
			bool hasSkin = false;
			bool hasIndices = _primitive.indices > -1;
			// Vertices
			{
				const float* bufferPos = nullptr;
				const void* bufferJoints = nullptr;
				const float* bufferWeights = nullptr;

				int posByteStride;
				int jointByteStride;
				int weightByteStride;

				int jointComponentType;

				// Position attribute is required
				assert(_primitive.attributes.find("POSITION") != _primitive.attributes.end());

				const tinygltf::Accessor& posAccessor = model.accessors[_primitive.attributes.find("POSITION")->second];
				const tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
				bufferPos = reinterpret_cast<const float*>(&(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
				posMin = glm::vec3(posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]);
				posMax = glm::vec3(posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]);
				vertexCount = static_cast<uint32_t>(posAccessor.count);
				posByteStride = posAccessor.ByteStride(posView) ? (posAccessor.ByteStride(posView) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);

				// Skinning
				// Joints
				if (_primitive.attributes.find("JOINTS_0") != _primitive.attributes.end()) {
					const tinygltf::Accessor& jointAccessor = model.accessors[_primitive.attributes.find("JOINTS_0")->second];
					const tinygltf::BufferView& jointView = model.bufferViews[jointAccessor.bufferView];
					bufferJoints = &(model.buffers[jointView.buffer].data[jointAccessor.byteOffset + jointView.byteOffset]);
					jointComponentType = jointAccessor.componentType;
					jointByteStride = jointAccessor.ByteStride(jointView) ? (jointAccessor.ByteStride(jointView) / tinygltf::GetComponentSizeInBytes(jointComponentType)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC4);
				}

				if (_primitive.attributes.find("WEIGHTS_0") != _primitive.attributes.end()) {
					const tinygltf::Accessor& weightAccessor = model.accessors[_primitive.attributes.find("WEIGHTS_0")->second];
					const tinygltf::BufferView& weightView = model.bufferViews[weightAccessor.bufferView];
					bufferWeights = reinterpret_cast<const float*>(&(model.buffers[weightView.buffer].data[weightAccessor.byteOffset + weightView.byteOffset]));
					weightByteStride = weightAccessor.ByteStride(weightView) ? (weightAccessor.ByteStride(weightView) / sizeof(float)) : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC4);
				}

				hasSkin = (bufferJoints && bufferWeights);

				for (size_t v = 0; v < posAccessor.count; v++) {
					vertex& vert = _loader_info.vertex_buffer[_loader_info.vertex_pos];
					vert.pos = glm::vec4(glm::make_vec3(&bufferPos[v * posByteStride]), 1.0f);

					if (hasSkin)
					{
						switch (jointComponentType) {
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
							const uint16_t* buf = static_cast<const uint16_t*>(bufferJoints);
							vert.joint0 = glm::vec4(glm::make_vec4(&buf[v * jointByteStride]));
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
							const uint8_t* buf = static_cast<const uint8_t*>(bufferJoints);
							vert.joint0 = glm::vec4(glm::make_vec4(&buf[v * jointByteStride]));
							break;
						}
						default:
							// Not supported by spec
							std::cerr << "Joint component type " << jointComponentType << " not supported!" << std::endl;
							break;
						}
					}
					else {
						vert.joint0 = glm::vec4(0.0f);
					}
					vert.weight0 = hasSkin ? glm::make_vec4(&bufferWeights[v * weightByteStride]) : glm::vec4(0.0f);
					// Fix for all zero weights
					if (glm::length(vert.weight0) == 0.0f) {
						vert.weight0 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
					}
					_loader_info.vertex_pos++;
				}
			}
			// Indices
			if (hasIndices)
			{
				const tinygltf::Accessor& accessor = model.accessors[_primitive.indices > -1 ? _primitive.indices : 0];
				const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

				indexCount = static_cast<uint32_t>(accessor.count);
				const void* dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

				switch (accessor.componentType) {
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
					const uint32_t* buf = static_cast<const uint32_t*>(dataPtr);
					for (size_t index = 0; index < accessor.count; index++) {
						_loader_info.index_buffer[_loader_info.index_pos] = buf[index] + vertexStart;
						_loader_info.index_pos++;
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
					const uint16_t* buf = static_cast<const uint16_t*>(dataPtr);
					for (size_t index = 0; index < accessor.count; index++) {
						_loader_info.index_buffer[_loader_info.index_pos] = buf[index] + vertexStart;
						_loader_info.index_pos++;
					}
					break;
				}
				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
					const uint8_t* buf = static_cast<const uint8_t*>(dataPtr);
					for (size_t index = 0; index < accessor.count; index++) {
						_loader_info.index_buffer[_loader_info.index_pos] = buf[index] + vertexStart;
						_loader_info.index_pos++;
					}
					break;
				}
				default:
					std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
					return;
				}
			}
			primitive* new_primitive = new primitive(indexStart, indexCount, vertexCount, _primitive.material > -1 ? _primitive.material : -1);
			new_mesh->primitives.push_back(new_primitive);
		}
		new_node->_mesh = new_mesh;
	}
	if (parent) {
		parent->children.push_back(new_node);
	}
	else {
		nodes.push_back(new_node);
	}
	linear_nodes.push_back(new_node);
}

void model::get_node_props(const tinygltf::Node& node, const tinygltf::Model& model, size_t& vertexCount, size_t& indexCount) {
	if (node.children.size() > 0) {
		for (size_t i = 0; i < node.children.size(); i++) {
			get_node_props(model.nodes[node.children[i]], model, vertexCount, indexCount);
		}
	}
	if (node.mesh > -1) {
		const tinygltf::Mesh mesh = model.meshes[node.mesh];
		for (size_t i = 0; i < mesh.primitives.size(); i++) {
			auto primitive = mesh.primitives[i];
			vertexCount += model.accessors[primitive.attributes.find("POSITION")->second].count;
			if (primitive.indices > -1) {
				indexCount += model.accessors[primitive.indices].count;
			}
		}
	}
}

void model::load_skins(tinygltf::Model& gltfModel) {
	for (tinygltf::Skin& source : gltfModel.skins) {
		skin* new_skin = new skin{};
		new_skin->name = source.name;

		// Find skeleton root node
		if (source.skeleton > -1) {
			new_skin->root = node_from_index(source.skeleton);
		}

		// Find joint nodes
		for (int jointIndex : source.joints) {
			node* _node = node_from_index(jointIndex);
			if (_node) {
				new_skin->joints.push_back(node_from_index(jointIndex));
			}
		}

		// Get inverse bind matrices from buffer
		if (source.inverseBindMatrices > -1) {
			const tinygltf::Accessor& accessor = gltfModel.accessors[source.inverseBindMatrices];
			const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];
			new_skin->inverse_bind_matrices.resize(accessor.count);
			memcpy(new_skin->inverse_bind_matrices.data(), &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::mat4));
		}

		skins.push_back(new_skin);
	}
}

void model::load_textures(tinygltf::Model& gltfModel) {

}

void model::load_materials(tinygltf::Model& gltfModel) {

}

void model::draw_node(node* _node) {

}

void model::draw() {

}

void model::destroy() {

}

node* model::find_node(node* parent, uint32_t index) {
	node* node_found = nullptr;
	if (parent->index == index) {
		return parent;
	}
	for (auto& child : parent->children) {
		node_found = find_node(child, index);
		if (node_found) {
			break;
		}
	}
	return node_found;
}

node* model::node_from_index(uint32_t index) {
	node* node_found = nullptr;
	for (auto& node : nodes) {
		node_found = find_node(node, index);
		if (node_found) {
			break;
		}
	}
	return node_found;
}