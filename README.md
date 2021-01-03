# Dino

## Description

Dino is a 2D Platformer where our main character is a dinosaur. He has to jump cliffs and jump to high places to be able to meet again with his mother because he has been lost.  

## Key Features

- XML config file loading
- Load/Save game state using XML file
- Tiled TMX map loading and drawing (orthographic)
- Map collisions detection (platforms)
- Map navigation: player movement and jumping

- Logo screen with fade-in fade-out
- Title screen
- Win / Lose Scenes
- Player Walk / Jump / Die / Attack animations
- Ending screen (on dying)
- Music in all scenes 
- Background with parallax
- DEBUG keys 

- Walking and flying enemies with pathfind
- Game capped to 60 frames
- Fx in Game
- Collectibles
- 3 lives before dying
- Projectile with cooldown
- Dynamic GUI
- A second Level

- Entity System and EntityManager
- Scene System and SceneManager
- GUI: Title Screen Main Menu with PLAY, CONTINUED, REMOVE GAME, SETTINGS, CREDITS, EXIT
- Settings contain Music Volume, Fx Volume, FullScreen Mode eneble/disble, and VSync Off
- All buttons responsive and include diferent states: NORMAL, FOCUSED, PRESSED, DISABLE
- The buttons generate an OnMouseClick event to be processed
- All buttons have Audio Feedback when is focused, pressed and disable
- GUI: Gameplay Screen HUD: Player lives, Collectibles, Timer and Score
- GUI: Gameplay Screen Pause Menu : RESUME, SETTINGS, BACK TO TITLE, EXIT

- You can light bonfires with the fireball
- When you get out of reach, the bats return to their initial position. 

## Controls

- WASD for movement
- E for Enable / Disable travel between CheckPoints
- Shift for run
- SPACE for jump
- SPACE in the middle of the jump for double jump
- K / F / Click Left for Shoot
- ESC for open and close Menu Pause
- +/- modify volume

## DEBUG keys

- F1 Start from the first level
- F2 Start from the second level
- F3 Start from the beginning of the current level
- F4 Enable / Disable FPS counter
- F5 Save the current game state
- F6 Load the previous state
- F8 View GUI bounds rectangles and state in different colors
- F9 View colliders / logic
- F10 God Mode (fly around, can not be killed)
- F11 Enable/Disable FPS cap to 30

## Developers

 - Ismael Tejada - Programmer / Game Designer
 - Mario Hernández - Lead Programmer
 - Alejandro Moreno - Programmer / Level Designer

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

Additional Licenses:
- Tileset: https://craftpix.net/file-licenses/
