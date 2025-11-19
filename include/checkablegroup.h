#pragma once

#include "ui.h"
#include "checkable.h"
#include "text.h"
#include <memory>

class CheckableGroup : public Ui
{
	public:
		virtual ~CheckableGroup();

		void uncheck_all_others(Checkable* toggle_to_not_uncheck);
		void handle_only_one_has_to_be_checked(Checkable* toggle_to_not_uncheck);
		void on_click(Checkable* t);

		void draw(SDL_Renderer* renderer) override;
		void update(Uint64 time_step) override;
		void handle_events(const SDL_Event& e) override;
		std::vector<Ui*> get_navigation_nodes() override;

		std::vector<std::unique_ptr<Checkable>> toggles;
		size_t number_of_toggles;
		bool only_one_has_to_be_checked;
		Text top_text; //TODO

	protected:
		CheckableGroup(size_t number_of_toggles, bool only_one_has_to_be_checked, std::string top_text, int x, int y, SDL_Renderer* renderer);

	private:
};

