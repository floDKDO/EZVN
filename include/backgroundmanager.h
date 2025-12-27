#pragma once

#include "background.h"

#include <map>

class BackgroundManager
{
	public:
		BackgroundManager(unsigned int& unique_id, unsigned int& current_unique_id, sdl::Renderer& renderer);

		void insert_background(const std::string_view background_path);
		void insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);

		void change_background(const Background& b);

		void draw();
		void update();

	protected:


	private:
		std::map<unsigned int, Background> backgrounds_;
		Background background_;

		unsigned int& unique_id_;
		unsigned int& current_unique_id_;
		sdl::Renderer& renderer_;
};

