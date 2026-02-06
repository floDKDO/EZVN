# EaZy Visual Novel (EZVN) engine
EZVN, a visual novel game engine written in modern C++ (C++17) and using the SDL2 library, that allows you to create visual novels easily while still having the freedom to modify many elements of the engine.

<p align="center">
  <img src="https://github.com/floDKDO/EZVN/blob/main/showcase/image1.png" alt="Image 1" width="500"/>
</p>

<p align="center">
  <img src="https://github.com/floDKDO/EZVN/blob/main/showcase/image2.png" alt="Image 2" width="500"/>
</p>

## Project architecture: 
- *fonts/*: the fonts used by the engine
- *img/*: the images (backgrounds, character and GUI elements images) used by the engine
- *sounds/*: the sounds and musics used by the engine
- *include/* and *src/*: the project's header and source files, respectively. These folders are divided into multiple subfolders:
  - *Color/*: classes that allow the use of multiple types of color representation (RGBA, RGBA8, HSVA, HSVA8 and hexadecimal)
  - *GUI/*: classes attached to the GUI system (buttons, textbuttons, texttoggles, checkboxes, inputfields, sliders)
  - *Managers/*: classes of the various engine resource managers (audio, background, character, music, sound, textbox and UI)
  - *RAII_SDL2/*: SDL2 encapsulation according to the RAII principle
  - *./*: the remaining files
- *src/main.cpp*: contains your story (an example is already written there)

## Current features: 
- Main screen, options menu (windowed/full-screen mode, music/sound volume, dialogue display speed)
- Display characters at the desired position with an animation (similar to Ren'Py's Transform)
- Composite images to change the image of characters
- Assign a text box, name box, and/or custom name color for each character independently
- Auto (dialog scrolls automatically based on their length) or Skip (skip dialog at high speed) mode
- Be able to return to previous dialogues using the mouse wheel
- Play music and sounds (possibly on different channels) while supporting volume changes and fade-in or fade-out effects
- Hide the textbox by clicking or scrolling the mouse wheel
- Rename characters
- Autofocus that highlights a character who is speaking by changing their size and zorder

## Remaining features to be implemented: 
- Background animation
- Add a scrollable area (for dialogues history for example)
- Add a logging system to obtain information about the engine states
- Save/load system and persistent variables
- Domain Specific Language like Ren'Py's
- Drag-and-drop system using UI elements for menu creation
- Possible switch to SDL3 if it's worth it

## Dependencies: 
- [SDL2](https://wiki.libsdl.org/SDL2/FrontPage)
- [SDL_image](https://wiki.libsdl.org/SDL2_image/FrontPage)
- [SDL_mixer](https://wiki.libsdl.org/SDL2_mixer/FrontPage)
- [SDL_ttf](https://wiki.libsdl.org/SDL2_ttf/FrontPage)
