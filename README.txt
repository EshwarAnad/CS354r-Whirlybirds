Wirlybirds
Thomas Custer, Steven Wilken, Robby Nevels, Micah Turner

GOAL:
Destroy the other players’ helicopters using fish missiles.

HOW TO RUN:
1. execute ./buildit and then run ./whirlybirds
2. Click the Tutorial button
OR
2. Click the Server button to start a host game
3. Run another instance of the game on another machine and click the client button, and input
the host computer’s name

CONTROLS:
WASD = forward, left, backward, right
Shift/Space = up, down
Mouse = turn
left mouse button = fire missiles
X = mute sounds

DESCRIPTION OF ARCHITECTURE:
Our architecture emerged from the background radiation of the universe. We used the same game engine design that we created earlier in the semester, with a main GameObject class to encapsulate both Ogre and Bullet elements. We also included a SoundSystem class to abstract sounds. 

A major addition to our overall architecture was code to handle the network. We used a low-level UDPNetEnt class to handle sending and receiving network packets, which was used by both a Server and Client class to handle interpreting and casted network bytes into useful data. That useful data was defined in ServerToClient and ClientToServer structs. The Server and Client classes are used by a Game class, which handles most game state. The Whirlybirds class is our Ogre GUI class, containing the render loop. Each frame, this class asks its Game object to send/receive data to/from the Client/Server (depending on how the user made the game). 

BETTER THAN PLANNED:
The level design
Supporting N players required less debugging than expected.
Particle effects on missiles and burning helicopters
Lights on front of helicopters

WORSE THAN PLANNED:
Powerups only work on the server side
Multiplayer is laggy
Sound on the clients sometimes doesn’t play
Keeping track of player scores became intrusive

DIVISION OF LABOR
Robby: Primarily network stuff and some lighting effects
Thomas: Primarily helicopter movement, powerups, sound
Micah: Fish, particles, particles on network
Steven: Level design, Collision handling, some game logic, miscellaneous bugfixes



