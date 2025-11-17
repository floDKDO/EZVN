#pragma once

#include "ui.h"
#include "image.h"

class Toggle : public Ui
{
	friend class ToggleGroup;

	public:
		Toggle(const std::string path_normal, const std::string path_selected, const std::string path_clicked, const std::string path_checked, const int x, const int y, bool is_checked, SDL_Renderer* renderer, std::function<void(Ui* ui)> callback_function);

		void on_pointer_up_hook_end() override;
		void on_enter_released_hook_end() override;
		void draw(SDL_Renderer* renderer) override;
		void update(Uint64 time_step) override;
		SDL_Rect get_rect() const override;

		bool is_checked;

	protected:

	private:
		Image normal;
		Image selected;
		Image clicked;
		Image checked; //TODO : renommer ??*/

		static const unsigned int INDEX_RECT_TOGGLE = 0;
};

