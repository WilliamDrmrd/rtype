# Network Communication Protocol for Game Interaction

## 1. Introduction
   This document describes a network communication protocol for exchanging game-related information between a client and a server.

## 2. Terminology
   The key words "MUST," "MUST NOT," "REQUIRED," "SHALL," "SHALL NOT," "SHOULD," "SHOULD NOT," "RECOMMENDED," "MAY," and "OPTIONAL" in this document are to be interpreted as described in RFC 2119.

## 3. Protocol Overview
   The protocol defines the communication structure between a game client and server using various packet types, component types, and update types.

## 4. Component Types
   | Code | Description                    |
   |------|--------------------------------|
   | 0    | AnimationComponent             |
   | 1    | CollisionComponent             |
   | 2    | ExcludeCollisionComponent      |
   | 3    | MovingComponent                |
   | 4    | PositionComponent              |
   | 5    | RenderableComponent            |
   | 6    | SpeedComponent                 |
   | 7    | ViewComponent                  |
   | 8    | WorldMoveProgressComponent     |
   | 9    | BaseBulletComponent            |
   | 10   | EnemyComponent                 |
   | 11   | EnemyAttackComponent           |
   | 12   | EnemyMovementsComponent        |
   | 13   | EnemyQueueComponent            |
   | 14   | MissileComponent               |
   | 15   | PlayerComponent                |
   | 16   | HealthComponent                |
   | 17   | LayeredRenderableComponent     |
   | 18   | LayeredAnimationComponent      |
   | 19   | WeaponTypeComponent            |
   | 20   | ScoreComponent                 |
   | 21   | BoosterComponent               |
   | 22   | isBoosterComponent             |
   | 23   | BoosterActiveComponent         |
   | 24   | LinkComponent                  |
   | 25   | NoneComponent                  |

## 5. Packet Types
   | Code | Name                        | Description                                   |
   |------|-----------------------------|-----------------------------------------------|
   | 0    | HandshakeRequest            | Initiates the handshake process between the client and server. The client MUST send this packet to the server to establish a connection. |
   | 1    | HandshakeResponse           | Responds to a handshake request. The server MUST send this packet to acknowledge the client's handshake request, confirming the connection establishment. |
   | 2    | SwitchWorld                 | Requests a switch to a different game world. Sent by the client when transitioning between game levels or scenarios. |
   | 3    | SwitchWorldOkForMe          | Acknowledges a successful world switch. The server MUST send this packet to confirm that the world switch request has been processed. |
   | 4    | LeaveLobby                  | Informs the server about the client's intention to leave the lobby. Used when a player wants to exit the game before or after it starts. |
   | 5    | LeaveLobbyResponse          | Responds to leaving the lobby. The server MUST acknowledge the client's request to leave the lobby. |
   | 6    | ClientIndependentInitialization | Initializes an independent client with essential information, such as its assigned player number and other client-specific details. |
   | 7    | InitializeGame              | Requests the initialization of the game. Sent by the client to signal its readiness to start the game. |
   | 8    | InitializeGameOkForMe       | Acknowledges a successful game initialization. The server MUST respond with this packet to confirm that the game is ready to commence. |
   | 9    | LaunchGame                  | Signals the start of the game. Sent by the server to notify all clients that the game has begun. |
   | 10   | KeyInputs                   | Transmits player input events. Clients MUST send this packet to inform the server about player actions, such as key presses or releases. |
   | 11   | ClientUpdate                | Conveys updates from the client to the server. Includes information about modified game entities or other relevant changes. |
   | 12   | ClientUpdateACK             | Acknowledges the receipt of a client update. Sent by the server to confirm the successful reception of client updates. |
   | 13   | GlobalState                 | Transmits the global state of the game. Includes information about various game elements and their current states. |
   | 14   | PlayerAction                | Communicates player-initiated actions. Sent by the client to inform the server about specific actions taken by a player. |
   | 15   | ActionOutcome               | Indicates the outcome of player actions. Sent by the server to update clients on the results of specific player-initiated actions. |
   | 16   | Heartbeat                   | Regular heartbeat signal for maintaining connection status. Clients and servers MUST exchange this packet at regular intervals to ensure the connection is active. |
   | 17   | PlayerDisconnected          | Notifies the server and other clients when a player disconnects from the game. |
   | 18   | Error                       | Indicates an error in the communication process. Used to report issues or unexpected events during the interaction between client and server. |
   | 19   | ErrorAcknowledged           | Acknowledges the receipt of an error notification. Sent by the server to confirm the reception of an error report from a client. |
   | 20   | EndGame                     | Requests the termination of the game. Sent by a client when ending the game session. |
   | 21   | EndGameAcknowledged         | Acknowledges the request to end the game. The server MUST respond with this packet to confirm the termination of the game. |


## 6. Update Types
   | Code | Name             | Description                          |
   |------|------------------|--------------------------------------|
   | 0    | AddComponents    | Add game components to entities      |
   | 1    | RemoveComponents | Remove game components from entities |
   | 2    | RemoveEntity     | Remove an entire entity              |

## 7. Protocol Format
   The protocol messages follow the format below:

   | Field        | Description                                     |
   |--------------|-------------------------------------------------|
   | PacketType   | Type of packet being sent or responded to       |
   | UpdateType   | Type of update (if applicable)                  |
   | ComponentType| Type of game component (if applicable)          |
   | Payload      | Data specific to the packet or update type      |





