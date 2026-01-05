#pragma once

#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/animation.h"
#include "RAII_SDL2/renderer.h"
#include "drawable.h"
#include "constants.h"

#include <memory>
#include <optional>

class Image : public Drawable
{
	public:
		Image(const std::string_view path, const int x, const int y, sdl::Renderer& renderer, const int zorder = constants::default_zorder_);

		void init_image(const std::string_view new_path, const int x, const int y, sdl::Renderer& renderer);
		void draw(sdl::Renderer& renderer) override;

		int zorder_;
		std::string path_;

	private:
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

		std::optional<sdl::Animation> create_animation(const std::string_view path);

		std::optional<sdl::Animation> animated_image_; 
		ImageType image_type_;
		int frame_index_;
};