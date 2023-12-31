#pragma once

#include "vec4.hpp"
#include "vec3.hpp"
#include "defines.hpp"

#include <iostream>
#include <assert.h>

#define MAT4_DOT(a_row, b_col) \
    lhs[0 * 4 + a_row] * rhs[b_col * 4 + 0] + \
    lhs[1 * 4 + a_row] * rhs[b_col * 4 + 1] + \
    lhs[2 * 4 + a_row] * rhs[b_col * 4 + 2] + \
    lhs[3 * 4 + a_row] * rhs[b_col * 4 + 3]

#define MAT4_VEC4_DOT(m_row, x, y, z, w) \
 x * mat[0 * 4 + m_row] + \
 y * mat[1 * 4 + m_row] + \
 z * mat[2 * 4 + m_row] + \
 w * mat[3 * 4 + m_row]

#define MAT4_SWAP(x, y) \
 {float t = x; x = y; y = t; }

#define MAT4_3X3MINOR(x, c0, c1, c2, r0, r1, r2) \
 (x[c0*4+r0]*(x[c1*4+r1]*x[c2*4+r2]-x[c1*4+r2]* \
 x[c2*4+r1])-x[c1*4+r0]*(x[c0*4+r1]*x[c2*4+r2]- \
 x[c0*4+r2]*x[c2*4+r1])+x[c2*4+r0]*(x[c0*4+r1]* \
 x[c1*4+r2]-x[c0*4+r2]*x[c1*4+r1]))

