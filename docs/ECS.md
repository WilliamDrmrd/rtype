# ECS Technical Documentation

# Components.hpp

```cpp
#pragma once

#include <memory>

namespace ECS
{
    class BaseComponent
    {
        public:
            virtual ~BaseComponent() = default;
    };
    template <typename T> class ComponentHandle
    {
        public:
            ComponentHandle() : _component(nullptr){};
            explicit ComponentHandle(std::shared_ptr<T> component) : _component(component){};
            ~ComponentHandle() = default;

            /*======================//
            //  Operator Overloads  //
            //======================*/

            std::shared_ptr<T> operator*()
            {
                return get();
            }

            std::shared_ptr<T> operator->()
            {
                return get();
            }

            T *operator=(T *component) = delete;

            bool operator==(const ComponentHandle<T> &other)
            {
                return _component == other._component;
            }

            bool operator==(bool other)
            {
                return isValid() == other;
            }

            bool operator!=(const ComponentHandle<T> &other)
            {
                return _component != other._component;
            }

            bool operator!=(bool other)
            {
                return isValid() != other;
            }

            /*=====================//
            //  Validity Handling  //
            //=====================*/

            bool isValid()
            {
                return _component != nullptr;
            }

        private:
            std::shared_ptr<T> _component; ///< Pointer to the component.
            std::shared_ptr<T> get()
            {
                return _component;
            }
    };

} // namespace ECS
```

The **`Components.hpp`** file is a fundamental part of the ECS (Entity-Component-System) architecture within the R-Type game project. This header file defines classes for components and a template class **`ComponentHandle`** to manage these components efficiently.

### **`BaseComponent`** Class:

- **`BaseComponent` Class**
  - **Description**: Abstract base class for ECS components.
  - **Member Functions**:
    - **`virtual ~BaseComponent()`**: Virtual destructor for **`BaseComponent`**.
      - **Description**: Destroys the base component object when derived components are destructed.
      - **Parameters**: None.
      - **Return**: Void.

### **`ComponentHandle`** Template Class:

- **`ComponentHandle` Template Class**
  - **Description**: Template class managing component access and manipulation.
  - **Member Functions**:
    - **`ComponentHandle()`**: Default constructor initializing the component handle to nullptr.
      - **Description**: Initializes the handle.
      - **Parameters**: None.
      - **Return**: Void.
    - **`explicit ComponentHandle(std::shared_ptr<T> component)`**: Constructor initializing the component handle with a given component.
      - **Description**: Initializes the handle with the provided component pointer.
      - **Parameters**: **`component`** - Pointer to the component.
      - **Return**: Void.
    - **`~ComponentHandle()`**: Default destructor.
      - **Description**: Destroys the component handle.
      - **Parameters**: None.
      - **Return**: Void.
    - **`std::shared_ptr<T> operator*()`**: Overloaded dereference operator to access the underlying component.
      - **Description**: Accesses the underlying component.
      - **Parameters**: None.
      - **Return**: Shared pointer to the component.
    - **`bool isValid()`**: Checks if the handle is valid (i.e., it points to a component).
      - **Description**: Checks the validity of the handle.
      - **Parameters**: None.
      - **Return**: **`true`** if the handle is valid, **`false`** otherwise.

# Entity.hpp

```cpp
#pragma once

#include <ctime>
#include <memory>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

#include "Clock.hpp"
#include "Components.hpp"
#include "Utilities.hpp"

namespace ECS
{

    class World;

    class Entity
    {
            friend class World;
        public:
            explicit Entity() : _components(), _clock(){};
            ~Entity()
            {
                removeAllComponents();
            };

            template <typename T> void addComponent(T *component)
            {
                _components.emplace(ECS_TYPEID(T), std::shared_ptr<T>(component));
            }

            template <typename T> void removeComponent()
            {
                auto index = ECS_TYPEID(T);
                auto it    = _components.find(index);
                if (it != _components.end()) {
                    _components.erase(it);
                }
            }
            void removeAllComponents()
            {
                _components.clear();
            }

            template <typename T> ComponentHandle<T> getComponent()
            {
                auto               baseComponentPtr     = _components.at(ECS_TYPEID(T));
                std::shared_ptr<T> specificComponentPtr = std::dynamic_pointer_cast<T>(baseComponentPtr);
                return ComponentHandle<T>(specificComponentPtr);
            }

            template <typename T> bool has() const
            {
                if (_components.empty()) return false;
                return _components.find(ECS_TYPEID(T)) != _components.end();
            }

            template <typename T, typename V, typename... Types> bool has() const
            {
                return has<T>() && has<V, Types...>();
            }

            std::clock_t getElapsedTime()
            {
                return _clock.getElapsedTime();
            }

        private:
            std::unordered_map<id_t, std::shared_ptr<BaseComponent>> _components;
            Clock                                                    _clock;
    };

    class GlobalEntity : public Entity
    {
        public:
            GlobalEntity(World *world) : Entity(), _worlds()
            {
                _worlds.push_back(world);
            }
            ~GlobalEntity() {}

        private:
            std::vector<World *> _worlds;
    };

} // namespace ECS
```

