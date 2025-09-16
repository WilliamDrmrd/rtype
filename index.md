# R-Type Engine Documentation

## Introduction

On these pages you will find the R-Type usage examples and the API usage.

## Initialization :

#### Engine Initialization :

To create the engine you need to use the `Engine::EngineClass::getEngine()` method.
This method will return the engine instance or will a create a new one if None exists.
**You must store the result as a reference.**

```c++
int main(int argc, char **argv, char **env)
{
	Engine::EngineClass &engine = Engine::EngineClass::getEngine();
	// ...
}
```

Creating the engine will create the window and set it to its default size :

```cpp
#define DEFAULT_WINDOW_SIZE_X 800

#define DEFAULT_WINDOW_SIZE_Y 600
```

You can override these values by redefining the macros.

#### Worlds Initialization :

The engine handles the different levels / menus of you game as Worlds.
If no world is created a default one will be instantiated by the engine.

Worlds are stored by the engine as a function that will created them.
The default world that is loaded is `"default"` you can either redefine the `DEFAULT_WINDOW_NAME` macro to set the name at compile time or the `EngineClass::setStartWorld` method.

```cpp
engine.setStartWorld("menu"); // will set the default world to "menu"
engine.addWorldFactory("menu", GameWorld::createMenuWorld); // you need to add the world factory
```

To add a new world you need to create a function that will create all its entities, subscribers and systems except the renderer that will be added automatically. (you can override this behavior by defining the `DONT_ADD_RENDERER_SYSTEM` macro)

[GameWorld.hpp](./include/R-Type/GameWorld/GameWorld.hpp)

```c++
namespace GameWorld
{
    inline ECS::World *createGameWorld()
    {
        auto world = new ECS::World();

        // Setup view and create entities
        Entities::createViewEntity(world);
        // ... other entities
        Entities::createScoreEntities(world);

        // Add systems
        Systems::addPhysics(world);
        // ... other systems
        Systems::addBoosterSystem(world);

        // Subscribe to events
        Subscribers::subscribeToEvents(world);

        return world;
    }

	// ...
}
```

[main.cpp](./src/main.cpp)

```cpp
#include "Engine/Engine.hpp"

int main(int argc, char **argv, char **env)
{
	Engine::EngineClass &engine = Engine::EngineClass::getEngine();

    try { // you might want to use a try catch for in case of errors
        engine.setStartWorld("menu");

	    // examples from our game
        engine.addWorldFactory("menu", GameWorld::createMenuWorld);
        // ... other worlds
        engine.addWorldFactory("waitingClient", GameWorld::createWaitingClient);

        engine.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}
```

#### Subscribers

To add a subscriber to your world you need to use the `ECS::World::subscribe<T>` method where `T` is the type of the event.

[Subscribers.hpp](./include/R-Type/GameWorld/Subscribers.hpp)

```cpp
inline void subscribeToEvents(ECS::World *world)
{
	auto *subMovePressed  = new Rtype::Subscriber::PlayerMovePressedSubscriber();
	auto *subMoveReleased = new Rtype::Subscriber::PlayerMoveReleasedSubscriber();
	auto *subCollision    = new Rtype::Subscriber::CollisionEventSubscriber();
	auto *subShootPlayer  = new Rtype::Subscriber::ShootPlayerReleasedSubscriber();
	auto *subShootPlayer2 = new Rtype::Subscriber::ShootPlayerPressedSubscriber();
	auto *subEnnemyDeath  = new Rtype::Subscriber::EnnemyDeathSubscriber();
	auto *subChangeWeapon = new Rtype::Subscriber::ChangeWeapon();

	world->subscribe<KeyPressedEvent>(subMovePressed);
	world->subscribe<KeyReleasedEvent>(subMoveReleased);
	world->subscribe<CollisionEvent>(subCollision);
	world->subscribe<KeyReleasedEvent>(subShootPlayer);
	world->subscribe<KeyPressedEvent>(subShootPlayer2);
	world->subscribe<KeyPressedEvent>(subChangeWeapon);
	world->subscribe<Rtype::Events::EnnemyDeath>(subEnnemyDeath);
}
```

Obviously the subscriber must be derived from `ECS::EventSubscriber<T>` where `T` is the same as in the subscribe.

#### Entities && Components

To add entities to the world use the `ECS::World::createEntity<typename... Components>` API.
You can create all the entities component with `new Component`. These component will be converted to a `std::shared_ptr<T>` and will be stored in the entity.

The method returns the entity ID which is also stored inside the entity itself.

[Entities.hpp](./include/R-Type/GameWorld/Entities.hpp)