//============================== mat4 =========================================//
// column major matrix                                                         //
// right_________________                                                      //
// up_______________     |                                                     //
// forward______    |    |                                                     //
// position_    |   |    |                                                     //
//          |   |   |    |                                                     //
//         {xx, yx, zx, wx}                                                    //
//         {xy, yy, zy, wy}                                                    //
//         {xz, yz, zz, wz}                                                    //
//         {xw, yw, zw, ww}                                                    //
// column1 -> m_right                                                          //
// column2 -> m_up                                                             //
// column3 -> m_forward                                                        //
// column4 -> m_position                                                       //
// how data is stored                                                          //
//  {xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wx, wy, wz, ww}       //
//  {m_right, m_up, m_forward, m_position}                                     //
//=============================================================================//
class mat4 {
public:
    inline mat4() :
        m_right    (1.0f, 0.0f, 0.0f, 0.0f),
        m_up	   (0.0f, 1.0f, 0.0f, 0.0f),
        m_forward  (0.0f, 0.0f, 1.0f, 0.0f),
        m_position (0.0f, 0.0f, 0.0f, 1.0f) 
    {}
    inline mat4(
        float _00, float _01, float _02, float _03,
        float _10, float _11, float _12, float _13,
        float _20, float _21, float _22, float _23,
        float _30, float _31, float _32, float _33) :
        m_right	   (_00, _01, _02, _03),
        m_up	   (_10, _11, _12, _13),
        m_forward  (_20, _21, _22, _23),
        m_position (_30, _31, _32, _33) 
    {}
    inline mat4(const vec4& right, const vec4& up, const vec4& forward, const vec4& position) :
        m_right(right),
        m_up(up),
        m_forward(forward),
        m_position(position)
    {}
    // operators
    friend bool operator==(const mat4& lhs, const mat4& rhs) {
        for (int i = 0; i < 16; ++i) {
            if (fabsf(lhs[i] - rhs[i]) > MAT4_EPSILON) {
                return false;
            }
        }
        return true;
    }
    friend bool operator!=(const mat4& a, const mat4& b) {
        return !(a == b);
    }
    real operator[](int i) const {
        assert(i >= 0 && i <= 15);
        if (i >= 0 && i <= 3) return  m_right[i];
        if (i >= 4 && i <= 7) return  m_up[i - 4];
        if (i >= 8 && i <= 11) return m_forward[i - 8];
        if (i >= 12 && i <= 15) return m_position[i - 12];
    }
    friend mat4 operator+(const mat4& a, const mat4& b) {
        return mat4(
            a.xx() + b.xx(), a.xy() + b.xy(), a.xz() + b.xz(), a.xw() + b.xw(),
            a.yx() + b.yx(), a.yy() + b.yy(), a.yz() + b.yz(), a.yw() + b.yw(),
            a.zx() + b.zx(), a.zy() + b.zy(), a.zz() + b.zz(), a.zw() + b.zw(),
            a.wx() + b.wx(), a.wy() + b.wy(), a.wz() + b.wz(), a.ww() + b.ww()
        );
    }
    friend mat4 operator*(const mat4& m, float f) {
        return mat4(
            m.xx() * f, m.xy() * f, m.xz() * f, m.xw() * f,
            m.yx() * f, m.yy() * f, m.yz() * f, m.yw() * f,
            m.zx() * f, m.zy() * f, m.zz() * f, m.zw() * f,
            m.wx() * f, m.wy() * f, m.wz() * f, m.ww() * f
        );
    }
    friend mat4 operator*(const mat4& lhs, const mat4& rhs) {
        return mat4(
            MAT4_DOT(0, 0), MAT4_DOT(1, 0), MAT4_DOT(2, 0), MAT4_DOT(3, 0),//Col 0
            MAT4_DOT(0, 1), MAT4_DOT(1, 1), MAT4_DOT(2, 1), MAT4_DOT(3, 1),//Col 1
            MAT4_DOT(0, 2), MAT4_DOT(1, 2), MAT4_DOT(2, 2), MAT4_DOT(3, 2),//Col 2
            MAT4_DOT(0, 3), MAT4_DOT(1, 3), MAT4_DOT(2, 3), MAT4_DOT(3, 3) //Col 3
        );
    }
    friend vec4 operator*(const mat4& mat, const vec4& vec) {
        return vec4(
            MAT4_VEC4_DOT(0, vec.x(), vec.y(), vec.z(), vec.w()),
            MAT4_VEC4_DOT(1, vec.x(), vec.y(), vec.z(), vec.w()),
            MAT4_VEC4_DOT(2, vec.x(), vec.y(), vec.z(), vec.w()),
            MAT4_VEC4_DOT(3, vec.x(), vec.y(), vec.z(), vec.w())
        );
    }
    vec3 transform_vector(const mat4& mat, const vec3& vec) {
        return vec3(
            MAT4_VEC4_DOT(0, vec.x(), vec.y(), vec.z(), 0.0f),
            MAT4_VEC4_DOT(1, vec.x(), vec.y(), vec.z(), 0.0f),
            MAT4_VEC4_DOT(2, vec.x(), vec.y(), vec.z(), 0.0f)
        );
    }
    vec3 transform_point(const mat4& mat, const vec3& vec) {
        return vec3(
            MAT4_VEC4_DOT(0, vec.x(), vec.y(), vec.z(), 1.0f),
            MAT4_VEC4_DOT(1, vec.x(), vec.y(), vec.z(), 1.0f),
            MAT4_VEC4_DOT(2, vec.x(), vec.y(), vec.z(), 1.0f)
        );
    }
    // overload function for transforming a point, the w component of vec4 is taken separately
    // it takes a reference of the w component and updates it wiht the calculated value
    vec3 transform_point(const mat4& mat, const vec3& vec, real& w) {
        float _w = w;
        w = MAT4_VEC4_DOT(3, vec.x(), vec.y(), vec.z(), _w);
        return vec3(
            MAT4_VEC4_DOT(0, vec.x(), vec.y(), vec.z(), _w),
            MAT4_VEC4_DOT(1, vec.x(), vec.y(), vec.z(), _w),
            MAT4_VEC4_DOT(2, vec.x(), vec.y(), vec.z(), _w)
        );
    }
    static mat4 transpose(const mat4& mat) {
        mat4 temp;

        temp.set_at_index(0, mat.get_at_index(0));
        temp.set_at_index(1, mat.get_at_index(4));
        temp.set_at_index(2, mat.get_at_index(8));
        temp.set_at_index(3, mat.get_at_index(12));
        
        temp.set_at_index(4, mat.get_at_index(1));
        temp.set_at_index(5, mat.get_at_index(5));
        temp.set_at_index(6, mat.get_at_index(9));
        temp.set_at_index(7, mat.get_at_index(13));

        temp.set_at_index(8, mat.get_at_index(2));
        temp.set_at_index(9, mat.get_at_index(6));
        temp.set_at_index(10, mat.get_at_index(10));
        temp.set_at_index(11, mat.get_at_index(14));

        temp.set_at_index(12, mat.get_at_index(3));
        temp.set_at_index(13, mat.get_at_index(7));
        temp.set_at_index(14, mat.get_at_index(11));
        temp.set_at_index(15, mat.get_at_index(15));

        return temp;
    }
    void transposed() {
        mat4 temp = mat4(m_right, m_up, m_forward, m_position);
        set_mat(
            temp.xx(), temp.yx(), temp.zx(), temp.wx(),
            temp.xy(), temp.yy(), temp.zy(), temp.wy(),
            temp.xz(), temp.yz(), temp.zz(), temp.wz(),
            temp.xw(), temp.yw(), temp.zw(), temp.ww()
        );
    }
    static float determinant(const mat4& m) {
        return m[0] * MAT4_3X3MINOR(m, 1, 2, 3, 1, 2, 3)
             - m[4] * MAT4_3X3MINOR(m, 0, 2, 3, 1, 2, 3)
             + m[8] * MAT4_3X3MINOR(m, 0, 1, 3, 1, 2, 3)
            - m[12] * MAT4_3X3MINOR(m, 0, 1, 2, 1, 2, 3);
    }
    static mat4 adjugate(const mat4& mat) {
        //Cof (M[i, j]) = Minor(M[i, j]] * pow(-1, i + j)
        mat4 cofactor;
        cofactor.set_at_index(0 ,  MAT4_3X3MINOR(mat, 1, 2, 3, 1, 2, 3));
        cofactor.set_at_index(1 , -MAT4_3X3MINOR(mat, 1, 2, 3, 0, 2, 3));
        cofactor.set_at_index(2 ,  MAT4_3X3MINOR(mat, 1, 2, 3, 0, 1, 3));
        cofactor.set_at_index(3 , -MAT4_3X3MINOR(mat, 1, 2, 3, 0, 1, 2));
        cofactor.set_at_index(4 , -MAT4_3X3MINOR(mat, 0, 2, 3, 1, 2, 3));
        cofactor.set_at_index(5 ,  MAT4_3X3MINOR(mat, 0, 2, 3, 0, 2, 3));
        cofactor.set_at_index(6 , -MAT4_3X3MINOR(mat, 0, 2, 3, 0, 1, 3));
        cofactor.set_at_index(7 ,  MAT4_3X3MINOR(mat, 0, 2, 3, 0, 1, 2));
        cofactor.set_at_index(8 ,  MAT4_3X3MINOR(mat, 0, 1, 3, 1, 2, 3));
        cofactor.set_at_index(9 , -MAT4_3X3MINOR(mat, 0, 1, 3, 0, 2, 3));
        cofactor.set_at_index(10,  MAT4_3X3MINOR(mat, 0, 1, 3, 0, 1, 3));
        cofactor.set_at_index(11, -MAT4_3X3MINOR(mat, 0, 1, 3, 0, 1, 2));
        cofactor.set_at_index(12, -MAT4_3X3MINOR(mat, 0, 1, 2, 1, 2, 3));
        cofactor.set_at_index(13,  MAT4_3X3MINOR(mat, 0, 1, 2, 0, 2, 3));
        cofactor.set_at_index(14, -MAT4_3X3MINOR(mat, 0, 1, 2, 0, 1, 3));
        cofactor.set_at_index(15,  MAT4_3X3MINOR(mat, 0, 1, 2, 0, 1, 2));
        return transpose(cofactor);
    }
    static mat4 inverse(const mat4& mat) {
        float det = determinant(mat);

        if (det == 0.0f) {
            std::cout << " Matrix determinant is 0\n";
            return mat4();
        }
        mat4 adj = adjugate(mat);
        return adj * (1.0f / det);
    }
    void invert(mat4& mat) {
        float det = determinant(mat);
        if (det == 0.0f) {
            std::cout << "Matrix determinant is 0\n";
            mat = mat4();
            return;
        }
        mat = adjugate(mat) * (1.0f / det);
    }
    mat4 frustum(real l, real r, real b,
        real t, real n, real f) {
        if (l == r || t == b || n == f) {
            std::cout << "Invalid frustum\n";
            return mat4(); // Error
        }
        return mat4(
            (2.0f * n) / (r - l), 0, 0, 0,
            0, (2.0f * n) / (t - b), 0, 0,
            (r + l) / (r - l), (t + b) / (t - b), (-(f + n)) / (f - n), -1,
            0, 0, (-2 * f * n) / (f - n), 0
        );
    }
    mat4 perspective(float fov, float aspect, float n, float f) {
        float ymax = n * tanf(fov * PI / 360.0f);
        float xmax = ymax * aspect;
        return frustum(-xmax, xmax, -ymax, ymax, n, f);
    }
    mat4 ortho(float l, float r, float b, float t,
        float n, float f) {
        if (l == r || t == b || n == f) {
            return mat4(); // Error
        }
        return mat4(
            2.0f / (r - l), 0, 0, 0,
            0, 2.0f / (t - b), 0, 0,
            0, 0, -2.0f / (f - n), 0,
            -((r + l) / (r - l)), -((t + b) / (t - b)), -((f + n) / (f - n)), 1
        );
    }
    mat4 lookAt(const vec3& position, const vec3& target,
        const vec3& up) {
        vec3 f = vec3::normalized(target - position) * -1.0f;
        vec3 r = vec3::cross(up, f); // Right handed
        if (r == vec3(0, 0, 0)) {
            return mat4(); // Error
        }
        r.normalize();
        vec3 u = vec3::normalized(vec3::cross(f, r)); // Right handed
        vec3 t = vec3(
            -vec3::dot(r, position),
            -vec3::dot(u, position),
            -vec3::dot(f, position)
        );
        return mat4(
            // Transpose upper 3x3 matrix to invert it
            r.x(), u.x(), f.x(), 0,
            r.y(), u.y(), f.y(), 0,
            r.z(), u.z(), f.z(), 0,
            t.x(), t.y(), t.z(), 1
        );
    }