The **`Entity.hpp`** file within the ECS (Entity-Component-System) framework for the R-Type game project contains classes representing entities and a specialized **`GlobalEntity`** class. These classes manage components and their functionalities within the ECS architecture.

### **`Entity`** Class:

- **`Entity` Class**
  - **Description**: Represents an entity in the ECS architecture.
  - **Member Functions**:
    - **`explicit Entity()`**: Constructs a new Entity object.
    - **`~Entity()`**: Destroys the Entity object.
    - **`template <typename T> void addComponent(T *component)`**: Adds a component to the Entity.
    - **`template <typename T> void removeComponent()`**: Removes and destroys a component.
    - **`void removeAllComponents()`**: Removes all components.
    - **`template <typename T> ComponentHandle<T> getComponent()`**: Gets a handle to a component.
    - **`template <typename T> bool has() const`**: Checks if the Entity has a specific component type.
    - **`template <typename T, typename V, typename... Types> bool has() const`**: Checks if the Entity has a list of specified components.
    - **`std::clock_t getElapsedTime()`**: Retrieves the time elapsed since the last call.

### **`GlobalEntity`** Class:

- **`GlobalEntity` Class (inherits from `Entity`)**
  - **Description**: Represents an entity with global access across multiple Worlds.
  - **Member Functions**:
    - **`GlobalEntity(World *world)`**: Constructs a GlobalEntity associated with a specific World.
    - **`~GlobalEntity()`**: Destroys the GlobalEntity object.

# EventSubscriber.hpp

```cpp
#pragma once

#include <string>

#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"

namespace ECS
{
    class BaseEventSubscriber
    {
        public:
            virtual ~BaseEventSubscriber() = default;
    };

    template <typename T> class EventSubscriber : public BaseEventSubscriber
    {
        public:
            ~EventSubscriber() override = default;

            virtual void receiveEvent(const std::string &name, const T &data) = 0;
    };

    namespace Events
    {
        struct OnEntityCreated {
                Entity *entity;
        };

        struct OnEntityDestroyed {
                Entity *entity;
        };

        template <typename T> struct OnComponentAdded {
                Entity             *entity;
                ComponentHandle<T> *component;
        };

        template <typename T> struct OnComponentRemoved {
                Entity             *entity;
                ComponentHandle<T> *component;
        };
    } // namespace Events
} // namespace ECS
```

The **`EventSubscriber.hpp`** file within the ECS (Entity-Component-System) framework for the R-Type game project defines classes for event subscribers. These classes allow entities to listen and respond to specific events within the ECS architecture.

### **`BaseEventSubscriber`** Class:

- **`BaseEventSubscriber` Class**
  - **Description**: Abstract base class for event subscribers within the ECS.
  - **Member Functions**:
    - **`virtual ~BaseEventSubscriber()`**: Virtual destructor for **`BaseEventSubscriber`**.
      - **Description**: Destroys the **`BaseEventSubscriber`** object when derived event subscribers are destructed.
      - **Parameters**: None.
      - **Return**: Void.

### **`EventSubscriber`** Template Class:

- **`EventSubscriber` Template Class**
  - **Description**: Template class for event subscribers specialized to certain event types.
  - **Member Functions**:
    - **`virtual void receiveEvent(const std::string &name, const T &data) = 0;`**
      - **Description**: Receives and processes an event.
      - **Parameters**:
        - **`name`** - The name of the event.
        - **`data`** - Reference to the event data of type **`T`**.
      - **Return**: Void.
      - **Note**: This method can cause infinite recursion under certain circumstances. Care must be taken when broadcasting events.

