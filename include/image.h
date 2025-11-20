#pragma once

#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/animation.h"
#include "RAII_SDL2/renderer.h"

#include <memory>

class Image
{
	public:
		Image(const std::string path, const int x, const int y, sdl::Renderer& renderer, const int zorder=0);

		enum class ImageType
		{
			NONE, //TODO : utile ?
			BUTTON,
			INPUTFIELD,
			SLIDER,
			TEXTBOX, 
			TEXTBUTTON, //TODO : utile ?
			TEXTTOGGLE, //TODO : utile ?
			CHECKBOX,
			GUI,
			CHARACTER,
			BACKGROUND
		};

		void show();
		void hide();

		void flip_vertically();
		void flip_horizontally();
		void flip_normal();

		void resize(const int w, const int h);
		void set_position(const int x, const int y);

		//TODO : void set_character_position(Transform t);

		void night_filter();
		void afternoon_filter();
		void own_filter(const Uint8 r, const Uint8 g, const Uint8 b);

		void draw(sdl::Renderer& renderer);

		int zorder;
		SDL_Rect position;

		Uint64 last_time; //TODO : trouver un meilleur nom

	protected:
		

	private:
		std::string path;
		ImageType image_type;
		
		Uint8 alpha;
		double angle;
		
		SDL_RendererFlip flip;
		Uint8 r, g, b;

		int frame_index;
		bool is_gif;
		std::unique_ptr<sdl::Animation> gif;

		std::unique_ptr<sdl::Texture> texture; 
		sdl::Renderer& renderer; //solution temporaire => cette classe ne possède pas / don't owns ce pointeur
};