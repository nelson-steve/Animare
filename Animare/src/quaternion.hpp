#pragma once

#include "defines.hpp"
#include "vec3.hpp"
#include "mat4.hpp"

// quaternion
// quaternions are used to store rotations
// rotation about an arbitrary axis
// quternion -> x, y, z, w -> where x, y, z are angles about 3 dimensional axis(i, j, k)
// w is a real number
// positive anlgles mean a counter-clockwise roation about an axis
class quaternion {
public:
    // creates an identity quaternion (0, 0, 0, 1)
    inline quaternion(): m_x(0.0f), m_y(0.0f), m_z(0.0f), m_w(1.0f) {}
    inline quaternion(real x, real y, real z, real w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

    // operators
    friend quaternion operator+(const quaternion& a, const quaternion& b) {
        return quaternion(a.x() + b.x(), a.y() + b.y(), a.z() + b.z(), a.w() + b.w());
    }
    quaternion operator-() {
        return quaternion(-m_x, -m_y, -m_z, -m_w);
    }
    friend quaternion operator-(const quaternion& a, const quaternion& b) {
        return quaternion(a.x() - b.x(), a.y() - b.y(), a.z() - b.z(), a.w() - b.w());
    }
    friend quaternion operator*(const quaternion& lhs, const quaternion& rhs) {
        return quaternion(
             rhs.x() * lhs.w() + rhs.y() * lhs.z() - rhs.z() * lhs.y() + rhs.w() * lhs.x(),
            -rhs.x() * lhs.z() + rhs.y() * lhs.w() + rhs.z() * lhs.x() + rhs.w() * lhs.y(),
             rhs.x() * lhs.y() - rhs.y() * lhs.x() + rhs.z() * lhs.w() + rhs.w() * lhs.z(),
            -rhs.x() * lhs.x() - rhs.y() * lhs.y() - rhs.z() * lhs.z() + rhs.w() * lhs.w()
        );
    }
    //friend quaternion operator*(const quaternion& lhs, const quaternion& rhs) {
    //    quaternion result;
    //    result.set_scalar(rhs.get_scalar() * lhs.get_scalar() -
    //        vec3::dot(rhs.get_vector(), lhs.get_vector()));
    //    result.set_vector((lhs.get_vector() * rhs.get_scalar()) +
    //        (rhs.get_vector() * lhs.get_scalar()) + vec3::cross(rhs.get_vector(), lhs.get_vector()));
    //    return result;
    //}
    friend vec3 operator*(const quaternion& q, const vec3& v) {
        return q.get_vector() * 2.0f * vec3::dot(q.get_vector(), v) +
            v * (q.get_scalar() * q.get_scalar() - vec3::dot(q.get_vector(), q.get_vector())) +
            vec3::cross(q.get_vector(), v) * 2.0f * q.get_scalar();
    }
    friend quaternion operator*(const quaternion& a, float b) {
        return quaternion(a.x() * b, a.y() * b, a.z() * b, a.w() * b);
    }
    quaternion operator-(const quaternion& q) {
        return quaternion(-q.x(), -q.y(), -q.z(), -q.w());
    }
    friend bool operator==(const quaternion& left, const quaternion& right) {
        return (fabsf(left.x() - right.x()) <= QUAT_EPSILON &&
            fabsf(left.y() - right.y()) <= QUAT_EPSILON &&
            fabsf(left.z() - right.z()) <= QUAT_EPSILON &&
            fabsf(left.w() - right.w()) <= QUAT_EPSILON);
    }
    friend bool operator!=(const quaternion& a, const quaternion& b) {
        return !(a == b);
    }
    quaternion mix(const quaternion& from, const quaternion& to, real t) {
        return from * (1.0f - t) + to * t;
    }
    static quaternion nlerp(const quaternion& from, const quaternion& to, float t) {
        return normalized(from + (to - from) * t);
    }
    friend quaternion operator^(const quaternion& q, float f) {
        float angle = 2.0f * acosf(q.get_scalar());
        vec3 axis = vec3::normalized(q.get_vector());
        float halfCos = cosf(f * angle * 0.5f);
        float halfSin = sinf(f * angle * 0.5f);
        return quaternion(axis.x() * halfSin,
            axis.y() * halfSin,
            axis.z() * halfSin,
            halfCos
        );
    }
    quaternion slerp(const quaternion& start, const quaternion& end, float t) {
        if (fabsf(dot(start, end)) > 1.0f - QUAT_EPSILON) {
            return nlerp(start, end, t);
        }
        quaternion delta = inverse(start) * end;
        return normalized((delta ^ t) * start);
    }
    static quaternion look_rotation(const vec3& direction, const vec3& up) {
        // Find orthonormal basis vectors
        vec3 f = vec3::normalized(direction); // Object Forward
        vec3 u = vec3::normalized(up); // Desired Up
        vec3 r = vec3::cross(u, f); // Object Right
        u = vec3::cross(f, r); // Object Up
        // From world forward to object forward
        quaternion worldToObject = from_to(vec3(0, 0, 1), f);
        // what direction is the new object up?
        vec3 objectUp = worldToObject * vec3(0, 1, 0);
        // From object up to desired up
        quaternion u2u = from_to(objectUp, u);
        // Rotate to forward direction first
        // then twist to correct up
        quaternion result = worldToObject * u2u;
        // Don't forget to normalize the result
        return normalized(result);
    }
    bool same_orientation(const quaternion& l, const quaternion& r) {
        return (fabsf(l.x() - r.x()) <= QUAT_EPSILON &&
                fabsf(l.y() - r.y()) <= QUAT_EPSILON &&
                fabsf(l.z() - r.z()) <= QUAT_EPSILON &&
                fabsf(l.w() - r.w()) <= QUAT_EPSILON) ||

                (fabsf(l.x() + r.x()) <= QUAT_EPSILON &&
                 fabsf(l.y() + r.y()) <= QUAT_EPSILON &&
                 fabsf(l.z() + r.z()) <= QUAT_EPSILON &&
                 fabsf(l.w() + r.w()) <= QUAT_EPSILON);
    }
    static float dot(const quaternion& a, const quaternion& b) {
        return a.x() * b.x() + a.y() * b.y() + a.z() * b.z() + a.w() * b.w();
    }
    float dot(const quaternion& a, const quaternion& b) {
        return a.x() * b.x() + a.y() * b.y() + a.z() * b.z() + a.w() * b.w();
    }
    float len(const quaternion& q) {
        float lenSq = q.x() * q.x() + q.y() * q.y() + q.z() * q.z() + q.w() * q.w();
        if (lenSq < QUAT_EPSILON) {
            return 0.0f;
        }
        return sqrtf(lenSq);
    }
    void normalize(quaternion& q) {
        float lenSq = q.x() * q.x() + q.y() * q.y() + q.z() * q.z() + q.w() * q.w();
        if (lenSq < QUAT_EPSILON) {
            return;
        }
        float i_len = 1.0f / sqrtf(lenSq);
        q.set_x(q.x() * i_len);
        q.set_y(q.y() * i_len);
        q.set_z(q.z() * i_len);
        q.set_w(q.w() * i_len);
    }
    static quaternion normalized(const quaternion& q) {
        float lenSq = q.x() * q.x() + q.y() * q.y() + q.z() * q.z() + q.w() * q.w();
        if (lenSq < QUAT_EPSILON) {
            return quaternion();
        }
        float il = 1.0f / sqrtf(lenSq); // il: inverse length
        return quaternion(q.x() * il, q.y() * il, q.z() * il, q.w() * il);
    }

    quaternion angle_axis(real angle, const vec3& axis) {
        vec3 norm = vec3::normalized(axis);
        // divind by 2 map the range of quaternion(720) to sin/cos(360)
        real s = sinf(angle * 0.5f);

        return quaternion(
            norm.x() * s,
            norm.y() * s,
            norm.z() * s,
            cosf(angle * 0.5f)
        );
    }
    quaternion conjugate(const quaternion& q) {
        return quaternion(
            -q.x(),
            -q.y(),
            -q.z(),
             q.w()
        );
    }
    static quaternion inverse(const quaternion& q) {
        float lenSq = q.x() * q.x() + q.y() * q.y() + q.z() * q.z() + q.w() * q.w();
        if (lenSq < QUAT_EPSILON) {
            return quaternion();
        }
        float recip = 1.0f / lenSq;
        return quaternion(-q.x() * recip,
            -q.y() * recip,
            -q.z() * recip,
             q.w() * recip
        );
    }

    static quaternion from_to(const vec3& from, const vec3& to) {
        vec3 f = vec3::normalized(from);
        vec3 t = vec3::normalized(to);
        if (f == t) {
            return quaternion();
        }
        else if (f == t * -1.0f) {
            vec3 ortho = vec3(1, 0, 0);
            if (fabsf(f.y()) < fabsf(f.x())) {
                ortho = vec3(0, 1, 0);
            }
            if (fabsf(f.z()) < fabs(f.y()) && fabs(f.z()) < fabsf(f.x())) {
                ortho = vec3(0, 0, 1);
            }
            vec3 axis = vec3::normalized(vec3::cross(f, ortho));
            return quaternion(axis.x(), axis.y(), axis.z(), 0);
        }
        vec3 half = vec3::normalized(f + t);
        vec3 axis = vec3::cross(f, half);
        return quaternion(axis.x(), axis.y(), axis.z(), vec3::dot(f, half));
    }
    static mat4 quat_to_mat4(const quaternion& q) {
        vec3 r = q * vec3(1, 0, 0);
        vec3 u = q * vec3(0, 1, 0);
        vec3 f = q * vec3(0, 0, 1);
        return mat4(r.x(), r.y(), r.z(), 0,
            u.x(), u.y(), u.z(), 0,
            f.x(), f.y(), f.z(), 0,
            0, 0, 0, 1
        );
    }
    static quaternion mat4ToQuat(const mat4& m) {
        vec3 up = vec3::normalized(vec3(m.get_up().x(), m.get_up().y(), m.get_up().z()));
        vec3 forward = vec3::normalized(
            vec3(m.get_forward().x(), m.get_forward().y(), m.get_forward().z()));
        vec3 right = vec3::cross(up, forward);
        up = vec3::cross(forward, right);
        return look_rotation(forward, up);
    }
    vec3 get_axis(const quaternion& quat) {
        return vec3::normalized(vec3(quat.x(), quat.y(), quat.z()));
    }
    float getAngle(const quaternion& quat) {
        return 2.0f * acosf(quat.w());
    }

    // getters
    inline real x() const { return m_x; }
    inline real y() const { return m_y; }
    inline real z() const { return m_z; }
    inline real w() const { return m_w; }
    inline real get_scalar() const { return m_w; }
    inline vec3 get_vector() const { return vec3(m_x, m_y, m_z); }
    inline quaternion get() const { return quaternion(m_x, m_y, m_z, m_w); }

    // setters
    inline void set_x(real x) { m_x = x; }
    inline void set_y(real y) { m_y = y; }
    inline void set_z(real z) { m_z = z; }
    inline void set_w(real w) { m_w = w; }
    inline void set_scalar(real scalar) { m_w = scalar; }
    inline void set_vector(const vec3& vec) { m_x = vec.x(); m_y = vec.y(); m_z = vec.z(); }
    inline void set(const quaternion& quat) {
        m_x = quat.x();
        m_y = quat.y();
        m_z = quat.z();
        m_w = quat.w();
    }
private:
    real m_x, m_y, m_z, m_w;
};