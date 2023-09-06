#pragma once

#include "vec3.hpp"
#include "quaternion.hpp"

class transform {
public:
    transform(const vec3& p, const quaternion& r, const vec3& s) :
        m_position(p), m_rotation(r), m_scale(s) {}
    transform() :
        m_position(vec3(0, 0, 0)),
        m_rotation(quaternion(0, 0, 0, 1)),
        m_scale(vec3(1, 1, 1)   )
    {}

    static transform combine(const transform& a, const transform& b) {
        transform out;
        out.set_scale(a.get_scale() * b.get_scale());
        out.set_rotation(b.get_rotation() * a.get_rotation()) ;
        out.set_position(a.get_rotation() * (a.get_scale() * b.get_position()));
        out.set_position(a.get_position() + out.get_position());
        return out;
    }
    transform inverse(const transform& t) {
        transform inv;
        inv.set_rotation(quaternion::inverse(t.get_rotation()));
        inv.get_scale().set_x(fabs(t.get_scale().x()) < VEC3_EPSILON ?
            0.0f : 1.0f / t.get_scale().x());
        inv.get_scale().set_y(fabs(t.get_scale().y()) < VEC3_EPSILON ?
            0.0f : 1.0f / t.get_scale().y());
        inv.get_scale().set_z(fabs(t.get_scale().z()) < VEC3_EPSILON ?
            0.0f : 1.0f / t.get_scale().z());
        vec3 inv_trans = t.get_position() * -1.0f;
        inv.set_position(inv.get_rotation() * (inv.get_scale() * inv_trans));
        return inv;
    }
    transform mix(const transform& a, const transform& b, float t) {
        quaternion bRot = b.get_rotation();
        if (quaternion::dot(a.get_rotation(), bRot) < 0.0f) {
            bRot = -bRot;
        }
        return transform(
            vec3::lerp(a.get_position(), b.get_position(), t),
            quaternion::nlerp(a.get_rotation(), bRot, t),
            vec3::lerp(a.get_scale(), b.get_scale(), t));
    }
    mat4 transformToMat4(const transform& t) {
        vec3 x = t.get_rotation() * vec3(1, 0, 0);
        vec3 y = t.get_rotation() * vec3(0, 1, 0);
        vec3 z = t.get_rotation() * vec3(0, 0, 1);
        x = x * t.get_scale().x();
        y = y * t.get_scale().y();
        z = z * t.get_scale().z();
        vec3 p = t.get_position();
        return mat4(
            x.x(), x.y(), x.z(), 0,
            y.x(), y.y(), y.z(), 0,
            z.x(), z.y(), z.z(), 0,
            p.x(), p.y(), p.z(), 1
        );
    }
    transform mat4ToTransform(const mat4& m) {
        transform out;
        out.set_position(vec3(m.get_at_index(12), m.get_at_index(13), m.get_at_index(14)));
        out.set_rotation(quaternion::mat4ToQuat(m));
        mat4 rotScaleMat(
            m.get_at_index(0), m.get_at_index(1), m.get_at_index(2), 0,
            m.get_at_index(4), m.get_at_index(5), m.get_at_index(6), 0,
            m.get_at_index(8), m.get_at_index(9), m.get_at_index(10), 0,
            0, 0, 0, 1
        );
        mat4 invRotMat = quaternion::quat_to_mat4(quaternion::inverse(out.get_rotation()));
        mat4 scaleSkewMat = rotScaleMat * invRotMat;
        out.set_scale(vec3(
            scaleSkewMat.get_at_index(0),
            scaleSkewMat.get_at_index(5),
            scaleSkewMat.get_at_index(10)
        ));
        return out;
    }
    vec3 transform_point(const transform& a, const vec3& b) {
        vec3 out;
        out = a.get_rotation() * (a.get_scale() * b);
        out = a.get_position() + out;
        return out;
    }
    vec3 transformVector(const transform& a, const vec3& b) {
        vec3 out;
        out = a.get_rotation() * (a.get_scale() * b);
        return out;
    }

    // getters
    vec3 get_position() const { return m_position; }
    quaternion get_rotation() const { return m_rotation; }
    vec3 get_scale() const { return m_scale; }

    // setters
    void set_position(const vec3& pos) { m_position = pos; }
    void set_rotation(const quaternion& rot) { m_rotation = rot; }
    void set_scale(const vec3& scale) { m_scale = scale; }
private:
    vec3 m_position;
    quaternion m_rotation;
    vec3 m_scale;
};