    // getters
    vec4 get_right()	const { return m_right; }
    vec4 get_up()		const { return m_up; }
    vec4 get_forward()  const { return m_forward; }
    vec4 get_position() const { return m_position; }
    vec4 get_right()	{ return m_right; }
    vec4 get_up()		{ return m_up; }
    vec4 get_forward()  { return m_forward; }
    vec4 get_position() { return m_position; }
    real get_at_index(int index) const {
        if (index == 0)       return m_right.x();
        else if (index == 1)  return m_right.y();
        else if (index == 2)  return m_right.z();
        else if (index == 3)  return m_right.w();

        else if (index == 4)  return m_up.x();
        else if (index == 5)  return m_up.y();
        else if (index == 6)  return m_up.z();
        else if (index == 7)  return m_up.w();

        else if (index == 8)  return m_forward.x();
        else if (index == 9)  return m_forward.y();
        else if (index == 10) return m_forward.z();
        else if (index == 11) return m_forward.w();

        else if (index == 12) return m_position.x();
        else if (index == 13) return m_position.y();
        else if (index == 14) return m_position.z();
        else if (index == 15) return m_position.w();
        else assert(false); // invalid index value
    }
    //
    real xx() const { return m_right.x(); }
    real xy() const { return m_right.y(); }
    real xz() const { return m_right.z(); }
    real xw() const { return m_right.w(); }
    //
    real yx() const { return m_up.x(); }
    real yy() const { return m_up.y(); }
    real yz() const { return m_up.z(); }
    real yw() const { return m_up.w(); }
    //
    real zx() const { return m_forward.x(); }
    real zy() const { return m_forward.y(); }
    real zz() const { return m_forward.z(); }
    real zw() const { return m_forward.w(); }
    //
    real wx() const { return m_position.x(); }
    real wy() const { return m_position.y(); }
    real wz() const { return m_position.z(); }
    real ww() const { return m_position.w(); }

