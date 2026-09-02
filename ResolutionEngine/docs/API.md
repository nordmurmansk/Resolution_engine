# Resolution Engine API Documentation

## Содержание

1. [Ядро движка](#ядро-движка)
2. [Рендеринг](#рендеринг)
3. [Физика](#физика)
4. [Скриптинг (Lua)](#скриптинг-lua)
5. [Сущности](#сущности)
6. [ИИ](#ии)
7. [Оружие](#оружие)
8. [Редактор](#редактор)

---

## Ядро движка

### Logger

```cpp
#include "Core/Logger.h"

// Логирование разных уровней
LOG_DEBUG("Debug message");
LOG_INFO("Info message");
LOG_WARNING("Warning message");
LOG_ERROR("Error message");

// Установка уровня логирования
Logger::getInstance().setLogLevel(LogLevel::Debug);
```

### Математика

```cpp
#include "Core/Math.h"

// Векторы
Vec3 v(1.0f, 2.0f, 3.0f);
Vec3 normalized = v.normalized();
f32 length = v.length();
f32 dot = v1.dot(v2);
Vec3 cross = v1.cross(v2);

// Матрицы
Mat4 identity = Mat4::identity();
Mat4 projection = Mat4::perspective(fov, aspect, near, far);
Mat4 view = Mat4::lookAt(eye, target, up);
Mat4 translation = Mat4::translate(position);
Mat4 rotation = Mat4::rotate(angle, axis);
Mat4 scale = Mat4::scale(Vec3(2, 2, 2));

// Raycast
Ray ray(origin, direction);
Vec3 point = ray.pointAt(t);

// AABB
AABB bounds(min, max);
bool intersects = bounds.intersects(other);
bool contains = bounds.contains(point);
```

---

## Рендеринг

### Camera

```cpp
#include "Renderer/Camera.h"

Camera camera;
camera.setPosition(Vec3(0, 0, 5));
camera.setRotation(Vec3(0, 0, 0));
camera.setFOV(90.0f * PI / 180.0f);
camera.setNearPlane(0.1f);
camera.setFarPlane(1000.0f);
camera.setAspect(16.0f / 9.0f);

// Получение матриц
Mat4 viewMatrix = camera.getViewMatrix();
Mat4 projectionMatrix = camera.getProjectionMatrix();
Mat4 viewProj = camera.getViewProjectionMatrix();

// Направление камеры
Vec3 forward = camera.getForward();
Vec3 right = camera.getRight();
Vec3 up = camera.getUp();

// FPS управление
camera.rotate(yaw, pitch);
camera.move(direction, distance);

// Raycast из экрана
Ray ray = camera.screenPointToRay(x, y, screenWidth, screenHeight);
```

---

## Физика

### PhysicsWorld

```cpp
#include "Physics/PhysicsWorld.h"

PhysicsWorld& physics = PhysicsWorld::getInstance();

// Гравитация
physics.setGravity(Vec3(0, -9.81f, 0));

// Коллайдеры
BoxCollider* box = new BoxCollider(center, size);
SphereCollider* sphere = new SphereCollider(center, radius);
CapsuleCollider* capsule = new CapsuleCollider(center, radius, height);

// Регистрация коллайдера
physics.addCollider(collider);
physics.removeCollider(collider);

// Настройка триггера
collider->setTrigger(true);
bool isTrigger = collider->isTrigger();

// Raycast
Ray ray(origin, direction);
RaycastHit hit = physics.raycast(ray, maxDistance);

if (hit.hit) {
    Vec3 hitPoint = hit.point;
    Vec3 hitNormal = hit.normal;
    f32 distance = hit.distance;
    Entity* hitEntity = hit.entity;
}
```

---

## Скриптинг (Lua)

### ScriptManager

```cpp
#include "Scripting/ScriptManager.h"

ScriptManager& scripts = ScriptManager::getInstance();
scripts.initialize();

// Загрузка скрипта
scripts.loadScript("scripts/enemy_ai.lua");

// Перезагрузка всех скриптов
scripts.reloadAllScripts();

// Хуки карты
scripts.onMapStart();
scripts.onMapEnd();

// Хуки сущностей
scripts.onSpawn(entity);
scripts.onUse(entity, user);
scripts.onTriggerEnter(trigger, other);
scripts.onTriggerExit(trigger, other);
scripts.onUpdate(entity, deltaTime);
```

### Lua API (для скриптов)

```lua
-- Хуки карты
function OnMapStart()
    -- Вызывается при загрузке карты
end

function OnMapEnd()
    -- Вызывается при завершении карты
end

-- Хуки сущностей
function OnSpawn(entity)
    -- Вызывается при создании сущности
end

function OnUse(entity, user)
    -- Вызывается при взаимодействии
end

function OnTriggerEnter(entity, other)
    -- Вызывается при входе в триггер
end

function OnTriggerExit(entity, other)
    -- Вызывается при выходе из триггера
end

-- Доступ к свойствам сущности
local pos = entity:GetPosition()
entity:SetPosition(x, y, z)

local rot = entity:GetRotation()
entity:SetRotation(pitch, yaw, roll)

local health = entity:GetHealth()
entity:SetHealth(100)
entity:Damage(25)
entity:Kill()

-- Враждебность
entity:SetFaction("hostile") -- friendly, neutral, hostile, attack_all

-- Перезагрузка скриптов
reload_scripts()
```

---

## Сущности

### Entity

```cpp
#include "Entities/Entity.h"

// Создание сущности
Entity* entity = new Entity(id, EntityType::Enemy);
entity->setName("Enemy_01");

// Компоненты
TransformComponent* transform = entity->addComponent<TransformComponent>();
transform->position = Vec3(0, 0, 0);
transform->rotation = Vec3(0, 90, 0);
transform->scale = Vec3(1, 1, 1);

// Получение компонента
TransformComponent* t = entity->getComponent<TransformComponent>();
bool hasTransform = entity->hasComponent<TransformComponent>();

// Свойства
entity->setProperty("health", 100.0f);
entity->setProperty("faction", std::string("hostile"));

// Здоровье
f32 health = entity->getHealth();
entity->setHealth(100);
entity->damage(25);
entity->kill();
bool alive = entity->isAlive();

// Враждебность
entity->setFaction(Faction::Hostile);
Faction faction = entity->getFaction();
```

---

## ИИ

### AIController

```cpp
#include "AI/AIController.h"

// Конфигурация ИИ
AIConfig config;
config.detectionRadius = 10.0f;
config.detectionAngle = 90.0f;
config.attackRange = 5.0f;
config.patrolSpeed = 2.0f;
config.chaseSpeed = 5.0f;
config.patrolPoints = {Vec3(0,0,0), Vec3(10,0,0), Vec3(10,0,10)};
config.faction = Faction::Hostile;

// Создание контроллера
AIController* ai = new AIController(entity, config);

// Обновление (в игровом цикле)
ai->update(deltaTime);

// Состояния
AIState state = ai->getState();
ai->setState(AIState::Patrol);

// Цели
ai->setTarget(playerEntity);
Entity* target = ai->getTarget();

// Проверки
bool canSee = ai->canSeeEntity(player);
bool inRadius = ai->isInDetectionRadius(player);
bool inRange = ai->isInAttackRange(player);
bool hostile = ai->isHostileTo(player);

// Патрулирование
ai->addPatrolPoint(Vec3(5, 0, 5));
ai->clearPatrolPoints();
```

---

## Оружие

### Weapon

```cpp
#include "Weapons/Weapon.h"

// Конфигурация оружия
WeaponConfig config;
config.name = "Assault Rifle";
config.type = WeaponType::Hitscan;
config.damage = 25.0f;
config.range = 100.0f;
config.spread = 0.02f;
config.fireRate = 0.1f;
config.magazineSize = 30;
config.penetration = 1;
config.reloadTime = 2.0f;

// Создание оружия
Weapon* weapon = new Weapon(config);
weapon->setOwner(player);

// Стрельба
if (weapon->canFire()) {
    weapon->fire(origin, direction, shooter);
}

// Перезарядка
weapon->startReload();
bool reloading = weapon->isReloading();

// Патроны
i32 current = weapon->getCurrentAmmo();
i32 magazine = weapon->getMagazineAmmo();
i32 total = weapon->getTotalAmmo();
weapon->giveAmmo(30);

// Менеджер оружия
WeaponManager& wm = WeaponManager::getInstance();
wm.addWeapon(std::make_unique<Weapon>(config));
Weapon* current = wm.getCurrentWeapon();
wm.selectWeapon(0);
wm.nextWeapon();
wm.previousWeapon();
```

---

## Редактор

### MapSerializer

```cpp
#include "Editor/MapSerializer.h"

MapSerializer& serializer = MapSerializer::getInstance();

// Сохранение карты
MapData mapData;
mapData.name = "level_01";
mapData.skyboxPath = "textures/skybox/";
mapData.fog.color = Color(0.5f, 0.5f, 0.5f);
mapData.fog.density = 0.01f;

serializer.saveMap("data/maps/level_01.json", mapData);

// Загрузка карты
MapData loadedData;
if (serializer.loadMap("data/maps/level_01.json", loadedData)) {
    // Использовать загруженные данные
}
```

### Формат карты (JSON)

```json
{
  "name": "level_01",
  "description": "First level",
  "entities": [...],
  "brushes": [
    {
      "position": [0, 0, 0],
      "rotation": [0, 0, 0],
      "size": [10, 5, 10],
      "texture": "textures/wall.png",
      "isTrigger": false
    }
  ],
  "props": [
    {
      "position": [5, 0, 5],
      "rotation": [0, 45, 0],
      "scale": [1, 1, 1],
      "modelPath": "models/crate.glb"
    }
  ],
  "lights": [
    {
      "position": [0, 10, 0],
      "color": [1, 1, 0.9],
      "intensity": 1.0,
      "range": 20.0,
      "isDirectional": false
    }
  ],
  "skybox": "textures/skybox/",
  "fog": {
    "color": [0.5, 0.5, 0.5],
    "density": 0.01,
    "startDistance": 10,
    "endDistance": 100
  },
  "mapScript": "scripts/level_01.lua"
}
```

---

## Команды запуска

```bash
# Запуск игры
ResolutionEngine

# Запуск редактора
ResolutionEngine -editor

# Загрузка проекта
ResolutionEngine -project MyGame

# Загрузка карты
ResolutionEngine -map level_01

# Создание нового проекта
ResolutionEngine -new_project MyGame

# Помощь
ResolutionEngine -help
```
