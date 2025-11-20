#pragma once

#include "GUI/checkable.h"
#include "image.h"

class CheckboxGroup;

class Checkbox : public Checkable
{
	//friend class CheckboxGroup;

	public:
		Checkbox(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		void draw(sdl::Renderer& renderer) override;
		void update(Uint64 time_step) override;
		SDL_Rect get_rect() const override;

	protected:

	private:
		Image normal;
		Image selected;
		Image clicked;
		Image checked; //TODO : renommer ??*/

		static const unsigned int INDEX_RECT_CHECKBOX = 0;
};

