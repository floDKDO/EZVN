#pragma once

#include "GUI/ui_widget.h"
#include "image.h"
#include "text.h"

enum class TextPosition
{
	ON,
	UNDER
};

class Button : public UiWidget
{
	public:
		Button(std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, TextPosition text_position = TextPosition::ON);
		Button(std::string_view path_normal, std::string_view path_selected, std::string_view path_pressed, std::string_view text, int x, int y, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function, TextPosition text_position = TextPosition::ON);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void change_position(int x, int y) override;

		Image normal_; //TODO : remettre en private
		Image selected_; //TODO : remettre en private
		Image pressed_; //TODO : remettre en private
		Text text_; //TODO : remettre en private

	private:
		TextPosition text_position_;
};

