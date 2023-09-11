#pragma once

#include "defines.hpp"
#include <cstdint>

template<uint32_t n>
class frame {
private:
    real m_value[n];
    real m_in[n];
    real m_out[n];
    real m_time;
};

typedef frame<1> scalar_frame;
typedef frame<3> vector_frame;
typedef frame<4> quaternion_frame;