#pragma once

#include "RAII_SDL2/texture.h"
#include "RAII_SDL2/animation.h"
#include "RAII_SDL2/renderer.h"
#include "drawable.h"

#include <memory>

class Image : public Drawable
{
	public:
		Image(const std::string path, const int x, const int y, sdl::Renderer& renderer, const int zorder=0);

		void draw(sdl::Renderer& renderer) override;

	private:
		std::unique_ptr<sdl::Animation> animated_image_;

	public:
		int zorder_;

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

		int frame_index_;
		bool is_animated_;

		sdl::Renderer& renderer_; //solution temporaire => cette classe ne possède pas / don't owns ce pointeur
};