    // setter
    void set_xx(real value) { m_right.set_x(value); }		// 0
    void set_xy(real value) { m_right.set_y(value); }		// 1
    void set_xz(real value) { m_right.set_z(value); }		// 2
    void set_xw(real value) { m_right.set_w(value); }		// 3
    //														 
    void set_yx(real value) { m_up.set_x(value); }			// 4
    void set_yy(real value) { m_up.set_y(value); }			// 5
    void set_yz(real value) { m_up.set_z(value); }			// 6
    void set_yw(real value) { m_up.set_w(value); }			// 7
    //														 
    void set_zx(real value) { m_forward.set_x(value); }		// 8
    void set_zy(real value) { m_forward.set_y(value); }		// 9
    void set_zz(real value) { m_forward.set_z(value); }		// 10
    void set_zw(real value) { m_forward.set_w(value); }		// 11
    //														 
    void set_wx(real value) { m_position.set_x(value); }	// 12
    void set_wy(real value) { m_position.set_y(value); }	// 13
    void set_wz(real value) { m_position.set_z(value); }	// 14
    void set_ww(real value) { m_position.set_w(value); }	// 15

    void set_mat(
        float _00, float _01, float _02, float _03,
        float _10, float _11, float _12, float _13,
        float _20, float _21, float _22, float _23,
        float _30, float _31, float _32, float _33) {
        m_right = vec4(_00, _01, _02, _03);
        m_up = vec4(_10, _11, _12, _13);
        m_forward = vec4(_20, _21, _22, _23);
        m_position = vec4(_30, _31, _32, _33);
    }

    void set_at_index(int index, real value) {
        if (index == 0)  set_xx(value);
        else if (index == 1)  set_xy(value);
        else if (index == 2)  set_xz(value);
        else if (index == 3)  set_xw(value);

        else if (index == 4)  set_yx(value);
        else if (index == 5)  set_yy(value);
        else if (index == 6)  set_yz(value);
        else if (index == 7)  set_yw(value);

        else if (index == 8)  set_zx(value);
        else if (index == 9)  set_zy(value);
        else if (index == 10) set_zz(value);
        else if (index == 11) set_zw(value);

        else if (index == 12) set_wx(value);
        else if (index == 13) set_wy(value);
        else if (index == 14) set_wz(value);
        else if (index == 15) set_ww(value);
        else assert(false); // invalid index value
    }
private:
    vec4 m_right;
    vec4 m_up;
    vec4 m_forward;
    vec4 m_position;
};