### **`Events`** Namespace:

- **`Events` Namespace**
  - **Description**: Contains various event structures used within the ECS architecture.
  - **Structures**:
    - **`OnEntityCreated`**: Event structure for entity creation.
    - **`OnEntityDestroyed`**: Event structure for entity destruction.
    - **`template <typename T> struct OnComponentAdded`**: Event structure for component addition to an entity.
    - **`template <typename T> struct OnComponentRemoved`**: Event structure for component removal from an entity.

# System.hpp

```cpp
#pragma once

#include <vector>

#include "Clock.hpp"
#include "Utilities.hpp"

namespace ECS
{
    class World;

    class BaseSystem
    {
        public:
            explicit BaseSystem(World &world) : _world(world), _entities_id(), _clock(){};

            BaseSystem(World &world, id_t ids...) : _world(world), _entities_id()
            {
                _entities_id.emplace_back(ids);
            }

            virtual ~BaseSystem() = default;

            virtual void configure(World &world) = 0;

            [[maybe_unused]] virtual void unconfigure() = 0;

            virtual void tick() = 0;

            [[nodiscard]] World &getWorld() const
            {
                return _world;
            }

        private:
            [[maybe_unused]] World &_world;       ///< Reference to the World this system belongs to.
            std::vector<id_t>       _entities_id; ///< List of entity IDs that this system processes.
            [[maybe_unused]] Clock  _clock{};     ///< Clock used to track the time between ticks.
    };

    class HigherOrderBaseSystem : public BaseSystem
    {
    };
} // namespace ECS
```

The **`System.hpp`** file in the ECS (Entity-Component-System) framework for the R-Type game project contains classes defining systems responsible for processing entities with specific sets of components.

### **`BaseSystem`** Class:

- **`BaseSystem` Class**
  - **Description**: Abstract base class for systems in the ECS framework.
  - **Member Functions**:
    - **`explicit BaseSystem(World &world)`**: Constructs a new BaseSystem object associated with a World.
    - **`BaseSystem(World &world, id_t ids...)`**: Constructs a new BaseSystem object with a list of entity IDs to be processed.
    - **`virtual ~BaseSystem()`**: Virtual destructor for BaseSystem.
    - **`virtual void configure(World &world) = 0;`**: Configures the system with the World.
    - **`[[maybe_unused]] virtual void unconfigure() = 0;`**: Unconfigures the system, releasing resources.
    - **`virtual void tick() = 0;`**: Updates the system to process its entities.
    - **`[[nodiscard]] World &getWorld() const`**: Retrieves the World associated with this system.

### **`HigherOrderBaseSystem`** Class:

- **`HigherOrderBaseSystem` Class (inherits from `BaseSystem`)**
  - **Description**: Placeholder class for future system extension or specialization.
  - **Note**: No additional functionalities defined in this class.

# World.hpp

