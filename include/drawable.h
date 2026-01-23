#pragma once

#include "RAII_SDL2/renderer.h"
#include "RAII_SDL2/texture.h"
#include "Color/color.h"

#include <memory>

class Drawable
{
	friend class TransformStep;

	public:
		virtual void draw(sdl::Renderer& renderer) = 0;
		virtual void update() = 0;

		void show();
		void hide();
		void set_alpha(Uint8 alpha);

		void rotate(double angle);

		void flip_vertically();
		void flip_horizontally();
		void flip_normal();

		int get_xcenter() const;
		int get_ycenter() const;
		void zoom(float zoom);
		void resize(int w, int h);
		void set_position(int x, int y);
		void set_position_xcenter(int x);
		void set_position_ycenter(int y);
		void set_position_xycenter(int x, int y);
		void set_position_xoffset(int x);
		void set_position_yoffset(int y);
		void set_center();

		void night_filter();
		void afternoon_filter();
		void change_color(Color color);

		Uint64 last_time_;
		SDL_Rect position_;
		
	protected:
		Drawable(sdl::Renderer& renderer, Color color = Color::from_rgba8(255, 255, 255));

		std::unique_ptr<sdl::Texture> texture_;
		SDL_Rect initial_rect_;
		double angle_;
		SDL_RendererFlip flip_;
		Color color_; 

		sdl::Renderer& renderer_; //do not own it !!
		
	private:
};

