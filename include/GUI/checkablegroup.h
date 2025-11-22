#pragma once

#include "GUI/ui.h"
#include "GUI/checkable.h"
#include "text.h"

#include <memory>

//TODO : utiliser la même syntaxe que le std::array pour indiquer le nombre de checkables à la place de l'indiquer dans le constructeur => utiliser une template

class CheckableGroup : public Ui
{
	public:
		virtual ~CheckableGroup() = default;

		void uncheck_all_others(Checkable* checkable_to_not_uncheck);
		void handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck);
		void on_click(Checkable* c);

		void draw(sdl::Renderer& renderer) override;
		void update(Uint64 time_step) override;
		void handle_events(const SDL_Event& e) override;
		std::vector<Ui*> get_navigation_nodes() override;

		int y_spacing_text_;
		std::vector<std::unique_ptr<Checkable>> checkables_;
		size_t number_of_checkables_;
		Checkable* selected_checkable_; //TODO!!
		bool only_one_has_to_be_checked_;
		Text top_text_; 
		int y_spacing_between_checkable_;
		
	protected:
		CheckableGroup(size_t number_of_checkables, bool only_one_has_to_be_checked, std::string top_text, int x, int y, sdl::Renderer& renderer);

	private:
};

