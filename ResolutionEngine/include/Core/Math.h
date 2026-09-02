#pragma once

#include "Core/Types.h"
#include <cmath>

namespace res {

// Вектор 3
struct Vec3 {
    f32 x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(f32 vx, f32 vy, f32 vz) : x(vx), y(vy), z(vz) {}
    
    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(f32 s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(f32 s) const { return Vec3(x / s, y / s, z / s); }
    
    Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    
    f32 dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    f32 length() const { return std::sqrt(x * x + y * y + z * z); }
    f32 lengthSq() const { return x * x + y * y + z * z; }
    
    Vec3 normalized() const {
        f32 len = length();
        return len > 0 ? *this / len : Vec3();
    }
    
    static Vec3 zero() { return Vec3(0, 0, 0); }
    static Vec3 one() { return Vec3(1, 1, 1); }
    static Vec3 up() { return Vec3(0, 1, 0); }
    static Vec3 forward() { return Vec3(0, 0, -1); }
    static Vec3 right() { return Vec3(1, 0, 0); }
};

// Вектор 2
struct Vec2 {
    f32 x, y;
    
    Vec2() : x(0), y(0) {}
    Vec2(f32 vx, f32 vy) : x(vx), y(vy) {}
    
    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(f32 s) const { return Vec2(x * s, y * s); }
    
    f32 length() const { return std::sqrt(x * x + y * y); }
    Vec2 normalized() const {
        f32 len = length();
        return len > 0 ? *this / len : Vec2();
    }
};

// Вектор 4
struct Vec4 {
    f32 x, y, z, w;
    
    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(f32 vx, f32 vy, f32 vz, f32 vw) : x(vx), y(vy), z(vz), w(vw) {}
    
    Vec4 operator+(const Vec4& other) const { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vec4 operator*(f32 s) const { return Vec4(x * s, y * s, z * s, w * s); }
};

// Матрица 4x4
struct Mat4 {
    f32 m[16];
    
    Mat4() {
        for (int i = 0; i < 16; ++i) m[i] = 0;
        m[0] = m[5] = m[10] = m[15] = 1; // Identity
    }
    
    static Mat4 identity() { return Mat4(); }
    
    static Mat4 perspective(f32 fov, f32 aspect, f32 near, f32 far) {
        Mat4 result;
        f32 tanHalfFov = std::tan(fov / 2.0f);
        
        result.m[0] = 1.0f / (aspect * tanHalfFov);
        result.m[5] = 1.0f / tanHalfFov;
        result.m[10] = -(far + near) / (far - near);
        result.m[11] = -1.0f;
        result.m[14] = -(2.0f * far * near) / (far - near);
        result.m[15] = 0;
        
        return result;
    }
    
    static Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
        Mat4 result;
        Vec3 f = (target - eye).normalized();
        Vec3 r = f.cross(up).normalized();
        Vec3 u = r.cross(f);
        
        result.m[0] = r.x; result.m[1] = u.x; result.m[2] = -f.x;
        result.m[4] = r.y; result.m[5] = u.y; result.m[6] = -f.y;
        result.m[8] = r.z; result.m[9] = u.z; result.m[10] = -f.z;
        result.m[12] = -r.dot(eye);
        result.m[13] = -u.dot(eye);
        result.m[14] = f.dot(eye);
        
        return result;
    }
    
    static Mat4 translate(const Vec3& t) {
        Mat4 result;
        result.m[12] = t.x;
        result.m[13] = t.y;
        result.m[14] = t.z;
        return result;
    }
    
    static Mat4 rotate(f32 angle, const Vec3& axis) {
        Mat4 result;
        f32 c = std::cos(angle);
        f32 s = std::sin(angle);
        f32 oc = 1 - c;
        
        result.m[0] = oc * axis.x * axis.x + c;
        result.m[1] = oc * axis.x * axis.y - axis.z * s;
        result.m[2] = oc * axis.z * axis.x + axis.y * s;
        result.m[4] = oc * axis.x * axis.y + axis.z * s;
        result.m[5] = oc * axis.y * axis.y + c;
        result.m[6] = oc * axis.y * axis.z - axis.x * s;
        result.m[8] = oc * axis.z * axis.x - axis.y * s;
        result.m[9] = oc * axis.y * axis.z + axis.x * s;
        result.m[10] = oc * axis.z * axis.z + c;
        
        return result;
    }
    
    static Mat4 scale(const Vec3& s) {
        Mat4 result;
        result.m[0] = s.x;
        result.m[5] = s.y;
        result.m[10] = s.z;
        return result;
    }
    
    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i * 4 + j] = 0;
                for (int k = 0; k < 4; ++k) {
                    result.m[i * 4 + j] += m[k * 4 + j] * other.m[i * 4 + k];
                }
            }
        }
        return result;
    }
};

// AABB (Axis-Aligned Bounding Box)
struct AABB {
    Vec3 min, max;
    
    AABB() : min(0, 0, 0), max(0, 0, 0) {}
    AABB(const Vec3& mn, const Vec3& mx) : min(mn), max(mx) {}
    
    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
    
    bool contains(const Vec3& point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }
    
    Vec3 center() const { return (min + max) * 0.5f; }
    Vec3 size() const { return max - min; }
};

// Ray
struct Ray {
    Vec3 origin;
    Vec3 direction;
    
    Ray() : origin(0, 0, 0), direction(0, 0, -1) {}
    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d.normalized()) {}
    
    Vec3 pointAt(f32 t) const { return origin + direction * t; }
};

// Quaternion
struct Quat {
    f32 x, y, z, w;
    
    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(f32 vx, f32 vy, f32 vz, f32 vw) : x(vx), y(vy), z(vz), w(vw) {}
    
    static Quat fromEuler(f32 pitch, f32 yaw, f32 roll) {
        f32 cp = std::cos(pitch * 0.5f);
        f32 sp = std::sin(pitch * 0.5f);
        f32 cy = std::cos(yaw * 0.5f);
        f32 sy = std::sin(yaw * 0.5f);
        f32 cr = std::cos(roll * 0.5f);
        f32 sr = std::sin(roll * 0.5f);
        
        return Quat(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }
    
    Mat4 toMatrix() const {
        Mat4 result;
        f32 xx = x * x, yy = y * y, zz = z * z;
        f32 xy = x * y, xz = x * z, yz = y * z;
        f32 wx = w * x, wy = w * y, wz = w * z;
        
        result.m[0] = 1 - 2 * (yy + zz);
        result.m[1] = 2 * (xy + wz);
        result.m[2] = 2 * (xz - wy);
        result.m[4] = 2 * (xy - wz);
        result.m[5] = 1 - 2 * (xx + zz);
        result.m[6] = 2 * (yz + wx);
        result.m[8] = 2 * (xz + wy);
        result.m[9] = 2 * (yz - wx);
        result.m[10] = 1 - 2 * (xx + yy);
        
        return result;
    }
};

// Цвет (RGBA)
struct Color {
    f32 r, g, b, a;
    
    Color() : r(1), g(1), b(1), a(1) {}
    Color(f32 vr, f32 vg, f32 vb, f32 va = 1) : r(vr), g(vg), b(vb), a(va) {}
    
    static Color white() { return Color(1, 1, 1, 1); }
    static Color black() { return Color(0, 0, 0, 1); }
    static Color red() { return Color(1, 0, 0, 1); }
    static Color green() { return Color(0, 1, 0, 1); }
    static Color blue() { return Color(0, 0, 1, 1); }
};

} // namespace res
