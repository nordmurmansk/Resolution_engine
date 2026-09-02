#pragma once

#include "Core/Math.h"
#include "Core/Types.h"

namespace res {

enum class CameraType {
    Perspective,
    Orthographic
};

class Camera {
public:
    Camera();
    
    // Позиция и ориентация
    Vec3 getPosition() const { return m_position; }
    void setPosition(const Vec3& pos) { m_position = pos; }
    
    Vec3 getRotation() const { return m_rotation; }
    void setRotation(const Vec3& rot) { m_rotation = rot; }
    
    Vec3 getForward() const;
    Vec3 getRight() const;
    Vec3 getUp() const;
    
    // Параметры камеры
    CameraType getType() const { return m_type; }
    void setType(CameraType type) { m_type = type; }
    
    f32 getFOV() const { return m_fov; }
    void setFOV(f32 fov) { m_fov = fov; }
    
    f32 getNearPlane() const { return m_near; }
    void setNearPlane(f32 near) { m_near = near; }
    
    f32 getFarPlane() const { return m_far; }
    void setFarPlane(f32 far) { m_far = far; }
    
    f32 getAspect() const { return m_aspect; }
    void setAspect(f32 aspect) { m_aspect = aspect; }
    
    f32 getOrthoSize() const { return m_orthoSize; }
    void setOrthoSize(f32 size) { m_orthoSize = size; }
    
    // Матрицы
    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;
    Mat4 getViewProjectionMatrix() const;
    
    // Управление от первого лица
    void lookAt(const Vec3& target);
    void rotate(f32 yaw, f32 pitch);
    void move(const Vec3& direction, f32 distance);
    
    // Raycast из камеры
    Ray screenPointToRay(f32 x, f32 y, f32 screenWidth, f32 screenHeight) const;
    
private:
    Vec3 m_position = Vec3(0, 0, 5);
    Vec3 m_rotation = Vec3(0, 0, 0); // Euler angles (pitch, yaw, roll)
    
    CameraType m_type = CameraType::Perspective;
    f32 m_fov = 90.0f * 3.14159f / 180.0f; // 90 degrees in radians
    f32 m_near = 0.1f;
    f32 m_far = 1000.0f;
    f32 m_aspect = 16.0f / 9.0f;
    f32 m_orthoSize = 10.0f;
};

} // namespace res