```cpp
#pragma once

#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "ECS/Components.hpp"
#include "ECS/System.hpp"
#include "Engine/Engine.hpp"
#include "Entity.hpp"
#include "EventSubscriber.hpp"
#include "Utilities.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
typedef long int Int64;
#endif

namespace ECS
{
    class World
    {
        public:

            World()
                : _entities(), _global_entities(), _subscribers(), _clock(), _engine(Engine::EngineClass::getEngine())
            {
            }

            ~World()
            {
                _systems.clear();
                _entities.clear();
                for (auto &sub : _subscribers) {
                    for (auto &pair : sub.second) {
                        delete pair.second;
                    }
                }
            }

            /*===================//
            //  Entity Handling  //
            //===================*/

            id_t addEntity(std::unique_ptr<Entity> entity)
            {
                type_t                  id = Utils::getNewId<Entity>();
                Events::OnEntityCreated event{entity.get()};

                _entities.emplace(id, std::move(entity));
                if (_subscribers.find(ECS_TYPEID(Events::OnEntityCreated)) != _subscribers.end())
                    broadcastEvent<Events::OnEntityCreated>(event);
                return id;
            }

            id_t addEntity()
            {
                std::unique_ptr<Entity> entity = std::make_unique<Entity>();

                return addEntity(std::move(entity));
            }

            template <typename... Components> id_t createEntity(Components &&...components)
            {
                id_t  id     = addEntity();
                auto &entity = getMutEntity(id);
                (entity.addComponent(std::forward<Components>(components)), ...);
                return id;
            }

            void removeEntity(id_t id)
            {
                Events::OnEntityDestroyed event{_entities.at(id).get()};

                _entities.erase(id);
                if (_subscribers.find(ECS_TYPEID(Events::OnEntityDestroyed)) != _subscribers.end())
                    broadcastEvent<Events::OnEntityDestroyed>(event);
            }

            const Entity &getEntity(id_t id) const
            {
                return *_entities.at(id);
            }

            Entity &getMutEntity(id_t id)
            {
                return *_entities.at(id);
            }

            template <typename T> std::unordered_map<Entity *, ComponentHandle<T>> get()
            {
                std::unordered_map<Entity *, ComponentHandle<T>> entities;
                for (auto &pair : _entities) {
                    Entity &entity = *pair.second;
                    if (entity.has<T>()) {
                        ComponentHandle<T> component = entity.getComponent<T>();
                        entities.emplace(&entity, component);
                    }
                }
                return entities;
            }

            template <typename T> void each(std::function<void(Entity *, ComponentHandle<T>)> func)
            {
                for (auto &pair : _entities) {
                    Entity &entity = *pair.second;
                    if (entity.has<T>()) {
                        ComponentHandle<T> component = entity.getComponent<T>();
                        func(&entity, component);
                    }
                }
            }

            template <typename T, typename U>
            void each(std::function<void(Entity *, ComponentHandle<T>, ComponentHandle<U>)> func)
            {
                for (auto &pair : _entities) {
                    Entity &entity = *pair.second;
                    if (entity.has<T>() && entity.has<U>()) {
                        ComponentHandle<T> componentT = entity.getComponent<T>();
                        ComponentHandle<U> componentU = entity.getComponent<U>();
                        func(&entity, componentT, componentU);
                    }
                }
            }

            template <typename T, typename U, typename V>
            void each(std::function<void(Entity *, ComponentHandle<T>, ComponentHandle<U>, ComponentHandle<V>)> func)
            {
                for (auto &pair : _entities) {
                    Entity &entity = *pair.second;
                    if (entity.has<T>() && entity.has<U>()) {
                        ComponentHandle<T> componentT = entity.getComponent<T>();
                        ComponentHandle<U> componentU = entity.getComponent<U>();
                        ComponentHandle<V> componentV = entity.getComponent<V>();
                        func(&entity, componentT, componentU, componentV);
                    }
                }
            }

            template <typename... Types> void each(std::function<void(Entity *, ComponentHandle<Types...>)> func)
            {
                for (auto &pair : _entities) {
                    Entity &entity = *pair.second;
                    if (entity.has<Types...>()) {
                        _eachHelper<Types...>(&entity, func);
                    }
                }
            }

            template <typename... Types> std::vector<Entity *> getEntitiesWithComponents()
            {
                std::vector<Entity *> entities;
                for (auto &pair : _entities) {
                    Entity &entity = *pair.second;
                    if (entity.has<Types...>()) {
                        entities.push_back(&entity);
                    }
                }
                return entities;
            }

            template <typename... Types> Entity *getEntityWithComponents()
            {
                std::vector<Entity *> entities = getEntitiesWithComponents<Types...>();

                if (entities.empty()) return nullptr;

                return entities[0];
            }

            /*=========================//
            //  GlobalEntity Handling  //
            //=========================*/

            id_t addGlobalEntity(std::unique_ptr<GlobalEntity> entity)
            {
                type_t id = Utils::getNewId<GlobalEntity>();

                _global_entities.emplace(id, std::move(entity));
                return id;
            }

            void removeGlobalEntity(id_t id)
            {
                _global_entities.erase(id);
            }

            const GlobalEntity &getGlobalEntity(id_t id)
            {
                return *_global_entities.at(id);
            }

            GlobalEntity &getMutGlobalEntity(id_t id)
            {
                return *_global_entities.at(id);
            }

            template <typename T> std::unordered_map<ECS::GlobalEntity *, ComponentHandle<T>> getGlobal()
            {
                std::unordered_map<ECS::GlobalEntity *, ComponentHandle<T>> entities;
                for (auto &pair : _global_entities) {
                    GlobalEntity &entity = *pair.second;
                    if (entity.has<T>()) {
                        ComponentHandle<T> component = entity.getComponent<T>();
                        entities.emplace(&entity, component);
                    }
                }
                return entities;
            }

            /*==================//
            //  Event Handling  //
            //==================*/

            template <typename T> id_t subscribe(const EventSubscriber<T> *subscriber)
            {
                auto type_id = ECS_TYPEID(T);
                auto uuid    = Utils::getNewId<T>();

                if (_subscribers.find(type_id) == _subscribers.end())
                    _subscribers.emplace(type_id, std::unordered_map<id_t, BaseEventSubscriber *>());

                _subscribers[type_id].emplace(
                    uuid, static_cast<BaseEventSubscriber *>(const_cast<EventSubscriber<T> *>(subscriber)));
                return uuid;
            }

            template <typename T> void unsubscribe(id_t uuid)
            {
                auto type_id = ECS_TYPEID(T);

                if (_subscribers.find(type_id) == _subscribers.end()) return;
                _subscribers[type_id].erase(uuid);
            }

            template <typename T> void broadcastEvent(T data, const std::string name = "")
            {
                using namespace std::chrono;

                const std::unordered_map<id_t, BaseEventSubscriber *> &subscribers = _subscribers[ECS_TYPEID(T)];

                for (auto &subscriber : subscribers) {
                    auto *sub = static_cast<EventSubscriber<T> *>(subscriber.second);
                    sub->receiveEvent(name, data);
                }
            }

            /*====================//
            //  Systems Handling  //
            //====================*/

            template <typename System, typename... types> void addSystem(std::string name, types &&...args)
            {
                _systems.emplace(name, std::make_unique<System>(*this, std::forward<types>(args)...));
            }

            template <typename System> void addSystem(std::string name)
            {
                _systems.emplace(name, std::make_unique<System>(*this));
            }

            /*==================//
            //  Logic Handling  //
            //==================*/

            std::clock_t getWorldTime()
            {
                return _clock.getElapsedTime();
            }

            void tick()
            {
                BaseSystem *renderer;

                for (auto &system : _systems) {
                    if (system.first == "Renderer") {
                        renderer = system.second.get();
                        continue;
                    }
                    system.second->tick();
                }
                renderer->tick();
            }

            /*===================//
            //  Getters/Setters  //
            //===================*/

            std::unordered_map<std::string, std::unique_ptr<BaseSystem>> &getSystems()
            {
                return _systems;
            }

            std::unordered_map<id_t, std::unique_ptr<Entity>> &getEntities()
            {
                return _entities;
            }

        private:
            /*====================//
            //  Helper Functions  //
            //====================*/

            template <typename T>
            void _eachHelper(Entity *entity, std::function<void(Entity *, ComponentHandle<T>)> func)
            {
                func(&entity, entity->getComponent<T>());
            }

            template <typename T, typename V, typename... Types>
            void _eachHelper(Entity *entity, std::function<void(Entity *, ComponentHandle<T>)> func)
            {
                entity->has<T>() ? _eachHelper<T>(entity, func) : nullptr;
                _eachHelper<V, Types...>(entity, func);
            }

            template <typename T>
            void _eachHelper(GlobalEntity *g_entity, std::function<void(GlobalEntity *, ComponentHandle<T>)> func)
            {
                func(&g_entity, g_entity->getComponent<T>());
            }

            template <typename T, typename V, typename... Types>
            void _eachHelper(GlobalEntity *g_entity, std::function<void(GlobalEntity *, ComponentHandle<T>)> func)
            {
                g_entity->has<T>() ? _eachHelper<T>(g_entity, func) : nullptr;
                _eachHelper<V, Types...>(g_entity, func);
            }

            /*==============//
            //  Attributes  //
            //==============*/

            std::unordered_map<id_t, std::unique_ptr<Entity>>                           _entities;
            std::unordered_map<id_t, std::unique_ptr<GlobalEntity>>                     _global_entities;
            std::unordered_map<type_t, std::unordered_map<id_t, BaseEventSubscriber *>> _subscribers;
            std::unordered_map<std::string, std::unique_ptr<BaseSystem>>                _systems;
            Clock                                                                       _clock;
            Engine::EngineClass                                                        &_engine;
    };
} // namespace ECS
```

