# Engine/Events Technical Documentation

# Collision.event.hpp
```cpp
#pragma once

#include <cstddef>

#include "ECS/Entity.hpp"

struct CollisionEvent {
        ECS::Entity *movingEntity;
        ECS::Entity *collidingEntity;
};
```
The **`CollisionEvent`** structure represent collision events between entities in an Entity-Component-System (ECS) architecture.

### Member Variables:

- **`ECS::Entity *movingEntity`**: Represents the pointer to the entity that is in motion and involved in the collision event.
- **`ECS::Entity *collidingEntity`**: Represents the pointer to the entity that the **`movingEntity`** is colliding with.

This structure serves as an event payload to convey collision information between entities in the ECS. It encapsulates details about the entities involved in the collision event, allowing systems or components within the ECS to react or respond accordingly to handle collisions between different entities.

# KeyPressed.event.hpp
```cpp
#pragma once

#include <SFML/Window/Event.hpp>
#include <cstddef>

struct KeyPressedEvent {
        sf::Event::KeyEvent keyEvent;
};
```
e **`KeyPressedEvent`** structure represent a key press event, storing details about the specific key that has been pressed.

### Member Variables:

- **`sf::Event::KeyEvent keyEvent`**: Represents an SFML **`KeyEvent`** struct, storing information about the key that was pressed, including the key code, the state of the Alt, Control, Shift, and System keys, and whether the event is repeated.

This structure serves as a wrapper or payload to convey information specifically related to key press events in an application using SFML. It allows for the encapsulation and transmission of key press details, enabling systems or components to react or respond to specific key presses within an event-driven architecture using SFML.

# KeyReleased.event.hpp
```cpp
#pragma once

#include <SFML/Window/Event.hpp>
#include <cstddef>

struct KeyReleasedEvent {
        sf::Event::KeyEvent keyEvent;
};
```
The **`KeyReleasedEvent`** structure represent a key release event, storing details about the specific key that has been released.

### Member Variables:

- **`sf::Event::KeyEvent keyEvent`**: Represents an SFML **`KeyEvent`** struct, storing information about the released key, including the key code, the state of the Alt, Control, Shift, and System keys, and whether the event is repeated.

This structure serves as a wrapper or payload to convey information specifically related to key release events in an application using SFML. It allows for the encapsulation and transmission of key release details, enabling systems or components to react or respond to specific key releases within an event-driven architecture using SFML.

# Rezise.event.hpp
```cpp
#pragma once

#include <cstddef>

struct ResizeEvent {
        ResizeEvent(std::size_t x, std::size_t y) : x(x), y(y) {}

        std::size_t x;
        std::size_t y;
};
```
The **`Resize.event.hpp`** file introduces a **`ResizeEvent`** structure designed to capture information related to a window resize event.

### **ResizeEvent Structure:**

- **Constructor:**
    - **`ResizeEvent(std::size_t x, std::size_t y)`**: Initializes the **`ResizeEvent`** object with provided width (**`x`**) and height (**`y`**) values.
- **Member Variables:**
    - **`std::size_t x`**: Stores the width of the resized window.
    - **`std::size_t y`**: Stores the height of the resized window.

This structure is intended to encapsulate data specific to a window's resizing in an application. It serves as a means to convey and handle information related to window dimension changes. The **`ResizeEvent`** can be used to propagate and process these changes throughout the system in response to window resizing events.