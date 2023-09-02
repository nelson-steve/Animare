#include <iostream>

#include "vec3.hpp"
#include "vec2.hpp"

int main(int argc, char** argv) {
	vec2 vec(0.1f, 1.0f);
	i_vec2 vec(3, 3);
	u_vec2 vec(3, 3);

	std::cout << vec.x();
}