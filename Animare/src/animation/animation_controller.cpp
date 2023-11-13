#include "animation_controller.hpp"

#include "renderer/shader.hpp"
#include "renderer/model.hpp"
#include "track.hpp"

animation_controller::animation_controller(model& _model) {
	// Samplers
	tinygltf::Model& model = _model._model;
	for (auto& samp : model.animations[1].samplers) {
		track _track;

		if (samp.interpolation == "LINEAR") {
			_track.set_interpolation(track::interpolation::linear);
		}
		if (samp.interpolation == "STEP") {
			_track.set_interpolation(track::interpolation::linear);
		}
		if (samp.interpolation == "CUBICSPLINE") {
			_track.set_interpolation(track::interpolation::linear);
		}

		// Read sampler input time values
		{
			const tinygltf::Accessor& accessor = model.accessors[samp.input];
			const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

			assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

			const void* dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];
			const float* buf = static_cast<const float*>(dataPtr);
			_track.resize(accessor.count);
			std::vector<float> inputs;
			std::vector<glm::vec4> outputsVec4;
			for (size_t index = 0; index < accessor.count; index++) {
				inputs.push_back(buf[index]);
				_track.get_frame_at_index(index).set_time(buf[index]);
			}

			for (auto input : inputs) {
				if (input < _model.m_animation_clip.end) {
					_model.m_animation_clip.start = input;
				};
				if (input > _model.m_animation_clip.end) {
					_model.m_animation_clip.end = input;
				}
			}

			real start = std::numeric_limits<float>::max();
			real end = std::numeric_limits<float>::min();
			for (auto input : inputs) {
				if (input < end) {
					start = input;
				};
				if (input > end) {
					end = input;
				}
			}
			_track.set_start_and_end_time(start, end);
		}

		// Read sampler output T/R/S values 
		{
			const tinygltf::Accessor& accessor = model.accessors[samp.output];
			const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

			assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

			const void* dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];

			switch (accessor.type) {
				case TINYGLTF_TYPE_VEC3: {
					const glm::vec3* buf = static_cast<const glm::vec3*>(dataPtr);
					for (size_t index = 0; index < accessor.count; index++) {
						_track.get_frame_at_index(index).set_value(glm::vec4(buf[index], 0.0f));
					}
					break;
				}
				case TINYGLTF_TYPE_VEC4: {
					const glm::vec4* buf = static_cast<const glm::vec4*>(dataPtr);
					for (size_t index = 0; index < accessor.count; index++) {
						_track.get_frame_at_index(index).set_value(buf[index]);
					}
					break;
				}
				default: {
					std::cout << "unknown type" << std::endl;
					break;
				}
			}
		}
		_model.m_animation_clip.add_track(_track);
	}

	for (auto& source : model.animations[0].channels) {
		if (source.target_path == "rotation") {
			_model.m_animation_clip.get_track_at_index(source.sampler).set_data_type(track::data_type::rotation);
		}
		if (source.target_path == "translation") {
			_model.m_animation_clip.get_track_at_index(source.sampler).set_data_type(track::data_type::translation);
		}
		if (source.target_path == "scale") {
			_model.m_animation_clip.get_track_at_index(source.sampler).set_data_type(track::data_type::scale);
		}
		if (source.target_path == "weights") {
			std::cout << "weights not yet supported, skipping channel" << std::endl;
			continue;
		}

		//_model.m_animation_clip.set_target_node(source.sampler, _model.get_node(source.target_node));
		_model.m_animation_clip.set_target_node(source.sampler, _model.node_from_index(source.target_node));
	}
	//_model.m_animation_clip.set_end_time(_model._model.animations[1].end)
}

void animation_controller::play_animation(model& _model, real dt, shader& _shader) {
	m_time += dt;
	if (m_time > _model.m_animation_clip.end) {
		m_time -= _model.m_animation_clip.end;
	}

	bool updated = false;
	for (track& _track: _model.m_animation_clip.get_tracks()) {
		for (int i = 0; i < _track.get_frames().size() - 1; i++) {
			frame& current_frame = _track.get_frame_at_index(i);
			frame& next_frame = _track.get_frame_at_index(i + 1);
			if ((m_time >= current_frame.get_time()) && (m_time <= next_frame.get_time())) {
				real u = std::max(0.0f, m_time - current_frame.get_time()) / (next_frame.get_time() - current_frame.get_time());
				if (u <= 1.0f) {
					switch (_track.get_data_type()) {
						case track::data_type::translation: {
							glm::vec4 trans = glm::mix(current_frame.get_value(), next_frame.get_value(), u);
							_track.get_node()->translation = glm::vec3(trans);
							break;
						}
						case track::data_type::scale: {
							glm::vec4 trans = glm::mix(current_frame.get_value(), next_frame.get_value(), u);
							_track.get_node()->scale = glm::vec3(trans);
							break;
						}
						case track::data_type::rotation: {
							glm::quat q1;
							const glm::vec4& output1 = current_frame.get_value();
							q1.x = output1.x;
							q1.y = output1.y;
							q1.z = output1.z;
							q1.w = output1.w;
							glm::quat q2;
							const glm::vec4& output2 = next_frame.get_value();
							q2.x = output2.x;
							q2.y = output2.y;
							q2.z = output2.z;
							q2.w = output2.w;
							_track.get_node()->rotation = glm::normalize(glm::slerp(q1, q2, u));
							break;
						}
					}
					updated = true;
					break;
				}
			}
		}
	}

	if (updated) {
		for (auto& node : _model.nodes) {
			update(node, _shader);
		}
	}
}

void animation_controller::update(node* _node, shader& _shader) {
	if (_node->_mesh) {
		glm::mat4 m = _node->get_matrix();
		if (_node->_skin) {
			_node->_mesh->skin_data.matrix = m;
			// Update join matrices
			glm::mat4 inverseTransform = glm::inverse(m);
			size_t numJoints = (_node->_skin->joints.size() >= 32) ? 32 : _node->_skin->joints.size();
			for (size_t i = 0; i < numJoints; i++) {
				node* joint_node = _node->_skin->joints[i];
				glm::mat4 jointMat = joint_node->get_matrix() * _node->_skin->inverse_bind_matrices[i];
				jointMat = inverseTransform * jointMat;
				_node->_mesh->skin_data.joint_matrix[i] = jointMat;
			}
			_node->_mesh->skin_data.joint_count = (float)numJoints;

			_shader.bind();

			for (int i = 0; i < _node->_mesh->skin_data.joint_count; i++) {
				_shader.set_uniform_mat4("joint_matrix[" + std::to_string(i) + "]", _node->_mesh->skin_data.joint_matrix[i]);
				//shader.setVec2(("offsets[" + std::to_string(i) + "]")), translations[i]);
			}
			_shader.set_uniform_mat4("matrix", _node->_mesh->skin_data.matrix);
			_shader.set_uniform_float("joint_count", _node->_mesh->skin_data.joint_count);
		}
	}

	for (auto& child : _node->children) {
		//update(child, _shader);
		child->update(&_shader);
	}
}