The **`World.hpp`** file is an integral part of the R-Type game project's ECS (Entity-Component-System) framework. It orchestrates the fundamental functionalities concerning entity management, global entities, systems, event handling, and logical control within the game world.

The header encapsulates core mechanisms essential for organizing and regulating the game's entities, systems, and events in an ECS architecture.

### Class Overview:

### **`World` Class:**

**Description:**
The **`World`** class manages the game world, providing functionalities for entity handling, global entity handling, event management, system handling, and logical control.

**Member Functions:**

- **`World()`**: Constructor for initializing a new game world.
- **`~World()`**: Destructor for destroying the game world and its elements.

**Entity Handling:**

- **`id_t addEntity(std::unique_ptr<Entity> entity)`**: Adds an entity to the world.
- **`id_t addEntity()`**: Adds an empty entity to the world.
- **`template <typename... Components> id_t createEntity(Components &&...components)`**: Creates an entity with specified components.
- **`void removeEntity(id_t id)`**: Removes an entity from the world.
- **`const Entity &getEntity(id_t id) const`**: Retrieves a const reference to an entity.
- **`Entity &getMutEntity(id_t id)`**: Retrieves a mutable reference to an entity.
- Template functions:
  - **`std::unordered_map<Entity *, ComponentHandle<T>> get()`**: Retrieves entities with a specific component.
  - **`void each(...)`**: Executes a function for each entity with specified components.
  - **`std::vector<Entity *> getEntitiesWithComponents()`**: Retrieves entities with a set of specified components.
  - **`Entity *getEntityWithComponents()`**: Retrieves the first entity with specified components.

