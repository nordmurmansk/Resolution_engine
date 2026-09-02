#pragma once

#include "Core/Math.h"
#include "Core/Types.h"
#include <vector>
#include <memory>
#include <functional>

namespace res {

class Collider;
class Entity;

// Результат рейкаста
struct RaycastHit {
    bool hit = false;
    f32 distance = 0;
    Vec3 point;
    Vec3 normal;
    Entity* entity = nullptr;
    Collider* collider = nullptr;
};

// Типы коллайдеров
enum class ColliderType {
    Box,
    Sphere,
    Capsule,
    Mesh
};

// Базовый коллайдер
class Collider {
public:
    virtual ~Collider() = default;
    virtual ColliderType getType() const = 0;
    virtual bool raycast(const Ray& ray, RaycastHit& hit) const = 0;
    virtual AABB getBounds() const = 0;
    
    void setEntity(Entity* entity) { m_entity = entity; }
    Entity* getEntity() const { return m_entity; }
    
    bool isTrigger() const { return m_isTrigger; }
    void setTrigger(bool trigger) { m_isTrigger = trigger; }
    
protected:
    Entity* m_entity = nullptr;
    bool m_isTrigger = false;
};

// Box Collider
class BoxCollider : public Collider {
public:
    BoxCollider(const Vec3& center, const Vec3& size);
    
    ColliderType getType() const override { return ColliderType::Box; }
    bool raycast(const Ray& ray, RaycastHit& hit) const override;
    AABB getBounds() const override;
    
    Vec3 getCenter() const { return m_center; }
    Vec3 getSize() const { return m_size; }
    
private:
    Vec3 m_center;
    Vec3 m_size;
};

// Sphere Collider
class SphereCollider : public Collider {
public:
    SphereCollider(const Vec3& center, f32 radius);
    
    ColliderType getType() const override { return ColliderType::Sphere; }
    bool raycast(const Ray& ray, RaycastHit& hit) const override;
    AABB getBounds() const override;
    
    Vec3 getCenter() const { return m_center; }
    f32 getRadius() const { return m_radius; }
    
private:
    Vec3 m_center;
    f32 m_radius;
};

// Capsule Collider (для игрока)
class CapsuleCollider : public Collider {
public:
    CapsuleCollider(const Vec3& center, f32 radius, f32 height);
    
    ColliderType getType() const override { return ColliderType::Capsule; }
    bool raycast(const Ray& ray, RaycastHit& hit) const override;
    AABB getBounds() const override;
    
private:
    Vec3 m_center;
    f32 m_radius;
    f32 m_height;
};

// Физический мир
class PhysicsWorld {
public:
    static PhysicsWorld& getInstance() {
        static PhysicsWorld instance;
        return instance;
    }
    
    // Гравитация
    Vec3 getGravity() const { return m_gravity; }
    void setGravity(const Vec3& gravity) { m_gravity = gravity; }
    
    // Регистрация коллайдеров
    void addCollider(Collider* collider);
    void removeCollider(Collider* collider);
    
    // Raycast
    RaycastHit raycast(const Ray& ray, f32 maxDistance = 1000.0f) const;
    RaycastHit raycast(const Ray& ray, u32 layerMask, f32 maxDistance = 1000.0f) const;
    
    // Проверка триггеров
    void checkTriggers();
    
    // Обновление
    void update(f32 deltaTime);
    
private:
    PhysicsWorld() : m_gravity(0, -9.81f, 0) {}
    
    Vec3 m_gravity;
    std::vector<Collider*> m_colliders;
    std::vector<Collider*> m_triggers;
};

} // namespace res
