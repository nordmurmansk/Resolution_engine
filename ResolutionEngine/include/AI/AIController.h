#pragma once

#include "Core/Math.h"
#include "Core/Types.h"
#include "Entities/Components.h"
#include <vector>
#include <string>

namespace res {

class Entity;

// Состояния AI
enum class AIState {
    Idle,
    Patrol,
    Alert,
    Chase,
    Attack,
    Flee,
    Dead
};

// Конфигурация AI
struct AIConfig {
    f32 detectionRadius = 10.0f;
    f32 detectionAngle = 90.0f; // degrees
    f32 attackRange = 5.0f;
    f32 patrolSpeed = 2.0f;
    f32 chaseSpeed = 5.0f;
    f32 attackCooldown = 1.0f;
    f32 reactionTime = 0.5f;
    
    std::vector<Vec3> patrolPoints;
    bool loopPatrol = true;
    
    Faction faction = Faction::Hostile;
};

// Контроллер ИИ
class AIController {
public:
    AIController(Entity* entity, const AIConfig& config);
    ~AIController() = default;
    
    // Обновление
    void update(f32 deltaTime);
    
    // Получение состояния
    AIState getState() const { return m_state; }
    Entity* getTarget() const { return m_target; }
    
    // Управление состоянием
    void setState(AIState state);
    void setTarget(Entity* target);
    
    // Проверка видимости
    bool canSeeEntity(Entity* entity) const;
    bool isInDetectionRadius(Entity* entity) const;
    bool isInAttackRange(Entity* entity) const;
    
    // Действия
    void moveTo(const Vec3& position, f32 speed);
    void lookAt(const Vec3& target);
    bool attack();
    
    // Патрулирование
    void addPatrolPoint(const Vec3& point);
    void clearPatrolPoints();
    
    // Конфигурация
    const AIConfig& getConfig() const { return m_config; }
    void setConfig(const AIConfig& config) { m_config = config; }
    
    // Враждебность
    bool isHostileTo(Entity* entity) const;
    
private:
    Entity* m_entity;
    AIConfig m_config;
    AIState m_state = AIState::Idle;
    Entity* m_target = nullptr;
    
    u32 m_currentPatrolIndex = 0;
    f32 m_attackTimer = 0;
    f32 m_reactionTimer = 0;
    
    // Методы состояний
    void updateIdle(f32 deltaTime);
    void updatePatrol(f32 deltaTime);
    void updateAlert(f32 deltaTime);
    void updateChase(f32 deltaTime);
    void updateAttack(f32 deltaTime);
    
    // Поиск пути к цели
    Vec3 calculateMoveDirection() const;
};

} // namespace res
