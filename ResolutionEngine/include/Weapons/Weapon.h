#pragma once

#include "Core/Math.h"
#include "Core/Types.h"
#include "Entities/Components.h"
#include <string>

namespace res {

class Entity;

// Конфигурация оружия
struct WeaponConfig {
    std::string name = "Weapon";
    WeaponType type = WeaponType::Hitscan;
    ProjectileType projectileType = ProjectileType::Bullet;
    
    f32 damage = 10.0f;
    f32 range = 100.0f;
    f32 spread = 0.0f;
    f32 fireRate = 0.1f; // seconds between shots
    i32 magazineSize = 30;
    i32 penetration = 0;
    
    f32 reloadTime = 2.0f;
    f32 drawTime = 0.5f;
    
    std::string modelPath;
    std::string fireSound;
    std::string reloadSound;
    std::string emptySound;
};

// Класс оружия
class Weapon {
public:
    Weapon(const WeaponConfig& config);
    virtual ~Weapon() = default;
    
    // Получение конфига
    const WeaponConfig& getConfig() const { return m_config; }
    
    // Состояние оружия
    bool canFire() const;
    bool isReloading() const { return m_isReloading; }
    bool isEmpty() const { return m_currentAmmo <= 0; }
    
    i32 getCurrentAmmo() const { return m_currentAmmo; }
    i32 getMagazineAmmo() const { return m_magazineAmmo; }
    i32 getTotalAmmo() const { return m_totalAmmo; }
    
    // Действия
    bool fire(const Vec3& origin, const Vec3& direction, Entity* shooter);
    void startReload();
    void update(f32 deltaTime);
    
    // Подбор/смена оружия
    void giveAmmo(i32 amount);
    void takeDamage(f32 amount);
    
    // Владельц
    Entity* getOwner() const { return m_owner; }
    void setOwner(Entity* owner) { m_owner = owner; }
    
protected:
    WeaponConfig m_config;
    
    i32 m_currentAmmo = 0;
    i32 m_magazineAmmo = 0;
    i32 m_totalAmmo = 0;
    
    bool m_isReloading = false;
    f32 m_reloadTimer = 0;
    f32 m_fireTimer = 0;
    f32 m_drawTimer = 0;
    
    Entity* m_owner = nullptr;
    
    // Методы для разных типов оружия
    virtual bool fireHitscan(const Vec3& origin, const Vec3& direction);
    virtual bool fireProjectile(const Vec3& origin, const Vec3& direction);
};

// Менеджер оружия (для игрока)
class WeaponManager {
public:
    static WeaponManager& getInstance() {
        static WeaponManager instance;
        return instance;
    }
    
    // Управление оружием
    void addWeapon(std::unique_ptr<Weapon> weapon);
    Weapon* getCurrentWeapon() const;
    Weapon* getWeapon(u32 index) const;
    
    void selectWeapon(u32 index);
    void nextWeapon();
    void previousWeapon();
    
    bool hasWeapon(const std::string& name) const;
    
    void update(f32 deltaTime);
    
private:
    WeaponManager() = default;
    
    std::vector<std::unique_ptr<Weapon>> m_weapons;
    i32 m_currentWeaponIndex = -1;
};

} // namespace res
