#pragma once

#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/animation.h"
#include "RAII_SDL2/renderer.h"

#include <memory>

class Image
{
	friend class TransformStep;

	public:
		Image(const std::string path, const int x, const int y, sdl::Renderer& renderer, const int zorder=0);

		void show();
		void hide();

		void rotate(const double angle);

		void flip_vertically();
		void flip_horizontally();
		void flip_normal();

		int get_xcenter() const;
		void zoom(const float zoom);
		void resize(const int w, const int h);
		void set_position(const int x, const int y);
		void set_position_xcenter(const int x, const int y);
		void set_center();

		void night_filter();
		void afternoon_filter();
		void own_filter(const Uint8 r, const Uint8 g, const Uint8 b);

		void draw(sdl::Renderer& renderer);

		SDL_Rect position_;
		int zorder_;
		Uint64 last_time_;

	protected:
		

	private:
		std::string path_;

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

		ImageType image_type_;
		
		SDL_Rect initial_rect_;
		
		Uint8 alpha_;
		double angle_;
		
		SDL_RendererFlip flip_;
		Uint8 r_, g_, b_;

		int frame_index_;
		bool is_animated_;
		std::unique_ptr<sdl::Animation> animated_image_;

		std::unique_ptr<sdl::Texture> texture_; 
		sdl::Renderer& renderer_; //solution temporaire => cette classe ne possède pas / don't owns ce pointeur
};