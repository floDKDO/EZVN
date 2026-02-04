#pragma once

#include "RAII_SDL2/animation.h"
#include "RAII_SDL2/renderer.h"
#include "drawable.h"
#include "constants.h"

#include <optional>
#include <string>

class Image : public Drawable
{
	public:
		Image(std::string_view path, int x, int y, sdl::Renderer& renderer, unsigned int zorder = constants::default_zorder_);

		void init_image(std::string_view new_path, int x, int y, sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer) override;
		void update() override;

		unsigned int zorder_;
		std::string path_;

	private:
		enum class Kind
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

		std::optional<sdl::Animation> create_animation(std::string_view path);

		std::optional<sdl::Animation> animated_image_; 
		Kind image_type_;
		int frame_index_; //int pour être comparable avec le champ "count" de IMG_Animation
};