#pragma once

#include "Scripting/LuaVM.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace res {

class Entity;

// Менеджер скриптов
class ScriptManager {
public:
    static ScriptManager& getInstance() {
        static ScriptManager instance;
        return instance;
    }
    
    // Инициализация
    bool initialize();
    void shutdown();
    
    // Загрузка скриптов
    bool loadScript(const std::string& filename);
    void unloadScript(const std::string& filename);
    void reloadAllScripts();
    
    // Выполнение хуков карты
    void onMapStart();
    void onMapEnd();
    
    // Выполнение хуков сущностей
    void onSpawn(Entity* entity);
    void onUse(Entity* entity, Entity* user);
    void onTriggerEnter(Entity* trigger, Entity* other);
    void onTriggerExit(Entity* trigger, Entity* other);
    void onUpdate(Entity* entity, f32 deltaTime);
    
    // Lua VM
    LuaVM* getVM() { return m_vm.get(); }
    
private:
    ScriptManager() = default;
    ~ScriptManager() = default;
    
    std::unique_ptr<LuaVM> m_vm;
    std::vector<std::string> m_loadedScripts;
};

} // namespace res
