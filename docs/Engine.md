# Engine Technical Documentation

```cpp
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstddef>
#include <functional>
#include <unordered_map>

#include "ECS/Entity.hpp"
#include "Server/Network.hpp"

namespace ECS
{
    class World;
} // namespace ECS

#ifndef DEFAULT_WINDOW_SIZE_X
#define DEFAULT_WINDOW_SIZE_X 800
#endif

#ifndef DEFAULT_WINDOW_SIZE_Y
#define DEFAULT_WINDOW_SIZE_Y 600
#endif

#ifndef DEFAULT_WINDOW_NAME
#define DEFAULT_WINDOW_NAME "default"
#endif

#ifndef BIND_F11_TO_OTHER
#define BIND_F11_TO_FULLSCREEN
#endif

#ifndef BIND_ESC_TO_OTHER
#define BIND_ESC_TO_QUIT
#endif

#ifndef DONT_ADD_RENDERER_SYSTEM
#define ADD_RENDERER_SYSTEM
#endif

#define WINDOW Engine::EngineClass::getEngine().window
#define WORLD Engine::EngineClass::getEngine().world()
#define NETWORK Engine::EngineClass::getEngine().network()

namespace Engine
{
    typedef std::unordered_map<std::string, std::function<ECS::World *()>> world_factories_t;
    typedef std::pair<std::string, ECS::World *>                           world_t;

    class EngineClass
    {
        public:
            /*===================//
            //  Singleton Logic  //
            //===================*/

            static EngineClass &getEngine();
            static EngineClass &getEngine(std::size_t x, std::size_t y, const std::string &name);

            EngineClass &operator=(const EngineClass &engine) = delete;
            EngineClass(const EngineClass &engine)            = delete;

        private:
            /*============================//
            //  Constructor / Destructor  //
            //============================*/

            EngineClass(std::size_t x = DEFAULT_WINDOW_SIZE_X, std::size_t y = DEFAULT_WINDOW_SIZE_Y,
                        const std::string &name = DEFAULT_WINDOW_NAME, std::string start_world = DEFAULT_WINDOW_NAME);

            ~EngineClass();

        public:
            /*===================//
            //  Getters/Setters  //
            //===================*/

            void setStartWorld(const std::string &name);
            int  getNetwork();

            std::size_t getWindowSizeX() const;
            std::size_t getWindowSizeY() const;

            void setCurrentPlayer(int);
            void setPlayersAmount(int);
            void setOwnPlayer(int);

            int getCurrentPlayer() const;
            int getPlayersAmount() const;
            int getOwnPlayer() const;

            /*===================//
            //  Worlds Handling  //
            //===================*/

            void createEmptyWorld(const std::string &name);

            void addWorldFactory(const std::string &name, std::function<ECS::World *()>);

            void switchWorld(const std::string &name);

            std::vector<std::string> getWorldsNames();

            ECS::World   &world() const;
            ECS::Network &network();

            /*=================//
            //  General Logic  //
            //=================*/

            void run();

            void toggleFullscreen();

            /*==================//
            //  Event Handling  //
            //==================*/

            void processSwitchEvent(sf::Event event);
            void handleEvents();
            void processClientsEvents();

            /*==================//
            // Network Handling //
            //==================*/

            void componentsUpdater(std::tuple<ECS::id_t, std::vector<ComponentType>,
                                              std::vector<std::pair<ECS::BaseComponent *, ComponentType>>> &tuple);

            /*=====================//
            //  Public Attributes  //
            //=====================*/

            sf::RenderWindow window;

            /*===================//
            //  Private Methods  //
            //===================*/

            ECS::id_t                addGlobalEntity(std::unique_ptr<ECS::GlobalEntity> entity);
            void                     removeGlobalEntity(ECS::id_t id);
            const ECS::GlobalEntity &getGlobalEntity(ECS::id_t id);
            ECS::GlobalEntity       &getMutGlobalEntity(ECS::id_t id);

            template <typename T> std::unordered_map<ECS::GlobalEntity *, ECS::ComponentHandle<T>> getGlobal();

            template <typename T> void each(std::function<void(ECS::GlobalEntity *, ECS::ComponentHandle<T>)> func);

            template <typename... Types>
            void each(std::function<void(ECS::GlobalEntity *, ECS::ComponentHandle<Types...>)> func);

            template <typename... Types> std::vector<ECS::GlobalEntity *> getGlobalEntitiesWithComponents();
            template <typename... Types> ECS::GlobalEntity               *getGlobalEntityWithComponents();

        private:
            /*==============//
            //  Attributes  //
            //==============*/

            bool                                                              _running;
            bool                                                              _fullscreen;
            world_factories_t                                                 _worldsFactories;
            world_t                                                           _currentWorld;
            std::vector<world_t>                                              _pending_destroy;
            std::string                                                       _startWorld;
            std::size_t                                                       _windowSizeX;
            std::size_t                                                       _windowSizeY;
            ECS::Network                                                      _network;
            int                                                               _playersAmount;
            int                                                               _currentPlayer;
            int                                                               _ownPlayer;
            std::unordered_map<ECS::id_t, std::unique_ptr<ECS::GlobalEntity>> _global_entities;

            /*===================//
            //  Private Methods  //
            //===================*/

            void destroyPendingWorlds();

            template <typename T>
            void _eachHelper(ECS::GlobalEntity                                                *g_entity,
                             std::function<void(ECS::GlobalEntity *, ECS::ComponentHandle<T>)> func);

            template <typename T, typename V, typename... Types>
            void _eachHelper(ECS::GlobalEntity                                                *g_entity,
                             std::function<void(ECS::GlobalEntity *, ECS::ComponentHandle<T>)> func);
    };
} // namespace Engine
```

