# Resolution Engine

Легковесный игровой движок на C++ с поддержкой Lua-скриптинга, редактором уровней и всеми необходимыми компонентами для создания FPS-игр.

## Требования

- Windows 10/11 или Linux
- Visual Studio 2022 или GCC 11+
- OpenGL 4.5+
- 4GB RAM минимум

## Сборка

### Windows (Visual Studio)
```bash
cd ResolutionEngine
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Linux
```bash
cd ResolutionEngine
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Структура проекта

```
ResolutionEngine/
├── src/                    # Исходный код
│   ├── Core/              # Ядро движка
│   ├── Renderer/          # Рендеринг
│   ├── Physics/           # Физика и коллизии
│   ├── Scripting/         # Lua-скриптинг
│   ├── Entities/          # Игровые сущности
│   ├── AI/                # Искусственный интеллект
│   ├── Weapons/           # Оружие
│   ├── UI/                # Пользовательский интерфейс
│   ├── Audio/             # Звук
│   ├── Editor/            # Редактор уровней
│   └── Assets/            # Управление ассетами
├── include/               # Заголовочные файлы
├── data/                  # Данные игры
│   ├── maps/              # Карты уровней
│   ├── textures/          # Текстуры
│   ├── models/            # 3D модели
│   ├── sounds/            # Звуки
│   └── scripts/           # Lua скрипты
├── docs/                  # Документация
├── projects/              # Проекты пользователей
├── CMakeLists.txt         # Конфигурация сборки
└── README.md              # Этот файл
```

## Запуск

После сборки движок запускается одним исполняемым файлом:

```bash
# Windows
ResolutionEngine.exe

# Linux
./ResolutionEngine
```

## Редактор уровней

Редактор встроен в движок и активируется флагом `-editor`:

```bash
ResolutionEngine.exe -editor
```

### Возможности редактора:

- **Viewport**: Навигация камерой (WASD), выбор объектов, трансформации
- **Иерархия**: Список всех объектов, создание/удаление/переименование
- **Свойства**: Редактирование параметров объектов, Key/Value для сущностей
- **Браши**: Создание геометрии (куб, плоскость), назначение текстур
- **Ассеты**: Drag-and-drop моделей и текстур
- **Сохранение/Загрузка**: Формат карт JSON

## Скриптинг (Lua)

### Хуки карты

```lua
function OnMapStart()
    -- Вызывается при загрузке карты
end

function OnMapEnd()
    -- Вызывается при завершении карты
end
```

### Хуки сущностей

```lua
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
```

### Доступ к свойствам сущности

```lua
-- Позиция
local pos = entity:GetPosition()
entity:SetPosition(x, y, z)

-- Поворот
local rot = entity:GetRotation()
entity:SetRotation(pitch, yaw, roll)

-- Здоровье
local health = entity:GetHealth()
entity:SetHealth(100)

-- Тип враждебности
entity:SetFaction("hostile") -- friendly, neutral, hostile, attack_all
```

### Перезагрузка скриптов

```lua
-- В консоли движка
reload_scripts()
```

## Формат карты (JSON)

```json
{
  "name": "level_01",
  "entities": [
    {
      "type": "player_spawn",
      "position": [0, 0, 0],
      "rotation": [0, 0, 0]
    },
    {
      "type": "enemy",
      "position": [10, 0, 5],
      "rotation": [0, 180, 0],
      "properties": {
        "health": 100,
        "faction": "hostile",
        "script": "scripts/enemy_ai.lua"
      }
    },
    {
      "type": "trigger",
      "bounds": [[0,0,0], [5,5,5]],
      "properties": {
        "script": "scripts/door_trigger.lua"
      }
    }
  ],
  "brushes": [],
  "props": [],
  "lights": [],
  "skybox": "textures/skybox/",
  "fog": {
    "color": [0.5, 0.5, 0.5],
    "density": 0.01
  }
}
```

## API оружия

Оружие настраивается через свойства:

```json
{
  "type": "weapon",
  "weapon_type": "hitscan", -- hitscan или projectile
  "damage": 25,
  "range": 1000,
  "spread": 0.02,
  "fire_rate": 0.1,
  "magazine_size": 30,
  "penetration": 1,
  "projectile_type": "bullet" -- bullet, rocket, grenade
}
```

## Типы сущностей

- `player_spawn` - Точка спавна игрока
- `enemy` - Враг (NPC с ИИ)
- `item` - Подбираемый предмет
- `trigger` - Триггер-зона
- `door` - Дверь (движущаяся геометрия)
- `light` - Источник света
- `decal` - Декаль
- `prop` - Статический объект
- `weapon` - Оружие

## ИИ врагов

Враги поддерживают следующие поведения:

- **Патрулирование**: Перемещение по заданным точкам
- **Обнаружение**: По радиусу и углу обзора
- **Преследование**: Следование за игроком
- **Атака**: Hitscan выстрелы
- **Смерть**: Анимация и удаление

Настройка враждебности:
- `friendly` - Дружелюбный
- `neutral` - Нейтральный (атакует только если атакован)
- `hostile` - Враждебный (атакует игрока)
- `attack_all` - Атакует всех

## Графические возможности

- Загрузка static mesh (glTF)
- Текстуры: albedo, normal, roughness (PNG)
- Прозрачные материалы
- Скайбокс (кубическая текстура)
- Туман
- Базовое освещение: directional + точечные источники

## Физика

- Гравитация
- Капсула игрока vs статическая геометрия
- Raycast для стрельбы и проверки видимости
- Триггер-зоны

## Звук

- Загрузка OGG/WAV
- Пространственный звук (3D позиционирование)
- Звуки: выстрел, перезарядка, шаги, получение урона, смерть

## Менеджер проектов

Движок включает менеджер проектов для организации игр:

```bash
# Создать новый проект
ResolutionEngine.exe -new_project MyGame

# Открыть проект
ResolutionEngine.exe -project MyGame

# Запустить карту из проекта
ResolutionEngine.exe -project MyGame -map level_01
```

## Лицензия

MIT License