**GlobalEntity Handling:**

- **`id_t addGlobalEntity(std::unique_ptr<GlobalEntity> entity)`**: Adds a global entity to the world.
- **`void removeGlobalEntity(id_t id)`**: Removes a global entity from the world.
- **`const GlobalEntity &getGlobalEntity(id_t id)`**: Retrieves a const reference to a global entity.
- **`GlobalEntity &getMutGlobalEntity(id_t id)`**: Retrieves a mutable reference to a global entity.
- Template function:
  - **`std::unordered_map<ECS::GlobalEntity *, ComponentHandle<T>> getGlobal()`**: Retrieves global entities with a specific component.

**Event Handling:**

- Template functions:
  - **`id_t subscribe(const EventSubscriber<T> *subscriber)`**: Subscribes to a specific event.
  - **`void unsubscribe(id_t uuid)`**: Unsubscribes from a specific event.
  - **`void broadcastEvent(T data, const std::string name = "")`**: Broadcasts an event to subscribers.

**Systems Handling:**

- Template functions:
  - **`void addSystem(std::string name, types &&...args)`**: Adds a system to the world.
  - **`void addSystem(std::string name)`**: Adds a system to the world.

**Logic Handling:**

- **`std::clock_t getWorldTime()`**: Retrieves the world's current time.
- **`void tick()`**: Advances the logical state of the world.

**Getters/Setters:**

- **`std::unordered_map<std::string, std::unique_ptr<BaseSystem>> &getSystems()`**: Retrieves the systems in the world.
- **`std::unordered_map<id_t, std::unique_ptr<Entity>> &getEntities()`**: Retrieves the entities in the world.

### **Entity Handling Helpers:**

- **`template <typename T> void _eachHelper(Entity *entity, std::function<void(Entity *, ComponentHandle<T>)> func)`**: Facilitates iteration over entities with specific components, executing a provided function for each matching entity-component pair.
- **`template <typename T, typename V, typename... Types> void _eachHelper(Entity *entity, std::function<void(Entity *, ComponentHandle<T>)> func)`**: Enables cascading entity-component checks, ensuring sequential execution based on the presence of required components.

### **GlobalEntity Handling Helpers:**

