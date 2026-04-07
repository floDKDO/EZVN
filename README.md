# EaZy Visual Novel (EZVN) engine
EZVN, a visual novel game engine written in modern C++ (C++17) and using the SDL2 library, that allows you to create visual novels easily while still having the freedom to modify many elements of the engine.

<p align="center">
  <img src="https://github.com/floDKDO/EZVN/blob/main/showcase/image1.png" alt="Image 1" width="400"/>
  <img src="https://github.com/floDKDO/EZVN/blob/main/showcase/image2.png" alt="Image 2" width="400"/>
</p>

## Build & install
For now, the engine is not yet complete: not all features have been implemented and bugs may still be present.
If you would like to test the current version (on Windows only for now), you can:
1. Download the Release
2. Inside the folder you just downloaded, type ```cmake -B build``` and ```cmake --build build```
3. Open *build/EZVN.sln* with Visual Studio 2022 and launch the project. You can also take *EZVN.exe* from *build/Debug* or *build/Release*, put it inside *build/* and run it

## Project architecture: 
- *resources/*: all the resources used by the engine. It contains multiple subfolders:
  - *fonts/*: the fonts used by the engine
  - *img/*: the images (backgrounds, character and GUI elements images) used by the engine
  - *sounds/*: the sounds and musics used by the engine
- *include/* and *src/*: the project's header and source files, respectively. These folders are divided into multiple subfolders:
  - *Color/*: classes that allow the use of multiple types of color representation (RGBA, RGBA8, HSVA, HSVA8 and hexadecimal)
  - *GameStates/*: classes that describe the game states of the engine (main menu, settings, in game, history menu)
  - *GUI/*: classes attached to the GUI system (buttons, textbuttons, texttoggles, checkboxes, inputfields, sliders)
  - *Managers/*: classes of the various engine resource managers (audio, background, character, music, sound, textbox and UI)
  - *RAII_SDL2/*: SDL2 encapsulation according to the RAII principle
  - *./*: the remaining files
- *src/main.cpp*: contains your story (an example is already written there)

## Current features: 
- Main screen, options menu (windowed/full-screen mode, music/sound volume, dialogue display speed, resolution change)
- Display characters at the desired position with an animation (similar to Ren'Py's Transform)
- Composite images to change the image of characters
- Fade transition for backgrounds
- Assign a text box, name box, and/or custom name color for each character independently
- Auto (dialog scrolls automatically based on their length) or Skip (skip dialog at high speed) mode
- Be able to return to previous dialogues using the mouse wheel
- Play music and sounds (possibly on different channels) while supporting volume changes and fade-in or fade-out effects
- Hide the textbox by right-clicking or clicking the mouse wheel button
- Rename characters
- Autofocus that highlights a character who is speaking by changing their size and zorder
- History menu that displays dialogue history
- Pop-up that displays when we try to quit the game
- Choice menus (display a dialogue based on the choice you made)

## Remaining features to be implemented: 
- Add other transitions for backgrounds (only fade for now) 
- Add a logging system to obtain information about the engine states
- Save/load system and persistent variables
- Domain Specific Language like Ren'Py's
- Drag-and-drop system using UI elements for menu creation
- Possible switch to SDL3 if it's worth it

## Dependencies:
- [SDL2 2.30.3](https://wiki.libsdl.org/SDL2/FrontPage)
- [SDL_image 2.8.2](https://wiki.libsdl.org/SDL2_image/FrontPage)
- [SDL_mixer 2.8.0](https://wiki.libsdl.org/SDL2_mixer/FrontPage)
- [SDL_ttf 2.22.0](https://wiki.libsdl.org/SDL2_ttf/FrontPage)
- [nlohmann_json 3.12.0](https://github.com/nlohmann/json)
