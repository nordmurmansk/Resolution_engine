#pragma once

#include "Core/Types.h"
#include "Core/Math.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace res {

// Вперёд объявляем классы
class Entity;
class Component;
class Script;

// Компоненты сущности
enum class ComponentType {
    Transform,
    Mesh,
    Collider,
    Rigidbody,
    Light,
    Camera,
    Audio,
    Script,
    Trigger
};

// Типы сущностей
enum class EntityType {
    None,
    PlayerSpawn,
    Player,
    Enemy,
    Item,
    Trigger,
    Door,
    Light,
    Decal,
    Prop,
    Weapon
};

// Враждебность AI
enum class Faction {
    Friendly,
    Neutral,
    Hostile,
    AttackAll
};

// Тип оружия
enum class WeaponType {
    Hitscan,
    Projectile
};

// Тип снаряда
enum class ProjectileType {
    Bullet,
    Rocket,
    Grenade
};

// Базовый компонент
class Component {
public:
    virtual ~Component() = default;
    virtual ComponentType getType() const = 0;
    virtual void update(f32 deltaTime) {}
    
    Entity* getEntity() const { return m_entity; }
    void setEntity(Entity* entity) { m_entity = entity; }
    
protected:
    Entity* m_entity = nullptr;
};

// Компонент трансформации
class TransformComponent : public Component {
public:
    Vec3 position = Vec3(0, 0, 0);
    Vec3 rotation = Vec3(0, 0, 0); // Euler angles
    Vec3 scale = Vec3(1, 1, 1);
    
    ComponentType getType() const override { return ComponentType::Transform; }
    
    Mat4 getMatrix() const {
        Mat4 result = Mat4::translate(position);
        result = result * Quat::fromEuler(rotation.x, rotation.y, rotation.z).toMatrix();
        result = result * Mat4::scale(scale);
        return result;
    }
    
    Vec3 forward() const {
        return Quat::fromEuler(rotation.x, rotation.y, rotation.z).toMatrix() * Vec3(0, 0, -1);
    }
    
    Vec3 right() const {
        return Quat::fromEuler(rotation.x, rotation.y, rotation.z).toMatrix() * Vec3(1, 0, 0);
    }
    
    Vec3 up() const {
        return Quat::fromEuler(rotation.x, rotation.y, rotation.z).toMatrix() * Vec3(0, 1, 0);
    }
};

} // namespace res
