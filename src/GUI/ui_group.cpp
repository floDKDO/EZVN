#include "GUI/ui_group.h"
#include "GUI/checkable.h"
#include "constants.h"

#include <iostream>

UiGroup::UiGroup(int x, int y)
	: title_(nullptr), frame_({x, y, 0, 0}), only_one_has_to_be_checked_(true)
{}

UiGroup::UiGroup(std::string_view title, int x, int y, sdl::Renderer& renderer)
	: frame_({x, y, 0, 0}), title_(std::make_unique<Text>(title, constants::textbutton_normal_color_, constants::textbutton_font_, constants::textbutton_text_size_, x, y, renderer)), only_one_has_to_be_checked_(true)
{}

void UiGroup::draw(sdl::Renderer& renderer)
{
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		ui_widget->draw(renderer);
	}

	if(title_ != nullptr)
	{
		title_->draw(renderer);
	}
}

void UiGroup::update()
{
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		ui_widget->update();
	}

	if(title_ != nullptr)
	{
		title_->update();
	}
}

std::vector<UiWidget*> UiGroup::get_navigation_nodes()
{
	std::vector<UiWidget*> widgets;
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		widgets.push_back(ui_widget.get());
	}
	return widgets;
}

void UiGroup::add_ui_element(std::unique_ptr<UiWidget> widget)
{
	if(Checkable* c = dynamic_cast<Checkable*>(widget.get()); c != nullptr)
	{
		c->checkable_group_ = this;
	}

	SDL_Rect rect = widget->rect_;
	rect.x = frame_.x;
	if(ui_elements_.size() == 0)
	{
		if(title_ == nullptr)
		{
			rect.y = frame_.y;
		}
		else
		{
			rect.y = frame_.y + 80; //TODO : hardcodé (80 = espace entre le premier checkable et le titre)
		}
	}
	else
	{
		rect.y = ui_elements_.back()->rect_.y + rect.h + 20; //TODO : hardcodé (20 = espace entre chaque checkable)
	}
	widget->change_position(rect.x, rect.y);

	ui_elements_.push_back(std::move(widget));
}

void UiGroup::set_title(std::string_view title)
{
	title_->text_ = title;
}


//Les 3 méthodes suivantes sont utilisées uniquement pour les éléments de type Checkable

void UiGroup::uncheck_all_others(const Checkable* checkable_to_not_uncheck)
{
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		Checkable* c = dynamic_cast<Checkable*>(ui_widget.get());
		if(c != nullptr && c != checkable_to_not_uncheck)
		{
			c->change_checked(false);
		}
	}
}

void UiGroup::handle_only_one_has_to_be_checked(Checkable* checkable_to_not_uncheck)
{
	if(checkable_to_not_uncheck->previous_checked_) //Cas press sur un checkable déjà coché => ne pas le décocher dans ce cas
	{
		checkable_to_not_uncheck->change_checked(true);
	}
	uncheck_all_others(checkable_to_not_uncheck);
}

void UiGroup::on_press(Checkable* c)
{
	if(only_one_has_to_be_checked_ && (c->is_checked_ || c->previous_checked_))
	{
		handle_only_one_has_to_be_checked(c);
	}
}