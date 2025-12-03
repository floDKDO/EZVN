#pragma once

#include "GUI/ui.h"
#include "GUI/checkable.h"
#include "text.h"

#include <memory>

class CheckableGroup : public Ui
{
	public:
		virtual ~CheckableGroup() = default;

		void uncheck_all_others(const Checkable* checkable_to_not_uncheck);
		void handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck);
		void on_click(Checkable* c);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void handle_events(const SDL_Event& e) override;
		std::vector<Ui*> get_navigation_nodes() override;

		int y_spacing_text_;
		std::vector<std::unique_ptr<Checkable>> checkables_;
		size_t number_of_checkables_;
		Checkable* selected_checkable_; 
		bool only_one_has_to_be_checked_;
		Text top_text_; 
		
	protected:
		CheckableGroup(const size_t number_of_checkables, const bool only_one_has_to_be_checked, const std::string top_text, const int x, const int y, sdl::Renderer& renderer);

	private:
};

