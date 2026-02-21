#pragma once

#include "GUI/ui_widget.h"
#include "GUI/button.h"

//Pour l'instant, que de haut en bas

class Scrollbar : public UiWidget
{
	public:
		Scrollbar(int x, int y, int container_h, int handle_h, sdl::Renderer& renderer, std::function<void(Ui* ui)> callback_function);

		int get_low_bound() const;
		int get_high_bound() const;

		void update_current_value();

		void disable_keyboard_focus();
		void handle_movement(int mouse_x, int mouse_y);
		std::vector<UiWidget*> get_navigation_nodes() override;

		void on_pointer_down_hook_end(PointerEventData pointer_event_data) override;
		void on_pointer_up_hook_end(PointerEventData pointer_event_data) override;

		void on_drag(PointerEventData pointer_event_data) override;

		void on_mouse_wheel(Sint32 mouse_wheel_y) override;

		void on_up_pressed() override;
		void on_down_pressed() override;
		void on_left_pressed() override;
		void on_right_pressed() override;
		void on_enter_pressed_hook_end() override;

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void change_position(int x, int y) override;

		float current_value_;
		float min_value_;
		float max_value_;

	protected:

	private:
		SDL_Rect container_;
		SDL_Rect container_outline_;
		SDL_Rect handle_;
		SDL_Rect handle_outline_;

		Button up_triangle_;
		Button down_triangle_;

		int delta_mouse_handle_y_; //y position of the mouse in comparison to the y position of the handle
		int step_y_;

		struct InfoWhenClick 
		{
			enum class WhereWasMouse
			{
				ABOVE,
				ON,
				BELOW
			};

			int mouse_y_pos_;
			int handle_y_pos_;
			WhereWasMouse where_was_mouse_;
			bool saved_;
		};
		InfoWhenClick info_when_click_;

		bool mouse_is_on_handle_; //lors du clic, la souris n'était pas sur l'handle mais à force de la déplacer en cliquant/déplaçant la souris, cette dernière est sur l'handle
};

