#pragma once

#include "Core/Math.h"
#include "Core/Types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <any>

namespace res {

class Entity;

// Структура карты
struct MapData {
    std::string name;
    std::string description;
    
    std::vector<Entity*> entities;
    
    // Браши (геометрия уровня)
    struct Brush {
        Vec3 position;
        Vec3 rotation;
        Vec3 size;
        std::string texture;
        bool isTrigger;
    };
    std::vector<Brush> brushes;
    
    // Проппы (статические модели)
    struct Prop {
        Vec3 position;
        Vec3 rotation;
        Vec3 scale;
        std::string modelPath;
    };
    std::vector<Prop> props;
    
    // Освещение
    struct LightData {
        Vec3 position;
        Vec3 color;
        f32 intensity;
        f32 range;
        bool isDirectional;
    };
    std::vector<LightData> lights;
    
    // Окружение
    std::string skyboxPath;
    
    struct FogData {
        Color color;
        f32 density;
        f32 startDistance;
        f32 endDistance;
    };
    FogData fog;
    
    // Скрипты уровня
    std::string mapScript;
    std::vector<std::string> entityScripts;
};

// Сериализатор карт (JSON формат)
class MapSerializer {
public:
    static MapSerializer& getInstance() {
        static MapSerializer instance;
        return instance;
    }
    
    // Сохранение карты
    bool saveMap(const std::string& filename, const MapData& data);
    
    // Загрузка карты
    bool loadMap(const std::string& filename, MapData& data);
    
    // Экспорт в бинарный формат
    bool saveBinary(const std::string& filename, const MapData& data);
    bool loadBinary(const std::string& filename, MapData& data);
    
private:
    MapSerializer() = default;
    
    std::string serializeEntity(Entity* entity) const;
    Entity* deserializeEntity(const std::string& json) const;
    
    std::string vec3ToJson(const Vec3& v) const;
    Vec3 jsonToVec3(const std::string& json) const;
};

// Десериализатор (для использования в Entity)
class MapDeserializer {
public:
    MapDeserializer(const std::string& json);
    
    std::string getString(const std::string& key) const;
    i32 getInt(const std::string& key) const;
    f32 getFloat(const std::string& key) const;
    Vec3 getVec3(const std::string& key) const;
    bool getBool(const std::string& key) const;
    
    bool hasKey(const std::string& key) const;
    
private:
    std::string m_json;
};

} // namespace res
