#pragma once

#include "Entities/Components.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <any>

namespace res {

class Script;

// Базовый класс сущности
class Entity {
public:
    Entity(u32 id, EntityType type = EntityType::None);
    virtual ~Entity();
    
    u32 getId() const { return m_id; }
    EntityType getType() const { return m_type; }
    void setType(EntityType type) { m_type = type; }
    
    std::string getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    // Компоненты
    template<typename T>
    T* addComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        T* component = new T();
        component->setEntity(this);
        m_components[component->getType()] = std::unique_ptr<Component>(component);
        return component;
    }
    
    template<typename T>
    T* getComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        auto it = m_components.find(T().getType());
        if (it != m_components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }
    
    template<typename T>
    bool hasComponent() const {
        return m_components.find(T().getType()) != m_components.end();
    }
    
    TransformComponent* getTransform() const {
        return getComponent<TransformComponent>();
    }
    
    // Свойства (для скриптов и редактора)
    void setProperty(const std::string& key, const std::any& value);
    std::any getProperty(const std::string& key) const;
    bool hasProperty(const std::string& key) const;
    
    const std::unordered_map<std::string, std::any>& getProperties() const { return m_properties; }
    
    // Скрипты
    void addScript(std::shared_ptr<Script> script);
    const std::vector<std::shared_ptr<Script>>& getScripts() const { return m_scripts; }
    
    // Здоровье и урон
    f32 getMaxHealth() const { return m_maxHealth; }
    f32 getHealth() const { return m_health; }
    void setHealth(f32 health);
    void damage(f32 amount);
    bool isAlive() const { return m_health > 0; }
    void kill();
    
    // Враждебность
    Faction getFaction() const { return m_faction; }
    void setFaction(Faction faction) { m_faction = faction; }
    
    // Обновление
    virtual void update(f32 deltaTime);
    
    // Серииализация/десериализация
    virtual void serialize(class MapSerializer& serializer) const;
    virtual void deserialize(const class MapDeserializer& deserializer);
    
protected:
    u32 m_id;
    EntityType m_type = EntityType::None;
    std::string m_name;
    
    std::unordered_map<ComponentType, std::unique_ptr<Component>> m_components;
    std::vector<std::shared_ptr<Script>> m_scripts;
    std::unordered_map<std::string, std::any> m_properties;
    
    f32 m_maxHealth = 100.0f;
    f32 m_health = 100.0f;
    Faction m_faction = Faction::Neutral;
    
    bool m_isDead = false;
};

} // namespace res
