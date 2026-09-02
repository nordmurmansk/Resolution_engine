-- Пример скрипта ИИ для Resolution Engine

-- Хук при создании сущности
function OnSpawn(entity)
    print("AI spawned: " .. entity:GetName())
    
    -- Установка начальных свойств
    entity:SetHealth(100)
    entity:SetFaction("hostile")
end

-- Хук при каждом кадре
function OnUpdate(entity, deltaTime)
    -- Логика ИИ здесь
    -- Например, патрулирование или поиск игрока
end

-- Хук при входе в триггер
function OnTriggerEnter(entity, other)
    if other:GetType() == "player" then
        print("Player detected!")
        -- Начать преследование
    end
end

-- Хук при выходе из триггера
function OnTriggerExit(entity, other)
    print("Entity left trigger")
end

-- Хук при взаимодействии
function OnUse(entity, user)
    print("Entity used by: " .. user:GetName())
end
