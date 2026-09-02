#pragma once

#include "Core/Types.h"
#include <string>
#include <functional>
#include <memory>

// Вперёд объявляем lua_State
typedef struct lua_State lua_State;

namespace res {

class Entity;

// Lua виртуальная машина
class LuaVM {
public:
    LuaVM();
    ~LuaVM();
    
    // Инициализация
    bool initialize();
    void shutdown();
    
    // Загрузка скриптов
    bool loadFile(const std::string& filename);
    bool loadString(const std::string& code, const std::string& name = "chunk");
    
    // Выполнение функций
    bool callFunction(const std::string& funcName);
    bool callFunctionWithEntity(const std::string& funcName, Entity* entity);
    bool callFunctionWithTwoEntities(const std::string& funcName, Entity* entity1, Entity* entity2);
    
    // Регистрация функций C++ в Lua
    void registerFunction(const std::string& name, int (*func)(lua_State*));
    
    // Получение lua_state для прямых операций
    lua_State* getState() const { return m_state; }
    
    // Перезагрузка всех скриптов
    void reloadScripts();
    
private:
    lua_State* m_state = nullptr;
    bool m_initialized = false;
    
    void registerBindings();
};

} // namespace res
