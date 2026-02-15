#pragma once

#include "GUI/ui_widget.h"
#include "image.h"
#include "text.h"

class Button : public UiWidget
{
	public:
		Button(std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void change_position(int x, int y) override;

	private:
		static const unsigned int index_rect_button_;

	public:
		Text text_; //TODO : public = temporaire pour tester les choice menu dans buttongroup.h

	private:
		Image normal_;
		Image selected_;
		Image pressed_;
};