```cpp
inline void createPlayerEntities(ECS::World *world, int playerNb)
{
	using namespace Engine::Components;
	std::vector<std::pair<int, int>> playerPos = [{100, 300}, {100, 360}, {100, 420}, {100, 480}];

	world->createEntity(
		new PlayerComponent(playerNb),
		new PositionComponent(playerPos[playerNb].first, playerPos[playerNb].second),
		new RenderableComponent("./assets/MainShip/MainShip-Base-Fullhealth.png",
		0, 0, 2, 90),
		new LayeredRenderableComponent(
			1, new RenderableComponent(
			"./assets/MainShip/MainShip-Engines-BaseEngine-Powering.png", 0, 0, 1, 90),
			new RenderableComponent(
			"./assets/MainShip/MainShip-Weapons-BigSpaceGun-Croped.png", 0, 0, 1, 90,
									{1, 1}, false, false),
			new RenderableComponent(
			"./assets/MainShip/MainShip-Weapons-Rockets-Croped.png", 0, 0, 1, 90),
			new RenderableComponent(
			"./assets/MainShip/MainShip-Engines-BaseEngine.png", 0, 0, 1, 90)),
		new LayeredAnimationComponent(new AnimationComponent(0, 0, 48, 48, 48, 48,
		100, 4)),
		new CollisionComponent(9, 11, 30, 26), new SpeedComponent(150),
		new WeaponTypeComponent(Rtype::Components::WeaponType::BASIC));
}
```

## Implementation guide

#### Subscribers

The subscribers are classes that derive from [`ECS::EventSubscriber<T>`](./doc/html/classECS_1_1EventSubscriber.html) where the type `T` is the event type.

[Keyboard.subscriber.hpp](./include/R-Type/Subscribers/PlayerMove.subscriber.hpp)

```cpp
    class PlayerMovePressedSubscriber : public virtual ECS::EventSubscriber<KeyPressedEvent>
    {
        public:
            PlayerMovePressedSubscriber()           = default;
            ~PlayerMovePressedSubscriber() override = default;
            void receiveEvent(const std::string &name, const KeyPressedEvent &data) override
            {
                if (!(data.keyEvent.code == sf::Keyboard::Z
                || data.keyEvent.code == sf::Keyboard::Q
                || data.keyEvent.code == sf::Keyboard::S
                || data.keyEvent.code == sf::Keyboard::D))
                    return;

                Engine::System::MovePlayer *movePlayerSystem =
                dynamic_cast<Engine::System::MovePlayer *>(
	                WORLD.getSystems()["MovePlayer"].get()
	            );

                if (!movePlayerSystem) return;

                movePlayerSystem->addMovePlayer(data.keyEvent);
            }
    };
```

The `receiveEvent` will be called when a new event is broadcasted.

#### Systems

Systems will be the classes handling all the logic (like gravity, movement or enemy AI)
They must derive from [`ECS::BaseSystems`](./doc/html/classECS_1_1BaseSystem.html) ([source code](./include/ECS/System.hpp))

The `tick` method will be called every world tick.

**You can get entities and components using the [World API](./doc/html/classECS_1_1World.html) :**

- `bool ECS::World::entityExists(id_t entityId)` Will check if the entity exists.
- `const Entity &ECS::World::getEntity(id_t id) const` will get an immutable entity from the world.
- `Entity &ECS::World::getMutEntity(id_t id)` will get an immutable entity from the world.
- `void ECS::World::each<T>(std::function<void(Entity *, ComponentHandle<T>)> func)` other definition available. This method will call the lambda on all the entities that have the components passed in the template `T`.
- `std::vector<Entity *> ECS::World::getEntitiesWithComponents<...T>()` Will get all the entities that have the components passed in the template `T`.
- `Entity *ECS::World::getEntityWithComponents<... T>()` Will get the first entity that have the components passed in the template `T`.

[**example here**](./src/Engine/Systems/Animation.system.cpp)

#### Components

Components are basic data structures that only store data for an entity.
They must derive from [`ECS::BaseComponent`](./doc/html/classECS_1_1BaseComponent.html).

[Missile.component.hpp](./include/R-Type/Components/Missile.component.hpp)

```cpp
struct MissileComponent : public ECS::BaseComponent {
	public:
		MissileComponent() = default;

		~MissileComponent() override = default;

		std::vector<char> serialize() override
		{
			std::ostringstream oss(std::ios::binary);

			const std::string &str = oss.str();
			return {str.begin(), str.end()};
		}

		ECS::BaseComponent *deserialize(std::vector<char> vec,
		ECS::BaseComponent *component) final
		{
			MissileComponent *missileComponent;
			if (component == nullptr) {
				missileComponent = new MissileComponent();
			} else {
				missileComponent = dynamic_cast<MissileComponent *>(component);
				if (missileComponent == nullptr) return nullptr;
			}

			std::istringstream iss(std::string(vec.begin(),
			vec.end()), std::ios::binary);

			return missileComponent;
		}

		ComponentType getType() override
		{
			return ComponentType::MissileComponent;
		}

	private:
};
```

## In code documentation and classes graphs

#### [doxygen](./doc/html/index.html)