### **`EngineClass` Class:**

- **Purpose**: Represents the main engine class responsible for managing the game engine's lifecycle, world switching, event handling, network management, and general game logic.
- **Singleton Logic**:
    - Utilizes the Singleton design pattern to ensure that there is only one instance of the **`EngineClass`**.
    - Provides static methods (**`getEngine`**) to access the single instance.
- **Constructor / Destructor**:
    - Private constructor and destructor to enforce the Singleton pattern.
    - Constructor allows customization of the window size, name, and the initial world.
- **Getters / Setters**:
    - **`getEngine`**: Static method to retrieve the single instance of the **`EngineClass`**.
    - **`setStartWorld`**: Sets the starting world for the engine.
    - **`getNetwork`**: Gets the network object associated with the engine.
    - **`getWindowSizeX`** / **`getWindowSizeY`**: Gets the dimensions of the game window.
    - **`setCurrentPlayer`** / **`setPlayersAmount`** / **`setOwnPlayer`**: Sets attributes related to the current player and multiplayer setup.
    - **`getCurrentPlayer`** / **`getPlayersAmount`** / **`getOwnPlayer`**: Gets attributes related to the current player and multiplayer setup.
- **Worlds Handling**:
    - **`createEmptyWorld`**: Creates an empty world with the given name.
    - **`addWorldFactory`**: Adds a factory function for creating worlds with the specified name.
    - **`switchWorld`**: Switches to the world with the given name.
    - **`getWorldsNames`**: Gets the names of all available worlds.
    - **`world`**: Gets a reference to the current world.
    - **`network`**: Gets a reference to the network object associated with the engine.
- **General Logic**:
    - **`run`**: Initiates the main game loop.
    - **`toggleFullscreen`**: Toggles between fullscreen and windowed modes.
- **Event Handling**:
    - **`processSwitchEvent`**: Processes events related to switching worlds.
    - **`handleEvents`**: Handles general events.
    - **`processClientsEvents`**: Processes events related to networked clients.
- **Network Handling**:
    - **`componentsUpdater`**: Updates components based on received network updates.
- **Public Attributes**:
    - **`window`**: SFML RenderWindow used by the engine.

-  **Private Methods**:

- **`addGlobalEntity`**: Adds a global entity to the engine and returns its unique identifier (**`ECS::id_t`**).
- **`removeGlobalEntity`**: Removes a global entity based on its identifier.
- **`getGlobalEntity`**: Retrieves a const reference to a global entity based on its identifier.
- **`getMutGlobalEntity`**: Retrieves a mutable reference to a global entity based on its identifier.
- **`getGlobal`**: Template method that returns an unordered map of global entities paired with component handles of the specified type.
- **`each`** (single type): Template method for iterating over global entities with a specific component type, applying a given function.
- **`each`** (multiple types): Template method for iterating over global entities with multiple component types, applying a given function.
- **`getGlobalEntitiesWithComponents`** (multiple types): Template method that returns a vector of global entities that have components of specified types.
- **`getGlobalEntityWithComponents`** (multiple types): Template method that returns a global entity with components of specified types.
- **`destroyPendingWorlds`**: Destroys worlds that are pending destruction.
- **`_eachHelper`** (single type): Helper function for the **`each`** method, applying a function to a global entity and its component handle of a specific type.
- **`_eachHelper`** (multiple types): Helper function for the **`each`** method with multiple component types, applying a function to a global entity and its component handles.

### Attributes:

- **`_running`**: Flag indicating whether the engine is currently running.
- **`_fullscreen`**: Flag indicating whether the game window is in fullscreen mode.
- **`_worldsFactories`**: Unordered map storing world factories associated with their names.
- **`_currentWorld`**: Pair representing the name and reference to the currently active world.
- **`_pending_destroy`**: Vector of world-tuples that are pending destruction.
- **`_startWorld`**: Name of the world to be set as the initial world.
- **`_windowSizeX`** / **`_windowSizeY`**: Dimensions of the game window.
- **`_network`**: The network object associated with the engine.
- **`_playersAmount`** / **`_currentPlayer`** / **`_ownPlayer`**: Attributes related to multiplayer setup.
- **`_global_entities`**:  Unordered map storing global entities associated with their identifiers.


&nbsp;


# [Components Documentation](./Engine/Components.md)
# [Events Documentation](./Engine/Events.md)
# [Systems Documentation](./Engine/Systems.md)
# [Server Documentation](./Engine/Server.md)
