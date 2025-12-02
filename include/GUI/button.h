#pragma once

#include "GUI/ui.h"
#include "image.h"
#include "text.h"

class Button : public Ui
{
	public:
		Button(const std::string text, const int x, const int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		SDL_Rect get_rect() const override; 

		Text text_; //TODO : public = temporaire pour tester les choice menu dans buttongroup.h

	protected:

	private:
		Image normal_;
		Image selected_;
		Image clicked_;

		static const unsigned int INDEX_RECT_BUTTON_ = 0;
};

