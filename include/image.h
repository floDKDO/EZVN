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

		int zorder_;
		SDL_Rect position_;

		Uint64 last_time_; //TODO : trouver un meilleur nom

	protected:
		

	private:
		std::string path_;
		ImageType image_type_;
		
		Uint8 alpha_;
		double angle_;
		
		SDL_RendererFlip flip_;
		Uint8 r_, g_, b_;

		int frame_index_;
		bool is_gif_;
		std::unique_ptr<sdl::Animation> gif_;

		std::unique_ptr<sdl::Texture> texture_; 
		sdl::Renderer& renderer_; //solution temporaire => cette classe ne possède pas / don't owns ce pointeur
};