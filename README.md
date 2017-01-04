# CMPT 373 Group Project: Multi-Featured Text-Based Combat Game

An input/output, text-based program in a team of 8 people to develop a mystery game that features networking, a chat system, and the ability to make new worlds and environments. This game is a semester-long project for my CMPT 373 (Software Development Methods) class

Specific tasks included:

* Using the C++ language, object-oriented programming, and knowledge of design patterns to create the "pig latin translator" functionality for the game
* Refactoring code using methods taught in class to make it more understandable for other group members to read
* Assembing the game using GitLab's version control system and the Slack team collaboration tool to plan the different objects and routines of the program in a group of 8 people

This project was administered by Dr. Nick Sumner. You can find more information about the project at his [course website](http://www.cs.sfu.ca/~wsumner/teaching/373/16/project.html).

---

# **BEBINCA'S TEXT ADVENTURE 2016**
#### Software Development Methods (CMPT 373) combat game and team project

![Alt text](https://www.sfu.ca/~pmadeya/newsplash.gif "Project Intro")

The experience of a lifetime awaits! Bebinca offers you a chance to roam through vast regions from the world of Mother Goose to Midguard and abroad. Level up with the built-in combat system to have a chance battle your friends and foes, cast spells, swap bodies all to become the most powerful being to set foot in the universe. Interact with different players by shouting words back and forth or whispering them to a teammate nearby. 

Feeling adventurous? You can build and create your very own custom worlds with a specialized world builder that will let you customize world to make your dreams become a reality.

---
## A. Requirements

You MUST install the following software on your computer for the game to work:

* **CMake** (with a minimum version 3.5.1)
* **libboost-all-dev** (Boost libraries)
* **libncurses-dev** (NCurses library)

---

## B. Getting Started

To get started, the player must first run the game server. After it has fully loaded, all of your friends will now be able to join in on the adventure of a lifetime all within the same chatroom. The usage will be as follows:

i) Clone the repository
```
https://csil-git1.cs.surrey.sfu.ca/373-16-3-bebinca/adventure2016.git
```

ii) Create a new directory for building and change into it
```
mkdir build
cd build
```

iii) Run CMake with the path to the source and run make
```
cmake ../adventure2016
make
```

iv) Run the game-server executable along with the config.yml file
```
./game-server config.yaml
```

v) Run the game-client executable and specify the same port and IP as the server
```
./game-client localhost 4000
```

---

## C. Game Description

You will commence your journey in the center of Midguard. Travel around the world with the regular compass directions by moving North, East, West, or South. Meet many characters along the way including the likes of Samylla and Xouwasi and many more! Battle your friends by casting spells on them and even swap bodies with other players in order to strategically become the most clever in all of the land! 

---

## D. Supported Commands
For a list of all supported game commands, type the Help command: `help` or `/h`. Type your intended command in the chat client, followed by parameters next to the command. These parameters will be highlighted by the angle brackets `<` and `>`. 

### **Lists**
1. The List of Players: **`players`**
    * Enumerates all players present in the current area
2. The List of Exits: **`exits`**
    * Enumerates all rooms adjacent to the current room and in which direction they are in

### **Information commands**
3. The Status command: **`status`**
    * Provides information about your current game level, health strength, and mana stock
4. The Look command: **`look <direction>`**
    * Enumerates all assets, exits, and necessary descriptions about the current room
5. The Move command: **`go <direction>`**, **`move <direction>`**, or **`g <direction>`**
    * Transfers the character from the current room to the intended, destination room (adjacent to the current room)

### **Communication commands**
6. The Say command: **`say <message>`**
    * Presents your intended message to all players in the game and provides a way for you to chat with them
    * Communicates globally across the whole game
7. The Shout command: **`shout <message>`**
    * Presents your intended message to all players in the current room and provides a way for you to chat with them
    * Communicates locally to a room only
8. The Whisper command: **`whisper <name of player> <message>`** or **`/w <name of player> <message>`**
    * Presents your intended message to the intended player in the game and provides a way for you to chat with him/her
    * Communicates locally to another player in the game and is kept confidential from the rest of the players

### **Combat commands**
9. The Casting of Spells command: **`cast <name of spell> <name of player>`** or **`/c <name of spell> <name of player>`**
    * Cast intended spells to friends and enemies to either increase or decrease their health or mana
    * The complete spell list can be shown by typing in `\c`
10. The Attack command: **`attack <name of player>`** or **`/a <name of player>`**
    * Gives the ability to compete in a dual combat with another player

---

## E. Preview of the Game

### Gameplay
![Alt text](https://www.sfu.ca/~pmadeya/Title%20screen.png "Title screen")

![Alt text](https://www.sfu.ca/~pmadeya/authentication.png "Authentication")

![Alt text](https://www.sfu.ca/~pmadeya/start_gameplay.png "First World")

![Alt text](https://www.sfu.ca/~pmadeya/Screen%20Shot%202016-11-30%20at%2010.46.35%20PM.png "Gameplay")

### Combat system
![Alt text](https://www.sfu.ca/~pmadeya/combat_ui.png "Project Intro")

---

## F. Authors
* **Brandon Chong**
* **Yui Hei Tsui**
* **Melissa Wang**
* **Aaryaman Girish**
* **Nihas Nebaskhan**
* **Zachary Cesaretti**
* **Paul Madeya**
* **Josh Arik Miguel Fernandez** 
