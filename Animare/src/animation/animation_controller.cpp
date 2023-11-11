#include "animation_controller.hpp"

#include "track.hpp"

animation_controller::animation_controller(tinygltf::Model model) {
	// Samplers
	for (auto& samp : model.animations[0].samplers) {
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

			_track.set_start_time(inputs[0]);
			_track.set_end_time(inputs[inputs.size() - 1]);
			//for (auto input : inputs) {
			//	if (input < _animation.start) {
			//		_animation.start = input;
			//	};
			//	if (input > _animation.end) {
			//		_animation.end = input;
			//	}
			//}
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
		m_animation_clip.add_track(_track);
	}

	for (auto& source : model.animations[0].channels) {
		if (source.target_path == "rotation") {
			m_animation_clip.get_track_at_index(source.sampler).set_data_type(track::data_type::rotation);
		}
		if (source.target_path == "translation") {
			m_animation_clip.get_track_at_index(source.sampler).set_data_type(track::data_type::translation);
		}
		if (source.target_path == "scale") {
			m_animation_clip.get_track_at_index(source.sampler).set_data_type(track::data_type::scale);
		}
		if (source.target_path == "weights") {
			std::cout << "weights not yet supported, skipping channel" << std::endl;
			continue;
		}
		//channel.node = node_from_index(source.target_node);
		//if (!channel.node) {
		//	continue;
		//}
	}
}

void animation_controller::play_track() {

}