- **`template <typename T> void _eachHelper(GlobalEntity *g_entity, std::function<void(GlobalEntity *, ComponentHandle<T>)> func)`**: Aids in iterating through global entities possessing specific components, executing a given function for each entity-component combination.
- **`template <typename T, typename V, typename... Types> void _eachHelper(GlobalEntity *g_entity, std::function<void(GlobalEntity *, ComponentHandle<T>)> func)`**: Extends the capability to handle cascading component checks for global entities, ensuring controlled function execution based on the existence of specified components.

# Utilities.hpp

```cpp
#pragma once

#include <cstddef>

#define ECS_TYPEID(T) typeid(T).hash_code()

namespace ECS
{
    typedef std::size_t id_t;
    typedef std::size_t type_t;

    class Utils
    {
        public:

            template <typename T> static id_t getNewId()
            {
                static id_t id = 0;
                return id++;
            }
    };
} // namespace ECS
```

The **`Utilities.hpp`** file within the ECS (Entity-Component-System) framework for the R-Type game project contains utility functions and type definitions essential for the ECS functionality.

### **`Utils`** Class:

- **`Utils` Class**
  - **Description**: Utility class for the ECS framework.
  - **Member Functions**:
    - **`template <typename T> static id_t getNewId()`**: Retrieves a new unique identifier for a specified type.
      - **Parameters**: None.
      - **Return**: **`id_t`** - A new unique identifier for the specified type.
      - **Note**: Typedefs will share identifiers with their underlying types.

# Clock.hpp

```cpp
#pragma once

#include <ctime>

#ifndef CLOCK_MIN_ELAPSED_TIME      // Minimum elapsed time between two calls to the update method of a system
#define CLOCK_MIN_ELAPSED_TIME 1000 // in milliseconds
#endif                              // !CLOCK_MIN_ELAPSED_TIME

namespace ECS
{
    class Clock
    {
        public:
            Clock()  = default;
            ~Clock() = default;

            std::clock_t operator==(const std::clock_t &other)
            {
                return std::clock() == other;
            }

            std::clock_t operator>=(const std::clock_t &other)
            {
                return std::clock() >= other;
            }

            std::clock_t operator<=(const std::clock_t &other)
            {
                return std::clock() <= other;
            }

            std::clock_t operator+(const std::clock_t &other)
            {
                return std::clock() + other;
            }

            std::clock_t operator-(const std::clock_t &other)
            {
                return std::clock() - other;
            }

            std::clock_t operator/(const std::clock_t &other)
            {
                return std::clock() / other;
            }

            std::clock_t getElapsedTime()
            {
                std::clock_t elapsed = std::clock() - _time;
                _time                = std::clock();
                return elapsed;
            }

            bool isValid()
            {
                return std::clock() >= CLOCK_MIN_ELAPSED_TIME;
            }

            void reset()
            {
                _time = 0;
            }

        private:
            std::clock_t _time;
    };
} // namespace ECS
```

The **`Clock`** class provides essential time tracking functionalities necessary for orchestrating time-dependent activities within the ECS system. It offers methods to compare, manipulate, and retrieve elapsed time, ensuring precise control over time-sensitive operations. The class also features validation checks and reset capabilities, augmenting its utility in managing time-related functionalities for systems and components within the game project.

### **Member Functions:**

- **`Clock()`**: Constructor for the Clock class.
- **`~Clock()`**: Destructor for the Clock class.
- **`std::clock_t operator==(const std::clock_t &other)`**: Overloads the equality operator to compare two clock values.
- **`std::clock_t operator>=(const std::clock_t &other)`**: Overloads the greater-than-or-equal-to operator for clock comparison.
- **`std::clock_t operator<=(const std::clock_t &other)`**: Overloads the less-than-or-equal-to operator for clock comparison.
- **`std::clock_t operator+(const std::clock_t &other)`**: Overloads the addition operator to add two clock values.
- **`std::clock_t operator-(const std::clock_t &other)`**: Overloads the subtraction operator to find the difference between two clock values.
- **`std::clock_t operator/(const std::clock_t &other)`**: Overloads the division operator for clock values.
- **`std::clock_t getElapsedTime()`**: Retrieves the elapsed time since the last clock update.
- **`bool isValid()`**: Checks whether the clock is valid based on a minimum elapsed time.
- **`void reset()`**: Resets the clock time to zero.
